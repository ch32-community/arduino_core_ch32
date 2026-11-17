#include "NetworkMdns.h"
#include "internal/NetworkCore.h"

#include <string.h>

#include "lwip/apps/mdns.h"
#include "lwip/def.h"
#include "lwip/prot/iana.h"
#include "lwip/netif.h"
#include "lwip/udp.h"

NetworkMdns MDNS;

namespace {
struct Service {
  char service[17]; /* "_http" */
  uint8_t proto;    /* DNSSD_PROTO_TCP / UDP */
  uint16_t port;
  uint8_t txtLength;
  uint8_t txt[96]; /* items, each after a length byte */
  bool used;
};

bool running;
bool explicitInstance;
char host[MDNS_LABEL_MAXLEN + 1];
char instance[MDNS_LABEL_MAXLEN + 1];
Service services[MDNS_MAX_SERVICES];

struct InterfaceServices {
  struct netif *netif;
  int8_t slots[MDNS_MAX_SERVICES]; // lwIP slots; negative means not registered
};
InterfaceServices interfaces[ch32rv_net::MAX_INTERFACES];
} // namespace

static void txt_items(struct mdns_service *service, void *userdata) {
  const Service *s = static_cast<const Service *>(userdata);
  for (uint8_t at = 0; at < s->txtLength; at = (uint8_t)(at + 1 + s->txt[at])) {
    mdns_resp_add_service_txtitem(
        service, reinterpret_cast<const char *>(s->txt + at + 1), s->txt[at]);
  }
}

static bool add_services(struct netif *n) {
  if (!n->state) {
    return true; // lwIP's loopback interface has no NetworkInterface owner.
  }
  InterfaceServices *registered = nullptr;
  for (auto &entry : interfaces) {
    if (entry.netif == n) {
      registered = &entry;
      break;
    }
  }
  if (!registered) {
    for (auto &entry : interfaces) {
      if (!entry.netif) {
        entry.netif = n;
        for (auto &slot : entry.slots) {
          slot = -1;
        }
        registered = &entry;
        break;
      }
    }
  }
  if (!registered ||
      (!mdns_resp_netif_active(n) && mdns_resp_add_netif(n, host) != ERR_OK)) {
    return false;
  }
  bool success = true;
  for (size_t i = 0; i < MDNS_MAX_SERVICES; i++) {
    Service &s = services[i];
    if (s.used && registered->slots[i] < 0) {
      registered->slots[i] = mdns_resp_add_service(n, instance, s.service,
                                                   (enum mdns_sd_proto)s.proto,
                                                   s.port, txt_items, &s);
      if (registered->slots[i] < 0) {
        success = false;
      }
    }
  }
  mdns_resp_announce(n);
  return success;
}

static bool add_all_services() {
  bool success = true;
  struct netif *n;
  NETIF_FOREACH(n) {
    if (!add_services(n)) {
      success = false;
    }
  }
  return success;
}

/* Interfaces starting and stopping, from the Network library. */
static void netif_changed(struct netif *n, bool up) {
  if (!n->state) {
    return; /* the loopback interface: no NetworkInterface, nothing to announce
             */
  }
  if (up) {
    add_services(n); // addService() or begin() can retry a failed allocation.
  } else {
    if (mdns_resp_netif_active(n)) {
      mdns_resp_remove_netif(n);
    }
    for (auto &entry : interfaces) {
      if (entry.netif == n) {
        entry.netif = nullptr;
      }
    }
  }
}

bool NetworkMdns::begin(const char *hostName) {
  if (!hostName || !*hostName || strlen(hostName) > MDNS_LABEL_MAXLEN) {
    return false;
  }
  ch32rv_net::start();
  end();
  strcpy(host, hostName);
  if (!explicitInstance) {
    strcpy(instance, hostName);
  }
  static bool initialised;
  if (!initialised) {
    // lwIP's void initializer assumes allocation and bind succeed. Probe both
    // first; no yield or other allocator runs between releasing and reusing it.
    struct udp_pcb *probe = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!probe) {
      return false;
    }
    const err_t bound = udp_bind(probe, IP_ANY_TYPE, LWIP_IANA_PORT_MDNS);
    udp_remove(probe);
    if (bound != ERR_OK) {
      return false;
    }
    mdns_resp_init();
    initialised = true;
  }
  running = true;
  ch32rv_net::setNetifListener(netif_changed);
  if (!add_all_services()) {
    end();
    return false;
  }
  return true;
}

void NetworkMdns::end() {
  if (!running) {
    return;
  }
  running = false;
  ch32rv_net::setNetifListener(nullptr);
  struct netif *n;
  NETIF_FOREACH(n) { netif_changed(n, false); }
}

bool NetworkMdns::setInstanceName(const char *name) {
  if (!name || strlen(name) > MDNS_LABEL_MAXLEN) {
    return false;
  }
  strcpy(instance, name);
  explicitInstance = *name != '\0';
  return true;
}

/* "http" or "_http" -> "_http". */
static bool label(char *out, size_t size, const char *in) {
  if (!in || !*in) {
    return false;
  }
  const char *bare = in[0] == '_' ? in + 1 : in;
  if (strlen(bare) + 2 > size) {
    return false;
  }
  out[0] = '_';
  strcpy(out + 1, bare);
  return true;
}

static Service *find(const char *service, const char *proto) {
  char name[sizeof services[0].service];
  if (!label(name, sizeof name, service)) {
    return nullptr;
  }
  const uint8_t p =
      (proto && strstr(proto, "udp")) ? DNSSD_PROTO_UDP : DNSSD_PROTO_TCP;
  for (auto &s : services) {
    if (s.used && s.proto == p && !strcmp(s.service, name)) {
      return &s;
    }
  }
  return nullptr;
}

bool NetworkMdns::addService(const char *service, const char *proto,
                             uint16_t port) {
  if (find(service, proto)) {
    return !running || add_all_services();
  }
  for (auto &s : services) {
    if (!s.used) {
      if (!label(s.service, sizeof s.service, service)) {
        return false;
      }
      s.proto =
          (proto && strstr(proto, "udp")) ? DNSSD_PROTO_UDP : DNSSD_PROTO_TCP;
      s.port = port;
      s.txtLength = 0;
      s.used = true;
      return !running || add_all_services();
    }
  }
  return false;
}

bool NetworkMdns::addServiceTxt(const char *service, const char *proto,
                                const char *key, const char *value) {
  Service *s = find(service, proto);
  if (!s || !key) {
    return false;
  }
  const size_t k = strlen(key), v = value ? strlen(value) : 0;
  const size_t item = k + (value ? 1 + v : 0);
  if (!k || strchr(key, '=') || item > 63) {
    return false;
  }
  size_t offset = s->txtLength, replaced = 0;
  for (size_t i = 0; i < s->txtLength; i += 1 + s->txt[i]) {
    const char *oldKey = (const char *)s->txt + i + 1;
    const char *equal = (const char *)memchr(oldKey, '=', s->txt[i]);
    const size_t oldKeyLength = equal ? (size_t)(equal - oldKey) : s->txt[i];
    if (oldKeyLength == k && lwip_strnicmp(oldKey, key, k) == 0) {
      offset = i;
      replaced = 1 + s->txt[i];
      break;
    }
  }
  const size_t newLength = s->txtLength - replaced + 1 + item;
  if (newLength > sizeof s->txt) {
    return false;
  }
  uint8_t *at = s->txt + offset;
  memmove(at + 1 + item, at + replaced, s->txtLength - offset - replaced);
  *at++ = (uint8_t)item;
  memcpy(at, key, k);
  if (value) {
    at[k] = '=';
    memcpy(at + k + 1, value, v);
  }
  s->txtLength = (uint8_t)newLength;
  if (running) {
    struct netif *n;
    NETIF_FOREACH(n) {
      if (mdns_resp_netif_active(n)) {
        mdns_resp_announce(n);
      }
    }
  }
  return true;
}

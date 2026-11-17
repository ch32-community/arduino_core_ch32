#include <Network.h>
#include "internal/NetworkCore.h"
#include "internal/NetworkDhcpServer.h"
#include "internal/NetworkTcp.h"
#include "lwip/apps/mdns.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/inet_chksum.h"
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/prot/dhcp.h"
#include "lwip/prot/ethernet.h"
#include "lwip/prot/ip4.h"
#include "lwip/prot/udp.h"
#include "lwip/udp.h"

// USB 't' runs once after reset. DHCP servers use isolated interfaces below;
// their frames never reach the Ethernet MAC or the surrounding LAN.
// Commands a/b/c/d exercise mDNS names and TXT replacement for a host browser.
// Link with -Wl,--wrap=yield to inject cancellation while retaining TinyUSB's
// real yield implementation. Normal CI builds compile this hook without using it.
class IsolatedInterface : public NetworkInterface {
public:
  bool begin(uint8_t id) {
    uint8_t mac[6] = {2, 0, 0, 0, 0, id};
    return netifStart(mac);
  }
  void end() { netifStop(); }
  void link(bool up) { setLinkUp(up); }
  void receive(const uint8_t *frame, size_t length) { frameReceived(frame, length); }
protected:
  bool transmitFrame(const uint8_t *, size_t) override { return true; }
};

unsigned failures, checks;
bool ran;
NetworkClient *stopOnYield;
NetworkClient *replaceOnYield;
void *heapBlocks[512];

extern "C" void __real_yield(void);
extern "C" void __wrap_yield(void) {
  __real_yield();
  if (stopOnYield) {
    NetworkClient *client = stopOnYield;
    stopOnYield = nullptr;
    client->stop();
    if (replaceOnYield) {
      *client = *replaceOnYield;
    }
  }
}

void setup() { Serial.begin(115200); }

void loop() {
  Network.poll();
  if (!Serial.available()) return;
  char command = Serial.read();
  if (command == 't' && !ran) {
    ran = true;
    Serial.print("Regression CPU="); Serial.print(F_CPU); Serial.print("\n\r");
    testRegistry();
    testDhcpServer();
    testDhcpRecovery();
    testDnsRenewal();
    testTcpUdp();
    testMdnsFailures();
    check("Ethernet starts", Ethernet.begin());
    uint32_t start = millis();
    while (!Ethernet.hasIP() && millis() - start < 20000) Network.poll();
    check("Ethernet gets DHCP address", Ethernet.hasIP());
    check("mDNS default old name", MDNS.begin("metro-regression-old"));
    check("mDNS initial TXT", MDNS.addServiceTxt("regtest", "tcp", "path", "/"));
    Serial.print("REGRESSION COMPLETE checks="); Serial.print(checks);
    Serial.print(" failures="); Serial.print(failures); Serial.print("\n\r");
  }
  if (command == 'a') {
    MDNS.end();
    check("mDNS refreshed default", MDNS.begin("metro-regression-new"));
  }
  if (command == 'b') {
    MDNS.setInstanceName("metro-regression-custom");
    MDNS.end();
    check("mDNS explicit name", MDNS.begin("metro-regression-third"));
  }
  if (command == 'c') {
    MDNS.end();
    check("mDNS preserved explicit name", MDNS.begin("metro-regression-fourth"));
  }
  if (command == 'd') {
    check("mDNS replace case-insensitive TXT key",
          MDNS.addServiceTxt("regtest", "tcp", "PaTh", "/new"));
  }
  Serial.print("IP="); Serial.print(Ethernet.localIP());
  Serial.print(" failures="); Serial.print(failures); Serial.print("\n\r");
}

void check(const char *name, bool success) {
  ++checks;
  if (!success) ++failures;
  Serial.print(success ? "PASS " : "FAIL "); Serial.print(name); Serial.print("\n\r");
}

unsigned interfaceCount() {
  unsigned count = 0;
  struct netif *n;
  NETIF_FOREACH(n) { ++count; }
  return count;
}

void testRegistry() {
  IsolatedInterface interfaces[5];
  unsigned original = interfaceCount();
  bool success = true;
  for (unsigned i = 0; i < 4; ++i) {
    interfaces[i].config(IPAddress(10, 42, i, 1));
    success = interfaces[i].begin(i + 1) && success;
  }
  check("four interfaces registered", success);
  check("fifth interface fails without leaking netif",
        !interfaces[4].begin(5) && !interfaces[4].started() && interfaceCount() == original + 4);
  interfaces[1].end();
  check("registry slot reusable", interfaces[4].begin(5));
}

NetworkDhcpServer *serverFor(struct netif *n) {
  for (udp_pcb *p = udp_pcbs; p; p = p->next) {
    if (p->local_port == 67 && p->netif_idx == netif_get_index(n)) {
      return (NetworkDhcpServer *)p->recv_arg;
    }
  }
  return nullptr;
}

void testDhcpServer() {
  IsolatedInterface iface;
  iface.config(IPAddress(10, 42, 7, 254));
  iface.begin(7);
  check("reject broadcast DHCP lease", !iface.dhcpServer(true, 1));
  iface.config(IPAddress(10, 42, 7, 1));
  const uint32_t address = lwip_ntohl((uint32_t)iface.localIP());
  const uint32_t mask = lwip_ntohl((uint32_t)iface.subnetMask());
  check("reject network, cross-subnet, wrapping, empty and own-address pools",
        !NetworkDhcpServer::validPool(address, mask, address - 1, 1) &&
        !NetworkDhcpServer::validPool(address, mask, address + 253, 2) &&
        !NetworkDhcpServer::validPool(address, mask, UINT32_MAX, 2) &&
        !NetworkDhcpServer::validPool(address, mask, address + 1, 0) &&
        !NetworkDhcpServer::validPool(address, mask, address, 1));
  check("valid DHCP pool starts", iface.dhcpServer(true, 4));
  NetworkDhcpServer *s = serverFor(iface.netif());
  if (!s) { check("server state available", false); return; }
  s->leases[0].state = NetworkDhcpServer::BOUND;
  s->leases[0].until = millis() + 3600000;
  s->leases[0].mac[5] = 42;
  iface.dhcpDns(IPAddress(1, 1, 1, 1));
  iface.dhcpAdvertiseGateway(true);
  s = serverFor(iface.netif());
  check("DNS/router updates preserve active lease", s &&
        s->leases[0].state == NetworkDhcpServer::BOUND && s->leases[0].mac[5] == 42 &&
        s->dns == (uint32_t)IPAddress(1, 1, 1, 1) && s->router == (uint32_t)iface.localIP());
  if (!s) return;
  s->leases[0].state = NetworkDhcpServer::OFFERED;
  s->leases[1].state = NetworkDhcpServer::BOUND;
  s->leases[2].state = NetworkDhcpServer::DECLINED;
  for (unsigned i = 0; i < 3; ++i) s->leases[i].until = millis() - 1;
  Network.poll();
  bool expired = true;
  for (unsigned i = 0; i < 3; ++i) expired = expired && s->leases[i].state == NetworkDhcpServer::ENDED;
  check("idle polling expires offered/bound/declined leases", expired);
  for (unsigned i = 0; i < 3; ++i) s->leases[i].until = millis() - 0x80000001u;
  Network.poll();
  check("expired lease cannot revive after signed clock half-range",
        s->leases[0].state == NetworkDhcpServer::ENDED && s->leases[1].state == NetworkDhcpServer::ENDED &&
        s->leases[2].state == NetworkDhcpServer::ENDED);
  iface.dhcpServer(true, 2);
  s = serverFor(iface.netif());
  check("pool resize creates fresh leases", s && s->count == 2 && s->leases[0].state == NetworkDhcpServer::UNUSED);
}

unsigned fillUdp(udp_pcb *held[16]) {
  unsigned count = 0;
  while (count < 16 && (held[count] = udp_new_ip_type(IPADDR_TYPE_V4))) ++count;
  return count;
}

void testDhcpRecovery() {
  for (unsigned method = 0; method < 2; ++method) {
    IsolatedInterface iface;
    if (method) iface.config(IPAddress(10, 42, 8, 1));
    iface.begin(8);
    if (method) iface.link(true);
    udp_pcb *held[16];
    unsigned count = fillUdp(held);
    if (method) iface.config();
    else iface.link(true);
    struct dhcp *d = netif_dhcp_data(iface.netif());
    check("DHCP allocation failure reproduced", count && (!d || !d->pcb_allocated));
    if (count) udp_remove(held[--count]);
    Network.poll();
    d = netif_dhcp_data(iface.netif());
    check(method ? "DHCP config retries after resources recover" : "DHCP link-up retries after resources recover",
          d && d->pcb_allocated && d->state == DHCP_STATE_SELECTING);
    while (count) udp_remove(held[--count]);
  }
}

void injectDhcp(IsolatedInterface &iface, uint8_t type) {
  uint8_t frame[14 + IP_HLEN + UDP_HLEN + sizeof(dhcp_msg)] = {};
  eth_hdr *eth = (eth_hdr *)frame;
  memset(eth->dest.addr, 255, 6);
  eth->src.addr[0] = 2; eth->src.addr[5] = 99;
  eth->type = PP_HTONS(ETHTYPE_IP);
  ip_hdr *ip = (ip_hdr *)(frame + 14);
  IPH_VHL_SET(ip, 4, 5);
  IPH_LEN_SET(ip, lwip_htons(sizeof(frame) - 14));
  IPH_TTL_SET(ip, 64);
  IPH_PROTO_SET(ip, IP_PROTO_UDP);
  ip->src.addr = (uint32_t)IPAddress(10, 42, 9, 1);
  ip->dest.addr = IPADDR_BROADCAST;
  IPH_CHKSUM_SET(ip, inet_chksum(ip, IP_HLEN));
  udp_hdr *udp = (udp_hdr *)(frame + 14 + IP_HLEN);
  udp->src = PP_HTONS(67); udp->dest = PP_HTONS(68);
  udp->len = lwip_htons(UDP_HLEN + sizeof(dhcp_msg));
  dhcp_msg *msg = (dhcp_msg *)(frame + 14 + IP_HLEN + UDP_HLEN);
  msg->op = DHCP_BOOTREPLY; msg->htype = 1; msg->hlen = 6;
  msg->xid = lwip_htonl(netif_dhcp_data(iface.netif())->xid);
  msg->yiaddr.addr = (uint32_t)IPAddress(10, 42, 9, 42);
  memcpy(msg->chaddr, iface.netif()->hwaddr, 6);
  msg->cookie = PP_HTONL(DHCP_MAGIC_COOKIE);
  uint8_t options[] = {DHCP_OPTION_MESSAGE_TYPE, 1, type, DHCP_OPTION_SERVER_ID, 4, 10, 42, 9, 1,
                       DHCP_OPTION_SUBNET_MASK, 4, 255, 255, 255, 0,
                       DHCP_OPTION_LEASE_TIME, 4, 0, 0, 14, 16,
                       DHCP_OPTION_DNS_SERVER, 4, 8, 8, 8, 8, DHCP_OPTION_END};
  memcpy(msg->options, options, sizeof(options));
  iface.receive(frame, sizeof(frame));
  Network.poll();
}

void testDnsRenewal() {
  IsolatedInterface iface;
  iface.config(IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress(1, 1, 1, 1));
  iface.begin(9);
  iface.link(true);
  injectDhcp(iface, DHCP_OFFER);
  injectDhcp(iface, DHCP_ACK);
  check("injected DHCP ACK binds address", iface.localIP() == IPAddress(10, 42, 9, 42));
  check("initial DNS override retained", iface.dnsIP() == IPAddress(1, 1, 1, 1));
  dhcp_renew(iface.netif());
  injectDhcp(iface, DHCP_ACK);
  check("same-address renewal retains DNS override",
        netif_dhcp_data(iface.netif())->state == DHCP_STATE_BOUND && iface.dnsIP() == IPAddress(1, 1, 1, 1));
}

void testTcpUdp() {
  const IPAddress loopback(127, 0, 0, 1);
  NetworkUDP receiver, sender;
  receiver.begin(4343);
  const uint8_t message[] = {1, 0, 3, 254, 5};
  bool sent = sender.beginPacket(loopback, 4343) && sender.write(message, sizeof(message)) == sizeof(message) && sender.endPacket();
  uint32_t start = millis();
  int size = 0;
  while (!(size = receiver.parsePacket()) && millis() - start < 1000) Network.poll();
  uint8_t received[sizeof(message)] = {};
  check("first UDP datagram preserves payload without begin",
        sent && size == sizeof(message) && receiver.read(received, sizeof(received)) == sizeof(received) &&
        memcmp(message, received, sizeof(message)) == 0);
  receiver.stop(); sender.stop();

  NetworkServer server(4342, 4);
  server.begin();
  NetworkClient client;
  bool connected = client.connect(loopback, 4342, 1000);
  check("loopback TCP connects", connected);
  if (!connected) return;
  NetworkClient peer = server.accept();
  NetworkClient *copies = new NetworkClient[300];
  check("allocate 300 TCP client copies", copies != nullptr);
  if (!copies) return;
  for (unsigned i = 0; i < 300; ++i) copies[i] = client;
  delete[] copies;
  client.write(message, sizeof(message));
  start = millis();
  while (peer.available() < sizeof(message) && millis() - start < 1000) Network.poll();
  check("300 TCP copies release without corrupting live client",
        client.connected() && peer.read(received, sizeof(received)) == sizeof(received) &&
        memcmp(message, received, sizeof(message)) == 0);
  stopOnYield = &client;
  check("connected tolerates stop from yield", !client.connected());
  stopOnYield = nullptr;
  peer.stop();

  stopOnYield = &client;
  start = millis();
  check("connect tolerates stop from yield", !client.connect(loopback, 4342, 1000) && millis() - start < 500);
  stopOnYield = nullptr;
  NetworkClient pending = server.accept(); pending.stop();

  NetworkClient replacement;
  connected = replacement.connect(loopback, 4342, 1000);
  NetworkClient replacementPeer = server.accept();
  replaceOnYield = &replacement;
  stopOnYield = &client;
  bool cancelled = !client.connect(loopback, 4342, 1000);
  stopOnYield = nullptr;
  replaceOnYield = nullptr;
  pending = server.accept(); pending.stop();
  check("cancelled connect preserves replacement connection", connected && cancelled && client.connected());
  client.stop(); replacement.stop(); replacementPeer.stop();
}

void testMdnsFailures() {
  udp_pcb *held[16];
  unsigned count = fillUdp(held);
  check("mDNS UDP exhaustion returns failure", count && !MDNS.begin("no-pcb"));
  if (count) udp_remove(held[--count]);
  check("mDNS retries after UDP capacity recovers", MDNS.begin("available-pcb"));
  MDNS.end();
  while (count) udp_remove(held[--count]);

  IsolatedInterface iface;
  iface.config(IPAddress(10, 42, 10, 1));
  iface.begin(10); iface.link(true);
  check("mDNS starts on isolated interface", MDNS.begin("isolated-mdns"));
  size_t used = 0;
  while (used < 512 && (heapBlocks[used] = mem_malloc(64))) ++used;
  check("mDNS service allocation failure reported", !MDNS.addService("regtest", "tcp", 4242));
  while (used) mem_free(heapBlocks[--used]);
  check("same mDNS service retries successfully", MDNS.addService("regtest", "tcp", 4242));
  check("repeat mDNS service does not allocate duplicates", MDNS.addService("regtest", "tcp", 4242));
  check("mDNS other service slots remain usable",
        mdns_resp_add_service(iface.netif(), "one", "_direct1", DNSSD_PROTO_TCP, 1, nullptr, nullptr) >= 0 &&
        mdns_resp_add_service(iface.netif(), "two", "_direct2", DNSSD_PROTO_TCP, 2, nullptr, nullptr) >= 0 &&
        mdns_resp_add_service(iface.netif(), "three", "_direct3", DNSSD_PROTO_TCP, 3, nullptr, nullptr) >= 0);
  IsolatedInterface late;
  late.config(IPAddress(10, 42, 11, 1));
  while (used < 512 && (heapBlocks[used] = mem_malloc(64))) ++used;
  bool started = late.begin(11);
  check("late interface mDNS allocation failure reproduced",
        started && !mdns_resp_netif_active(late.netif()));
  while (used) mem_free(heapBlocks[--used]);
  check("mDNS retries missing interface without duplicate services",
        MDNS.addService("regtest", "tcp", 4242) && mdns_resp_netif_active(late.netif()));
  MDNS.end();
}

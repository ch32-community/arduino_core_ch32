// Arduino compiles only files under src/. Collect the enabled lwIP sources here
// so the pinned upstream submodule can stay unchanged outside that directory.

// Core and transport protocols.
#include "../lwip/src/core/def.c"
#include "../lwip/src/core/dns.c"
#include "../lwip/src/core/inet_chksum.c"
#include "../lwip/src/core/init.c"
#include "../lwip/src/core/ip.c"
#include "../lwip/src/core/mem.c"
#include "../lwip/src/core/memp.c"
#include "../lwip/src/core/netif.c"
#include "../lwip/src/core/pbuf.c"
#include "../lwip/src/core/stats.c"
#include "../lwip/src/core/sys.c"
#include "../lwip/src/core/tcp.c"
#include "../lwip/src/core/tcp_in.c"
#include "../lwip/src/core/tcp_out.c"
#include "../lwip/src/core/timeouts.c"
#include "../lwip/src/core/udp.c"

// IPv4 and Ethernet.
#include "../lwip/src/core/ipv4/acd.c"
#include "../lwip/src/core/ipv4/autoip.c"
#include "../lwip/src/core/ipv4/dhcp.c"
#include "../lwip/src/core/ipv4/etharp.c"
#include "../lwip/src/core/ipv4/icmp.c"
#include "../lwip/src/core/ipv4/igmp.c"
#include "../lwip/src/core/ipv4/ip4.c"
#include "../lwip/src/core/ipv4/ip4_addr.c"
#include "../lwip/src/core/ipv4/ip4_frag.c"
#include "../lwip/src/netif/ethernet.c"

// mDNS responder.
#include "../lwip/src/apps/mdns/mdns.c"
#include "../lwip/src/apps/mdns/mdns_domain.c"
#include "../lwip/src/apps/mdns/mdns_out.c"

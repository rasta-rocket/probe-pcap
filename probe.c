#include "probe.h"

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
  fprintf(stdout, "Here is the captured packets:\n");
  while(*packet){
    fprintf(stdout, "%02x", *packet++);
  }
  fprintf(stdout, "\n");
}

int probe_run(char *device, char *filters){
  pcap_t *session;
  char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */

  struct bpf_program fp;		/* The compiled filter */

  bpf_u_int32 mask;		/* netmask */
  bpf_u_int32 net;		        /* IP */

  struct pcap_pkthdr header;	/* The header that pcap gives us */
  const u_char *packet;		/* The actual packet */

  /* Find the properties for the device */
  if (pcap_lookupnet(device, &net, &mask, errbuf) == -1) {
    fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, errbuf);
    net = 0;
    mask = 0;
  }

  /* Open the session in promiscuous mode */
  session = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
  if (session == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
    return 2;
  }

  /* Compile and apply the filter */
  if (pcap_compile(session, &fp, filters, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filters, pcap_geterr(session));
    return 2;
  }
  if (pcap_setfilter(session, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filters, pcap_geterr(session));
    return 2;
  }

  /* Test to catch single packet
  packet = pcap_next(session, &header);
  Display its length
  printf("Packet catched with length of [%d]\n", header.len);
  */

  /* Get packets into a loop */
  pcap_loop(session, -1, process_packet, NULL);

  pcap_close(session);
  return 0;
}

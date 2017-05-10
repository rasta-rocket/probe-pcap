#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <stdio.h>

const char *argp_program_version = "probe-pcap 1.0.0";
const char *argp_program_bug_address = "<bruno.fernandos@yahoo.com>";
static char doc[] = "pcap exercise";
static char args_doc[] = "[FILTERS...]";
static struct argp_option options[] = {
  { "interfaces", 'i', "<name>", 0, "interface name"},
  { 0 }
};

struct arguments {
  char dev[10];
  char *pcap_filter;
  int size;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'i': strcpy(arguments->dev, arg); break;
    case ARGP_KEY_ARG:
    //printf("%s\n", arg);
    if(arguments->pcap_filter==NULL){
      arguments->size = strlen(arg) + 2;
      arguments->pcap_filter = (char *) malloc(arguments->size * sizeof(char));
    }
    else{
      arguments->size += strlen(arg) + 2;
      arguments->pcap_filter = (char *) realloc(arguments->pcap_filter, arguments->size * sizeof(char));
    }
    //printf("%d\n", arguments->size);
    if((arg != NULL) && (strchr(arg, '_'))){
      sprintf(arguments->pcap_filter, "%s %s", arguments->pcap_filter, strsep(&arg, "_"));
    }
    if((arg != NULL) && (strchr(arg, '='))){
      sprintf(arguments->pcap_filter, "%s %s", arguments->pcap_filter, strsep(&arg, "="));
    }
    if(arg != NULL){
      sprintf(arguments->pcap_filter, "%s %s", arguments->pcap_filter, arg);
    }
    //printf("RESULT: %s\n", arguments->pcap_filter);
    break;
    case ARGP_KEY_END:
    if (state->arg_num < 0){
      /* Not enough arguments. */
      argp_usage (state);
    }
    break;
    default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

void manage_args(int argc, char *argv[], struct arguments *arguments){
  argp_parse(&argp, argc, argv, 0, 0, arguments);
}

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
  fprintf(stdout, "Here is the captured packets:\n%s\n", packet);
}

int main(int argc, char *argv[]){
  pcap_t *session;
  char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */

  struct bpf_program fp;		/* The compiled filter */

  bpf_u_int32 mask;		/* netmask */
  bpf_u_int32 net;		        /* IP */

  struct pcap_pkthdr header;	/* The header that pcap gives us */
  const u_char *packet;		/* The actual packet */

  struct arguments arguments;
  arguments.pcap_filter = NULL;
  arguments.size = 0;
  manage_args(argc, argv, &arguments);

  fprintf(stdout, "Device choosen : %s\n", arguments.dev);
  fprintf(stdout, "Filter expression : %s\n", arguments.pcap_filter);
  //return 0;
  /* Find the properties for the device */
  if (pcap_lookupnet(arguments.dev, &net, &mask, errbuf) == -1) {
    fprintf(stderr, "Couldn't get netmask for device %s: %s\n", arguments.dev, errbuf);
    net = 0;
    mask = 0;
  }

  /* Open the session in promiscuous mode */
  session = pcap_open_live(arguments.dev, BUFSIZ, 1, 1000, errbuf);
  if (session == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", arguments.dev, errbuf);
    return(2);
  }

  /* Compile and apply the filter */
  if (pcap_compile(session, &fp, arguments.pcap_filter, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", arguments.pcap_filter, pcap_geterr(session));
    return(2);
  }
  if (pcap_setfilter(session, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", arguments.pcap_filter, pcap_geterr(session));
    return(2);
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

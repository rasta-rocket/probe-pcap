#include "arguments.h" 

#include <stdlib.h>
#include <string.h>
#include <argp.h>

const char *argp_program_version = "probe-pcap 1.0.0";
const char *argp_program_bug_address = "<bruno.fernandos@yahoo.com>";
static char doc[] = "pcap exercise";
static char args_doc[] = "[FILTERS...]";
static struct argp_option options[] = {
  { "interfaces", 'i', "<name>", 0, "interface name"},
  { 0 }
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'i': strcpy(arguments->dev, arg); break;
    case ARGP_KEY_ARG:
    //printf("%s\n", arg);
    if(arguments->filters==NULL){
      arguments->size = strlen(arg) + 2;
      arguments->filters = (char *) malloc(arguments->size * sizeof(char));
    }
    else{
      arguments->size += strlen(arg) + 2;
      arguments->filters = (char *) realloc(arguments->filters, arguments->size * sizeof(char));
    }
    //printf("%d\n", arguments->size);
    if((arg != NULL) && (strchr(arg, '_'))){
      sprintf(arguments->filters, "%s %s", arguments->filters, strsep(&arg, "_"));
    }
    if((arg != NULL) && (strchr(arg, '='))){
      sprintf(arguments->filters, "%s %s", arguments->filters, strsep(&arg, "="));
    }
    if(arg != NULL){
      sprintf(arguments->filters, "%s %s", arguments->filters, arg);
    }
    //printf("RESULT: %s\n", arguments->filters);
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

void arguments_parse(int argc, char *argv[], struct arguments *arguments){
  argp_parse(&argp, argc, argv, 0, 0, arguments);
}


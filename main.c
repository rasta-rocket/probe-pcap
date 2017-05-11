#include "arguments.h"
#include "probe.h"

#include <stdio.h>

int main(int argc, char *argv[]){
  struct arguments arguments;
  arguments.filters = NULL;
  arguments.size = 0;

  arguments_parse(argc, argv, &arguments);

  fprintf(stdout, "Device choosen : %s\n", arguments.dev);
  fprintf(stdout, "Filter expression : %s\n", arguments.filters);
  
  probe_run(arguments.dev, arguments.filters);  
  return 0;
}

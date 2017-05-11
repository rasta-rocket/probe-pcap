#ifndef ARGUMENTS_H
#define ARGUMENTS_H
struct arguments {
  char dev[10];
  char *filters;
  int size;
};

void arguments_parse(int argc, char *argv[], struct arguments *arguments);
#endif

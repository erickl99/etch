#include "cat.h"
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int cat_file(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "cat-file: Expected hash after command.\n");
    return -1;
  }
  chdir(".evc/objects/");
  FILE *fp = fopen(argv[2], "rb");
  if (fp == NULL) {
    fprintf(stderr, "cat-file: The provided hash does not exist in the object database.\n");
    return -1;
  }
  char buffer[BUFFER_SIZE];
  int bytes_read;
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
    printf("%.*s", bytes_read, buffer);
  }
  fclose(fp);
  return 0;
}

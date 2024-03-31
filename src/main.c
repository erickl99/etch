#include "hash.h"
#include "cat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: evc [command]\n");
    exit(0);
  }
  char *command = argv[1];
  if (strcmp(command, "init") == 0) {
    mkdir(".evc", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(".evc/objects", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    printf("Created .evc directory\n");
  } else if (strcmp(command, "hash-object") == 0) {
    int result = hash_object(argc, argv);
    if (result < 0) {
      exit(1);
    }
  } else if (strcmp(command, "cat-file") == 0) {
    int result = cat_file(argc, argv);
    if (result < 0) {
      exit(1);
    }
  }
  return 0;
}

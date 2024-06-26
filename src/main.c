#include "cat.h"
#include "hash.h"
#include "write.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: etch [command]\n");
    exit(0);
  }
  char *command = argv[1];
  if (strcmp(command, "init") == 0) {
    mkdir(".etch", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(".etch/objects", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    printf("Created .etch directory\n");
  } else if (strcmp(command, "hash-object") == 0) {
    if (argc < 3) {
      fprintf(stderr, "hash-object: Expected file name after command\n");
    }
    char *file_hash;
    int result = hash_file_object(argv[2], &file_hash);
    if (result < 0) {
      exit(1);
    }
    printf("%s\n", file_hash + 5);
    free(file_hash);
  } else if (strcmp(command, "cat-file") == 0) {
    int result = cat_file(argc, argv);
    if (result < 0) {
      exit(1);
    }
  } else if (strcmp(command, "write-tree") == 0) {
    int result = write_tree(argc, argv);
    if (result < 0) {
      exit(1);
    }
  } else {
    fprintf(stderr, "Invalid command provided: %s\n", command);
    exit(1);
  }
  return 0;
}

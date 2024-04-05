#include "hash.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int traverse() {
  DIR *dir = opendir(".");
  struct dirent *dir_entry;
  while ((dir_entry = readdir(dir)) != NULL) {
    int not_self = strcmp(dir_entry->d_name, ".");
    int not_parent = strcmp(dir_entry->d_name, "..");
    int not_etch = strcmp(dir_entry->d_name, ".etch");
    if (not_self && not_parent && not_etch) {
      if (dir_entry->d_type == DT_DIR) {
        chdir(dir_entry->d_name);
        traverse();
        chdir("..");
      } else if (dir_entry->d_type == DT_REG) {
        char *file_hash;
        if (hash_file_object(dir_entry->d_name, &file_hash) < 0) {
          printf("We couldn't hash %s!\n", dir_entry->d_name);
        } else {
          free(file_hash);
        }
      }
    }
  }
  // TODO: Create tree hash object of current directory!
  return 0;
}

int write_tree(int argc, char *argv[]) {
  char *dir_name = ".";
  if (argc == 3) {
    dir_name = argv[2];
  }
  if (chdir(dir_name) < 0) {
    fprintf(stderr, "The directory %s does not exist or cannot be accessed\n",
            dir_name);
  }
  printf("Starting traversal through %s\n", dir_name);
  return traverse();
}

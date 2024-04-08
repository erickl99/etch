#include "hash.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INIT_SIZE 10

typedef struct {
  unsigned int count;
  unsigned int capacity;
  char **strings;
} str_list;

static void str_list_init(str_list *ls) {
  ls->count = 0;
  ls->capacity = INIT_SIZE;
  ls->strings = malloc(sizeof(char *) * INIT_SIZE);
}

static void str_list_append(str_list *ls, char *string) {
  if (ls->count + 1 > ls->capacity) {
    unsigned int new_cap = ls->capacity * 2;
    ls->strings = realloc(ls->strings, sizeof(char *) * new_cap);
    ls->capacity = new_cap;
  }
  ls->strings[ls->count++] = string;
}

static void str_list_free(str_list *ls) {
  for (int i = 0; i < ls->count; i++) {
    free(ls->strings[i]);
  }
  free(ls->strings);
}

static int traverse(char **hash) {
  DIR *dir = opendir(".");
  struct dirent *dir_entry;
  str_list hashes;
  str_list_init(&hashes);
  while ((dir_entry = readdir(dir)) != NULL) {
    int not_self = strcmp(dir_entry->d_name, ".");
    int not_parent = strcmp(dir_entry->d_name, "..");
    int not_etch = strcmp(dir_entry->d_name, ".etch");
    if (not_self && not_parent && not_etch) {
      if (dir_entry->d_type == DT_DIR) {
        chdir(dir_entry->d_name);
        char *next_dir_hash;
        traverse(&next_dir_hash);
        chdir("..");
      } else if (dir_entry->d_type == DT_REG) {
        char *file_hash;
        if (hash_file_object(dir_entry->d_name, &file_hash) < 0) {
          printf("We couldn't hash %s!\n", dir_entry->d_name);
        } else {
          str_list_append(&hashes, file_hash);
        }
      }
    }
  }
  str_list_free(&hashes);
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
  char *tree_hash;
  return traverse(&tree_hash);
}

#include "utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char etch_fp[512] = {0};

char *get_etch_file_path() {
  if (etch_fp[0] != 0) {
    printf("Returning cached value\n");
    return etch_fp;
  }
  char prev_dir[512];
  getcwd(prev_dir, 512);
  DIR *cwd = opendir(".");
  DIR *prev = cwd;
  int found = 0;
  struct dirent *entry;
  do {
    while ((entry = readdir(cwd)) > 0 && !found) {
      if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".etch") == 0) {
        realpath(entry->d_name, etch_fp);
        found = 1;
      }
    }
    chdir("..");
    prev = cwd;
    cwd = opendir(".");
  } while (prev != cwd && !found);
  chdir(prev_dir);
  return etch_fp;
}

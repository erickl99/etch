#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int traverse(char *dir_name) {
  chdir(dir_name);
  DIR *dir = opendir(".");
  if (dir == NULL) {
    fprintf(stderr, "Failed to open directory %s\n", dir_name);
    return -1;
  }
  printf("Contents of %s: {\n", dir_name);
  struct dirent *dir_entry;
  while ((dir_entry = readdir(dir)) != NULL) {
    int not_self = strcmp(dir_entry->d_name, ".");
    int not_parent = strcmp(dir_entry->d_name, "..");
    int not_etch = strcmp(dir_entry->d_name, ".etch");
    if (not_self && not_parent && not_etch) {
      if (dir_entry->d_type == DT_DIR) {
        traverse(dir_entry->d_name);
        chdir("./..");
      } else {
        printf("%s\n", dir_entry->d_name);
      }
    }
  }
  printf("}\n");
  return 0;
}

int write_tree(int argc, char *argv[]) {
  char *dir_name = ".";
  if (argc == 3) {
    dir_name = argv[2];
  }
  return traverse(dir_name);
}

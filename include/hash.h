#ifndef hash_h
#define hash_h

int hash_file_object(char *file_path, char **file_hash);
int hash_tree_object(char *dir_path, char **hashes, char**dir_hash);
#endif /* ifndef hash_h */

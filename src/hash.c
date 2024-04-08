#include "hash.h"
#include "utils.h"
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOB_HEADER "blob"
#define BLOB_HEADER_SIZE 4
#define BUFFER_SIZE 4096
#define DIR_BUFFER_SIZE 512

int hash_file_object(char *file_path, char **file_hash) {
  FILE *rfp = fopen(file_path, "rb");
  if (rfp == NULL) {
    fprintf(stderr, "hash-object: File %s does not exist\n", file_path);
    return -1;
  }
  char *etch_fp = get_etch_file_path();
  if (etch_fp == NULL) {
    fprintf(stderr,
            "hash-object: No .etch directory found. Run etch init first.\n");
    fclose(rfp);
    return -1;
  }
  char prev_dir[DIR_BUFFER_SIZE];
  getcwd(prev_dir, DIR_BUFFER_SIZE);
  printf("Previous dir is %s\n", prev_dir);
  chdir(etch_fp);
  chdir("objects");

  const EVP_MD *md = EVP_sha1();
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, md, NULL);
  unsigned char buffer[BUFFER_SIZE];
  size_t bytes_read;
  do {
    bytes_read = fread(buffer, 1, BUFFER_SIZE, rfp);
    EVP_DigestUpdate(ctx, buffer, bytes_read);
  } while (bytes_read == BUFFER_SIZE);
  unsigned int hash_len;
  EVP_DigestFinal(ctx, buffer, &hash_len);
  EVP_MD_CTX_free(ctx);

  char *full_hash = malloc(2 * hash_len + (BLOB_HEADER_SIZE + 1) + 1);
  memcpy(full_hash, BLOB_HEADER, BLOB_HEADER_SIZE);
  full_hash[BLOB_HEADER_SIZE] = ' ';
  char *new_name = full_hash + BLOB_HEADER_SIZE + 1;
  for (int i = 0; i < hash_len; i++) {
    sprintf(new_name + 2 * i, "%02x", buffer[i]);
  }
  new_name[2 * hash_len] = '\0';

  FILE *wfp = fopen(new_name, "wb");
  rewind(rfp);
  memcpy(buffer, BLOB_HEADER, BLOB_HEADER_SIZE);
  buffer[BLOB_HEADER_SIZE] = '\0';
  fwrite(buffer, 1, BLOB_HEADER_SIZE + 1, wfp);
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, rfp)) > 0) {
    fwrite(buffer, 1, bytes_read, wfp);
  }

  *file_hash = full_hash;
  fclose(wfp);
  fclose(rfp);
  chdir(prev_dir);
  return 0;
}

int hash_tree_object(char *dir_path, char **hashes, char **dir_hash) {
  return 0;
}

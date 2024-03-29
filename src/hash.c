#include "hash.h"
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int hash_object(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "hash-object: Expected file name\n");
    return -1;
  }
  char *file_path = argv[2];
  FILE *rfp = fopen(file_path, "rb");
  if (rfp == NULL) {
    fprintf(stderr, "File %s does not exist\n", file_path);
    return -1;
  }
  if (chdir(".evc/objects/") < 0) {
    fprintf(stderr, "No .evc directory found. Run etch init first.\n");
    fclose(rfp);
    return -1;
  }
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
  char *new_name = malloc(2 * hash_len + 1);
  for (int i = 0; i < hash_len; i++) {
    printf("%02x", buffer[i]);
    sprintf(new_name + 2 * i, "%02x", buffer[i]);
  }
  printf("\n");
  EVP_MD_CTX_free(ctx);
  new_name[2 * hash_len] = '\0';
  FILE *wfp = fopen(new_name, "wb");
  rewind(rfp);
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, rfp)) > 0) {
    fwrite(buffer, 1, bytes_read, wfp);
  }
  free(new_name);
  fclose(wfp);
  fclose(rfp);
  return 0;
}

#include <openssl/evp.h>
#include <openssl/opensslv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: evc [command]\n");
    exit(0);
  }
  char *command = argv[1];
  if (strcmp(command, "init") == 0) {
    mkdir(".evc", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    printf("Created .evc directory");
  } else if (strcmp(command, "hash-object")) {
    if (argc < 3) {
      fprintf(stderr, "Expected file name\n");
      exit(1);
    }
    const EVP_MD *md = EVP_sha1();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, NULL);
    char buffer[BUFFER_SIZE];
    fopen(argv[2], "rb");
    EVP_DigestUpdate(ctx, NULL, 0);
    EVP_MD_CTX_free(ctx);
  }
  return 0;
}

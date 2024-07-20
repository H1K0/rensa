#include <stdlib.h>
#include <stdio.h>


int rensa_encrypt(const char *plain, size_t len, const char *key, char zeroth_char, char *cipher);

int rensa_decrypt(const char *cipher, size_t len, const char *key, char zeroth_char, char *plain);

int rensa_fencrypt(FILE *input, FILE *output, FILE *key, size_t key_shift, char zeroth_char);

int rensa_fdecrypt(FILE *input, FILE *output, FILE *key, size_t key_shift, char zeroth_char);

#ifndef UTIL_H
#define UTIL_H

void xor_encrypt(char string[], size_t string_len, char *password);
void xor_decrypt(char string[], size_t string_len, char *password);
int compare_key(char *string, char *key);

#endif
#include <stdlib.h>
#include <string.h>

void xor_encrypt(char string[], size_t string_len, char *password)
{
    size_t passwd_len = strlen(password);
    for (int i = 0; i < string_len; i++)
    {
        string[i] = string[i] ^ password[(i % passwd_len)];
    }
}

void xor_decrypt(char string[], size_t string_len, char *password)
{
    size_t passwd_len = strlen(password);
    for (int i = 0; i < string_len; i++)
    {
        string[i] = string[i] ^ password[(i % passwd_len)];
    }
}

int compare_key(char *string, char *key)
{
    size_t key_len = strlen(key);
    if (0 != memcmp(string, key, key_len))
        return -1;
    return 1;
}
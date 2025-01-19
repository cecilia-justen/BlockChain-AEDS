#include <openssl/sha.h>
#include <stdio.h>

int main() {
    unsigned char data[] = "teste";
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(data, sizeof(data) - 1, hash);

    printf("SHA-256: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return 0;
}

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>

int main() {
    printf("OpenSSL versão: %s\n", OpenSSL_version(OPENSSL_VERSION));
    return 0;
}

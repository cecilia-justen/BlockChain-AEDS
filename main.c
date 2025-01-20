#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Estrutura de um bloco
typedef struct Block {
    int index;
    char previous_hash[65];
    char data[256];
    char hash[65];
    int nonce; // N�mero usado na prova de trabalho
    struct Block *next;
} Block;

// Fun��o para calcular o hash SHA-256
void calculate_hash(Block *block, char *output) {
    char input[512];
    snprintf(input, sizeof(input), "%d%s%s%d", 
             block->index, block->previous_hash, block->data, block->nonce);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)input, strlen(input), hash);

    // Converte o hash para uma string hexadecimal
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0'; // Finaliza a string com nulo
}

// Fun��o para realizar a prova de trabalho
void proof_of_work(Block *block, int difficulty) {
    char prefix[65] = {0};
    memset(prefix, '0', difficulty); // Cria um prefixo com 'difficulty' zeros

    printf("Iniciando prova de trabalho para o bloco %d...\n", block->index);

    do {
        block->nonce++;
        calculate_hash(block, block->hash);
    } while (strncmp(block->hash, prefix, difficulty) != 0);

    printf("Prova de trabalho conclu�da para o bloco %d! Nonce: %d\n", block->index, block->nonce);
}

// Fun��o para criar um bloco
Block *create_block(int index, const char *previous_hash, const char *data, int difficulty) {
    Block *block = (Block *)malloc(sizeof(Block));
    block->index = index;
    strncpy(block->previous_hash, previous_hash, 65);
    strncpy(block->data, data, 256);
    block->nonce = 0;

    // Prova de trabalho para encontrar o hash v�lido
    proof_of_work(block, difficulty);

    block->next = NULL;
    return block;
}

// Fun��o para criar o bloco g�nesis
Block *create_genesis_block(int difficulty) {
    printf("Criando bloco g�nesis...\n");
    return create_block(0, "0", "Genesis Block", difficulty);
}

// Fun��o para adicionar um bloco � cadeia
void add_block(Block **blockchain, const char *data, int difficulty) {
    Block *last_block = *blockchain;

    // Percorre a cadeia at� o �ltimo bloco
    while (last_block->next != NULL) {
        last_block = last_block->next;
    }

    // Cria um novo bloco e adiciona � cadeia
    Block *new_block = create_block(last_block->index + 1, last_block->hash, data, difficulty);
    last_block->next = new_block;
}

// Fun��o para imprimir toda a cadeia
void print_blockchain(Block *blockchain) {
    Block *current = blockchain;

    while (current != NULL) {
        printf("Bloco %d\n", current->index);
        printf("Hash anterior: %s\n", current->previous_hash);
        printf("Dados: %s\n", current->data);
        printf("Hash: %s\n", current->hash);
        printf("Nonce: %d\n\n", current->nonce);
        current = current->next;
    }
}

// Fun��o principal
int main() {
    int difficulty = 4; // N�mero de zeros iniciais no hash

    // Cria o bloco g�nesis
    Block *blockchain = create_genesis_block(difficulty);

    // Adiciona blocos � cadeia
    add_block(&blockchain, "Primeiro bloco de dados", difficulty);
    add_block(&blockchain, "Segundo bloco de dados", difficulty);

    // Imprime a cadeia de blocos
    printf("Imprimindo a blockchain completa:\n");
    print_blockchain(blockchain);

    // Libera��o de mem�ria
    Block *current = blockchain;
    while (current != NULL) {
        Block *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}

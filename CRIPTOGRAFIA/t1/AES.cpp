#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include <iostream>

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext) 
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // cria e inicializa o contexto
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // inicializa a operacao de encriptacao
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    // pega mensagem a ser criptografada e obtem o output. Pode ser chamada multiplas vezes.
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext+len, &len))
        handleErrors();
    ciphertext_len += len;

    // finaliza
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    // cria e inicializa o contexto
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // inicializa a descriptografia
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    // recebe a mensagem criptografada, e obtem o output
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    // finaliza a decriptacao
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    // limpa
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void) 
{
    // chave exemplo de 256 bits
    unsigned char *key = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                           0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                           0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
                           0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
                         };

    // IV (vetor de inicialização) exemplo de 128 bits
    unsigned char *iv = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                          0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
                        };

    // mensagem a ser encriptada
    unsigned char *plaintext = 
                        (unsigned char *)"The quick brown fox jumps over the lazy dog";

    /* Buffer para texto cifrado. Deve ser grande suficiente para que o texto cifrado seja
    maior doque o plaintext, dependendo do algoritmo e do modo. */
    unsigned char ciphertext[128];

    unsigned char decryptedtext_len, ciphertext_len;

    // Criptografa o plaintext
    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);

    std::cout << "Texto original:\n" << plaintext << std::endl;
    std::cout << "Texto encriptado:\n" << ciphertext << std::endl;

    // Descriptografa o ciphertext
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);

    // Adiciona terminador nulo de string, espera-se texto printavel
    decryptedtext[decryptedtext_len] = '\0';

    std::cout << "Texto decriptado:\n" << decryptedtext << std::endl;

    return 0;
}
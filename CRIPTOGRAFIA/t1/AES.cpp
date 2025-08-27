#include <openssl/evp.h>
#include <openssl/err.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>

typedef unsigned char byte;

static void handle_errors() {
    ERR_print_errors_fp(stderr);
    std::abort();
}

void aes256_encrypt(const unsigned char key[32], const unsigned char iv[16],
                    const std::vector<unsigned char> &plaintext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_errors();
    // inicializa encrypt
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
        handle_errors();

    // Espaço suficiente: tamanho do plaintext + tamanho do bloco para o padding
    int block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    std::vector<unsigned char> ciphertext(plaintext.size() + block_size);

    int out_len1 = 0;
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1, plaintext.data(), static_cast<int>(plaintext.size())) != 1)
        handle_errors();

    int out_len2 = 0;
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2) != 1)
        handle_errors();

    ciphertext.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

void aes256_decrypt(const unsigned char key[32], const unsigned char iv[16],
                    const std::vector<unsigned char> &plaintext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_errors();
    // inicializa decrypt
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
        handle_errors();

    // Tamanho do plaintext será <= tamanho do ciphertext
    std::vector<unsigned char> plaintext(ciphertext.size());

    int out_len1 = 0;
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &out_len1, ciphertext.data(), static_cast<int>(ciphertext.size())) != 1)
        handle_errors();

    int out_len2 = 0;
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + out_len1, &out_len2) != 1)
        handle_errors();

    plaintext.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

int main()
{
    // chave de 256 bits
    const unsigned char key[32] = { 
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
        0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
    };

    // IV(vetor de inicialização) de 128 bits
    const unsigned char iv[16] = { 
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
    };

    // mensagem a ser encrypted
    std::string plaintext = "The quick brown fox jumps over the lazy dog";
    
}
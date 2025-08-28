#include <openssl/evp.h>
#include <openssl/err.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <memory>
#include <stdexcept>

typedef unsigned char byte;

using EVP_CIPHER_CTX_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

void aes256_encrypt(const byte key[32], 
                const byte iv[16], 
                const std::vector<unsigned char> &plaintext, 
                std::vector<unsigned char> &ciphertext)
{
    // inicializacoes de contexto e init
    EVP_CIPHER_CTX_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    if (!ctx)
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptInit_ex failed");

    // pre-dimensiona plaintext +1 bloco para paddings
    ciphertext.resize(plaintext.size() + 16); // 16 eh o tamanho do bloco AES
    
    int out_len1 = 0;
    const byte *in_ptr = plaintext.empty() ? nullptr : (const byte *)&plaintext[0];

    rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ciphertext[0], &out_len1, 
                            in_ptr, (int)plaintext.size());
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptUpdate failed");
   
    int out_len2 = (int)ciphertext.size() - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ciphertext[0]+out_len1, &out_len2);
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptFinal_ex failed");

    // ajusta tamanho final do texto cifrado
    ciphertext.resize(out_len1 + out_len2);
}

void aes256_decrypt(const byte key[32], 
                const byte iv[16], 
                const std::vector<unsigned char> &ciphertext, 
                std::vector<unsigned char> &decryptedtext)
{
    // inicializacoes de contexto e init
    EVP_CIPHER_CTX_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    if (!ctx)
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptInit_ex failed");

    // decryptedtext <= ciphertext (padding sera removido no final)
    decryptedtext.resize(ciphertext.size());

    int out_len1 = 0;
    const byte* in_ptr = ciphertext.empty() ? nullptr : (const byte*)&ciphertext[0];

    rc = EVP_DecryptUpdate(ctx.get(),
                           (byte*)&decryptedtext[0], &out_len1,
                           in_ptr, (int)ciphertext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptUpdate failed");

    int out_len2 = (int)decryptedtext.size() - out_len1;
    rc = EVP_DecryptFinal_ex(ctx.get(),
                             (byte*)&decryptedtext[0] + out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptFinal_ex failed");

    // ajusta tamanho final do texto plano (pos-remocao do padding)
    decryptedtext.resize(out_len1 + out_len2);
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
    std::string msg = "The quick brown fox jumps over the lazy dog";
    std::vector<unsigned char> plaintext(msg.begin(), msg.end());

    std::vector<unsigned char> ciphertext;
    std::vector<unsigned char> decryptedtext;

    // criptografa
    aes256_encrypt(key, iv, plaintext, ciphertext);

    // descriptografa
    aes256_decrypt(key, iv, ciphertext, decryptedtext);

    std::cout << "Original: " << msg << "\n";

    std::cout << "Cifrado (hex): ";
    for (unsigned char c : ciphertext)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    td::cout << std::dec << "\n";

    std::string recovered(decryptedtext.begin(), decryptedtext.end());
    std::cout << "Decifrado: " << recovered << "\n";
    
    return 0;
}
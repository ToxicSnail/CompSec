#include "Aes256.h"
#include <openssl/aes.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

bool Aes256::encryptFile(const std::string& inFile, const std::string& outFile, const std::string& keyStr) 
{
    std::ifstream in(inFile, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), {});
    int pad = AES_BLOCK_SIZE - data.size() % AES_BLOCK_SIZE;
    data.insert(data.end(), pad, pad);

    unsigned char key[32] = {0};
    std::memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));

    AES_KEY aesKey;
    AES_set_encrypt_key(key, 256, &aesKey);

    unsigned char iv[AES_BLOCK_SIZE] = {0};
    std::vector<unsigned char> out(data.size());
    AES_cbc_encrypt(data.data(), out.data(), data.size(), &aesKey, iv, AES_ENCRYPT);

    std::ofstream ofs(outFile, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(out.data()), out.size());
    return true;
}

bool Aes256::decryptFile(const std::string& inFile, const std::string& outFile, const std::string& keyStr) 
{
    std::ifstream in(inFile, std::ios::binary);
    std::vector<unsigned char> enc((std::istreambuf_iterator<char>(in)), {});
    if (enc.size() % AES_BLOCK_SIZE) return false;

    unsigned char key[32] = {0};
    std::memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));

    AES_KEY aesKey;
    AES_set_decrypt_key(key, 256, &aesKey);

    unsigned char iv[AES_BLOCK_SIZE] = {0};
    std::vector<unsigned char> out(enc.size());
    AES_cbc_encrypt(enc.data(), out.data(), enc.size(), &aesKey, iv, AES_DECRYPT);

    int pad = out.back();
    out.resize(out.size() - pad);

    std::ofstream ofs(outFile, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(out.data()), out.size());
    return true;
}
#ifndef AES256_H
#define AES256_H
#include <string>

class Aes256 {
    public:
        static bool encryptFile(const std::string& inFile, const std::string& outFile, const std::string& key);
        static bool decryptFile(const std::string& inFile, const std::string& outFile, const std::string& key);
};
#endif // AES256_H
#ifndef ENCRYPTOR_H 
#define ENCRYPTOR_H

#include <string>

class Encryptor { 
    public: 
        static Encryptor& getInstance(); 
        void setKey(const std::string& key); 
        void encrypt(const std::string& path); 
        void decrypt(const std::string& path); 
    private: 
        Encryptor() = default; 
        Encryptor(const Encryptor&) = delete; 
        Encryptor& operator=(const Encryptor&) = delete; 
        void processFile(const std::string& filePath, bool encrypt); 
        
        std::string setted_key; 
};

#endif // ENCRYPTOR_H
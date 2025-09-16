#include "Encryptor.h" 
#include "DirWalker.h" 
#include "Aes256.h" 
#include <iostream> 
#include <cstdio>

Encryptor& Encryptor::getInstance() 
{ 
    static Encryptor instance; 
    return instance; 
}

void Encryptor::setKey(const std::string& key) 
{ 
    setted_key = key; 
}

void Encryptor::encrypt(const std::string& path) 
{ 
    auto files = DirWalker::getFiles(path); 
    for (const auto& file : files) 
    { 
        processFile(file, true); 
    } 
}

void Encryptor::decrypt(const std::string& path) 
{ 
    auto files = DirWalker::getFiles(path); 
    for (const auto& file : files) 
    { 
        processFile(file, false); 
    } 
}

void Encryptor::processFile(const std::string& filePath, bool encrypt) 
{ 
    std::string tempPath = filePath + ".tmp"; 
    bool ok; 
    
    if (encrypt) 
    { 
        std::cout << "Encrypting: " << filePath << std::endl; 
        ok = Aes256::encryptFile(filePath, tempPath, setted_key); 
    } 
    else 
    { 
        std::cout << "Decrypting: " << filePath << std::endl; 
        ok = Aes256::decryptFile(filePath, tempPath, setted_key); 
    } 

    if (ok) 
    { 
        std::remove(filePath.c_str()); 
        std::rename(tempPath.c_str(), filePath.c_str()); 
    } 
    else 
    { 
        std::cerr << (encrypt ? "Encryption failed: " : "Decryption failed: ") << filePath << std::endl; 
        std::remove(tempPath.c_str()); 
    } 
}

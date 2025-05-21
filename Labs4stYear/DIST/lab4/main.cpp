#include "Encryptor.h" 
#include <openssl/evp.h> 
#include <openssl/err.h> 
#include <iostream> 
#include <string>

void initCrypto()  //openssl constuctor
{ 
    OpenSSL_add_all_algorithms(); 
}

void cleanupCrypto() //destructor openssl
{ 
    EVP_cleanup(); 
}

int main(int argc, char* argv[]) 
{ 
    if (argc != 4) 
    { 
        std::cerr << "Usage: " << argv[0] << " [encrypt|decrypt] <folder> <key>" << std::endl; 
        return 1; 
    }

    std::string mode = argv[1];
    std::string folder = argv[2];
    std::string key = argv[3];

    initCrypto();

    Encryptor& enc = Encryptor::getInstance();  //singleton
    enc.setKey(key);

    if (mode == "encrypt") 
    {
        enc.encrypt(folder);
    } 
    else if (mode == "decrypt") 
    {
        enc.decrypt(folder);
    } 
    else 
    {
        std::cerr << "Unknown mode: " << mode << std::endl;
        cleanupCrypto();
        return 1;
    }

    cleanupCrypto();
    return 0;

}
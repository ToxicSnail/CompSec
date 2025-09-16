#ifndef DIRWALKER_H 
#define DIRWALKER_H

#include <string> 
#include <vector>

class DirWalker { 
    public: 
        static std::vector<std::string> getFiles(const std::string& path); 
    private: 
        static void recurse(const std::string& path, std::vector<std::string>& files); 
};

#endif // DIRWALKER_H
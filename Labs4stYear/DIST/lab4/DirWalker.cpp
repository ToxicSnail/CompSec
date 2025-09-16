#include "DirWalker.h" 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h> 
#include <vector> 
#include <string> 
#include <unistd.h>

void DirWalker::recurse(const std::string& path, std::vector<std::string>& files) 
{ 
    DIR* dir = opendir(path.c_str()); 
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) 
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..") continue;

        std::string fullPath = path + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == -1) continue;

        if (S_ISDIR(st.st_mode))    // опред директ?
        {
            recurse(fullPath, files);
        } else if (S_ISREG(st.st_mode))     //опред файл?
        {
            files.push_back(fullPath);
        }
    }
    closedir(dir);

}

std::vector<std::string> DirWalker::getFiles(const std::string& path) 
{ 
    std::vector<std::string> files; 
    recurse(path, files); 
    return files; 
}
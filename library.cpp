#include "library.h"
#include <iostream>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;





void Finder::process(int i){

    if(found){
        return;
    }
        try {
            for (const auto &entry: fs::recursive_directory_iterator(subdirs[i])) {
                if(found){
                    return;
                }
                std::string t = entry.path().filename().string();
                if (t == to_find) {
                    found = true;
                    std::cout << entry.path() << std::endl;
                    return;
                }
            }
        }
        catch (...){
            //std::cout << "exception occured on " << subdirs[i] << std::endl;
        }
}

void Finder::divide(const std::string& path){
    for (const auto & entry : fs::directory_iterator(path)){
        if(is_directory(entry.path()))
            subdirs.push_back(entry.path().string());

        if(entry.path().filename() == to_find){
            found = true;
            std::cout << entry.path();
            return;
        }
    }
}



void Finder::FindFile(const std::string &path) {
    divide(path);
    if(found)
        return;

    int amount = 8;
    if(subdirs.size() <= 8)
        amount = subdirs.size();

    thread_pool pool(amount);

    for(int i = 0; i < subdirs.size(); ++i)
        pool.push(std::function<void(int)>(&Finder::process,  i, this));

    pool.run();

    if(!found)
        std::cout << "File doesn't exist\n";

}

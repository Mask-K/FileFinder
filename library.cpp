#include "library.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;





void Finder::process(int i){

    if(found){
        return;
    }

        for (; i < subdirs.size(); i += 8) {
            try {
                for (const auto &entry: fs::recursive_directory_iterator(subdirs[i])) {
                    if(found)
                        return;
                    std::string t = entry.path().filename().string();
                    //std::cout << entry.path() << std::endl;
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
    int amount = subdirs.size() <= 8 ? subdirs.size() : 8;
    for(int i = 0; i < amount; ++i)
        threads.emplace_back([this, i](){ process(i);});

    for(auto& el : threads)
        el.join();

    if(!found)
        std::cout << "File doesn't exist\n";

}

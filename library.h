
#pragma once
#include "Core.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "ThreadPool/ThreadPool.h"


class Finder{
public:
    explicit Finder(std::string  file): to_find(std::move(file)){}


    void FindFile(const std::string& path);


private:
    void divide(const std::string& path);
    void process(int i);

    std::vector<std::string> subdirs;
    std::vector<std::thread> threads;
    std::atomic<bool> found = false;
    std::string to_find;
    std::mutex found_mutex;
};


#include "../library.h"
#include <iostream>

std::mutex cout_m;

void write(){

    std::lock_guard<std::mutex> lock(cout_m);
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main()
{

//    std::string path = "C:\\";
//    Finder f("test.rar");
    thread_pool tr;
    for(int i = 0; i < 128; ++i)
        tr.push(std::function<void(void)> (write));
    tr.run();
    //f.FindFile(path);

}





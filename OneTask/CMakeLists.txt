
add_executable(OneTask main.cpp)

include_directories(../FileFinder)

include_directories(../ThreadPool)

target_link_libraries(OneTask FileFinder ThreadPool)


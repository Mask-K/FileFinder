
add_executable(OneTask main.cpp)

include_directories(../FileFinder)

target_link_libraries(OneTask FileFinder)


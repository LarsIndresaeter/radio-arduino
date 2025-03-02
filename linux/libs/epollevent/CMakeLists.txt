
include_directories(include)
add_library(epollevent STATIC epollevent.cpp)
target_include_directories(epollevent INTERFACE include)

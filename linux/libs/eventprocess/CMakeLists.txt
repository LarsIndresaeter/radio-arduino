
include_directories(include)
add_library(eventprocess STATIC eventprocess.cpp)
target_link_libraries(eventprocess epollevent)
target_include_directories(eventprocess INTERFACE include)

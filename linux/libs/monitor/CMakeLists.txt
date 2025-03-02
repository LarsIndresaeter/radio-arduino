
include_directories(include)
add_library(monitor STATIC monitor.cpp)
target_link_libraries(monitor uart protocol eventprocess)
target_include_directories(monitor INTERFACE include)

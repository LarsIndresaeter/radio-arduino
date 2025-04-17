
include_directories(include)
add_library(radiosession STATIC radioSession.cpp)
target_link_libraries(radiosession monitor eventprocess)
target_include_directories(radiosession INTERFACE include)

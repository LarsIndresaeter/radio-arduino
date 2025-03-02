include_directories(include)
add_library(commands INTERFACE)
target_link_libraries(commands INTERFACE protocol)
target_include_directories(commands INTERFACE include)

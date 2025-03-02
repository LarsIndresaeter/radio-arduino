include_directories(include)
add_library(parser-base INTERFACE)
target_link_libraries(parser-base INTERFACE protocol)
target_include_directories(parser-base INTERFACE include)

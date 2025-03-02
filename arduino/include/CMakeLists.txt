add_library(version INTERFACE)
target_include_directories(version INTERFACE ${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

configure_file(version.h.in version.h)

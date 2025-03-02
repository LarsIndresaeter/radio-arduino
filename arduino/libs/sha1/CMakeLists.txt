include_directories(include)

add_library(sha1
    sha1.cpp
    )

target_link_libraries(sha1)

target_include_directories(sha1 INTERFACE include)

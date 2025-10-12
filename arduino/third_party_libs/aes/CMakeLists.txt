include_directories(include)

add_library(aes
    aes.cpp
    )

target_include_directories(aes INTERFACE include)

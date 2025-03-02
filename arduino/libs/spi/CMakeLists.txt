include_directories(include)

add_library(spi
    spi.cpp
    )

target_include_directories(spi INTERFACE include)

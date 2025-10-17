include_directories(include)

add_library(spi
    spi.cpp
    command-handlers-spi.cpp
    )

target_link_libraries(spi protocol)

target_include_directories(spi INTERFACE include)

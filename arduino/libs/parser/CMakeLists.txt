include_directories(include)

add_library(parser
    parser.cpp
    )

target_link_libraries(parser aes nrf24l01 random sleep protocol radio-link)

target_include_directories(parser PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(parser INTERFACE include)

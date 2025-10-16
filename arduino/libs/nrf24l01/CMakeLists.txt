include_directories(include)

add_library(nrf24l01
    nrf24l01.cpp
    )

target_link_libraries(nrf24l01 com-bus spi)

target_include_directories(nrf24l01 PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(nrf24l01 INTERFACE include)

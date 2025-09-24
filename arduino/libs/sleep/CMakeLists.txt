include_directories(include)

add_library(sleep
    sleep.cpp
    )

target_link_libraries(sleep nrf24l01)

target_include_directories(sleep PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(sleep INTERFACE include)

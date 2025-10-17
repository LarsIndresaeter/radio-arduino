include_directories(include)

add_library(sleep
    sleep.cpp
    command-handlers-sleep.cpp
    )

target_link_libraries(sleep nrf24l01 radio-link quadencoder protocol)

target_include_directories(sleep PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(sleep INTERFACE include)

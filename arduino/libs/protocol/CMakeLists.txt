include_directories(include)

add_library(protocol
    arduinoCryptoHandler.cpp
    protocol.cpp
    )

target_link_libraries(protocol uart adc sha1 aes)

target_include_directories(protocol PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(protocol INTERFACE include)

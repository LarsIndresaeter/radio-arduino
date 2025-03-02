include_directories(include)

add_library(protocol 
    STATIC 
    linuxCryptoHandler.cpp
    protocol.cpp
    )

target_link_libraries(protocol epollevent ${CONAN_LIBS})

target_include_directories(protocol PUBLIC ${CONAN_INCLUDE_DIRS_UART-API})
target_include_directories(protocol INTERFACE include)

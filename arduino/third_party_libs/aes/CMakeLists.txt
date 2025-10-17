include_directories(include)

add_library(aes
    aes.cpp
    command-handlers-aes.cpp
    )

target_link_libraries(aes eepromlib protocol)

target_include_directories(aes INTERFACE include)

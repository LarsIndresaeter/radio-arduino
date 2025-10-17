include_directories(include)

add_library(ds18b20
    ds18b20.c
    onewire.c
    romsearch.c
    command-handlers-ds18b20.cpp
    )

target_link_libraries(ds18b20 protocol)

target_include_directories(ds18b20 PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(ds18b20 INTERFACE include)

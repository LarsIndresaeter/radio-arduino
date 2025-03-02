include_directories(include)

add_library(ds18b20
    ds18b20.c
    onewire.c
    romsearch.c
    )

target_include_directories(ds18b20 PUBLIC ${CONAN_INCLUDE_DIRS_UART-API})
target_include_directories(ds18b20 INTERFACE include)

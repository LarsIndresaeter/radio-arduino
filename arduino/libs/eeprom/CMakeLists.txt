include_directories(include)

add_library(eepromlib
    eeprom.cpp
    )

target_include_directories(eepromlib INTERFACE include)

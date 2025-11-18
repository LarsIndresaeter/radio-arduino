include_directories(include)

add_library(eepromlib
    eeprom.cpp
    command-handlers-eeprom.cpp
    )

target_link_libraries(eepromlib protocol parser)

target_include_directories(eepromlib INTERFACE include)

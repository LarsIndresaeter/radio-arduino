include_directories(include)

add_library(adc
    adc.cpp
    command-handlers-adc.cpp
    )

target_link_libraries(adc protocol)

target_include_directories(adc INTERFACE include)

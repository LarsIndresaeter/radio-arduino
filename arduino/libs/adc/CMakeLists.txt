include_directories(include)

add_library(adc
    adc.cpp
    )

target_include_directories(adc INTERFACE include)

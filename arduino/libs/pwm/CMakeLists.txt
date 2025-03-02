include_directories(include)

add_library(pwm
    pwm.cpp
    )

target_include_directories(pwm INTERFACE include)

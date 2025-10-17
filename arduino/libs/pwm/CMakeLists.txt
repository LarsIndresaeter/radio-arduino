include_directories(include)

add_library(pwm
    pwm.cpp
    command-handlers-pwm.cpp
    )

target_link_libraries(pwm protocol)

target_include_directories(pwm INTERFACE include)

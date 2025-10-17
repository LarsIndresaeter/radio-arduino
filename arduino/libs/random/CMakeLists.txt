include_directories(include)

add_library(random
    random.cpp
    command-handlers-random.cpp
    )

target_link_libraries(random sha1 adc protocol)

target_include_directories(random INTERFACE include)

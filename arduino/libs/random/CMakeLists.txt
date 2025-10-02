include_directories(include)

add_library(random
    random.cpp
    )

target_link_libraries(random sha1 adc)

target_include_directories(random INTERFACE include)

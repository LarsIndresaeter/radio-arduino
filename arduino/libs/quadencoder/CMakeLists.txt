include_directories(include)

add_library(quadencoder
    quadencoder.cpp
    command-handlers-quadencoder.cpp
    )

target_link_libraries(quadencoder protocol)

target_include_directories(quadencoder INTERFACE include)

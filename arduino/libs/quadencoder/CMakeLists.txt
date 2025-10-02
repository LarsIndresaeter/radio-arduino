include_directories(include)

add_library(quadencoder
    quadencoder.cpp
    )

target_include_directories(quadencoder INTERFACE include)

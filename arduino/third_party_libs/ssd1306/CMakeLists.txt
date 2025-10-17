include_directories(include)

add_library(ssd1306
    Framebuffer.cpp
    SSD1306.cpp
    command-handlers-ssd1306.cpp
    )

target_link_libraries(ssd1306 i2c protocol)

target_include_directories(ssd1306 INTERFACE include)

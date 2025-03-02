
#if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    #INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    #CONAN_BASIC_SETUP()
#endif()

include_directories(include)
add_library(crypto OBJECT crypto.cpp)
target_link_libraries(crypto ${CONAN_LIBS})
target_include_directories(crypto INTERFACE include)

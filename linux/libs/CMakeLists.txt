
#if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    #INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    #CONAN_BASIC_SETUP()
#endif()

#add_subdirectory(crypto)
add_subdirectory(epollevent)
add_subdirectory(eventprocess)
add_subdirectory(uart)
add_subdirectory(protocol)
add_subdirectory(monitor)
add_subdirectory(radiosession)

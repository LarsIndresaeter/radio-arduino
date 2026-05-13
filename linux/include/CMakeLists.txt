add_library(version INTERFACE)
target_include_directories(version INTERFACE ${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

configure_file(version.h.in version.h)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../schema/adapter.schema.json" ADAPTER_JSON_SCHEMA)
configure_file(json_schema.h.in json_schema.h)


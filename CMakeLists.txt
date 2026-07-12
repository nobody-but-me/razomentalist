
cmake_minimum_required(VERSION 3.21)
project(razomentalist C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(razomentalist)

target_sources(razomentalist PRIVATE ${CMAKE_SOURCE_DIR}/main.c)

target_compile_options(razomentalist PRIVATE -Wall -Werror -pedantic -Wno-format -Wno-unused-function)

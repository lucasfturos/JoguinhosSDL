# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

set(PROJECT_NAME
    Cobrinha
    LANGUAGES C
    VERSION 1.0.0
)

project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(RESOURCES_DIRECTORY 
    "${CMAKE_SOURCE_DIR}/font"
)

file(COPY ${RESOURCES_DIRECTORY} 
    DESTINATION ${CMAKE_BINARY_DIR}
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g
)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME}
    # Math
    PRIVATE m
    # SDL2
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_ttf::SDL2_ttf
)

# -------------------------------------------------------------------
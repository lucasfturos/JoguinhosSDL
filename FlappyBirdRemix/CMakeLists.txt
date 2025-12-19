# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

# -------------------------------------------------------------------

set(PROJECT_NAME FlappyBirdRemix)

project(${PROJECT_NAME} LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RESOURCES_DIRECTORY
    ${CMAKE_SOURCE_DIR}/res
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g
)

add_executable(${PROJECT_NAME}
    src/main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "index"
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sNO_EXIT_RUNTIME=0
        -sASSERTIONS=1
        --preload-file ${RESOURCES_DIRECTORY}@/font
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        m
    )

else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        m
    )

    file(COPY ${RESOURCES_DIRECTORY}
        DESTINATION ${CMAKE_BINARY_DIR}
    )

endif()

# -------------------------------------------------------------------

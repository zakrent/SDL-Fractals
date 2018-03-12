project(SDL-Fractals)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(./lib)


file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
        "src/*.h"
        )

add_executable(SDL-Fractals ${SOURCE_FILES})

target_link_libraries(SDL-Fractals SDL2-static SDL2main)

include_directories(${SDL2_SOURCE_DIR}/include)


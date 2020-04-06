cmake_minimum_required(VERSION 3.5)
project(cg2game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x64)



file(GLOB cg2game_SRC
    "../source/*.h"
    "../source/*.cpp"
)

add_executable(cg2game ${cg2game_SRC})

target_compile_options(cg2game PRIVATE -Wnarrowing -g)
target_link_libraries(cg2game sfml-graphics sfml-system sfml-window -lX11)
set_target_properties(cg2game
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../build/linux"
)

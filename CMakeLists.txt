cmake_minimum_required(VERSION 3.24)
project(TetrisGame)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_library(MapLib STATIC
        sources/Map.cpp)

add_library(TetrominoLib STATIC
        sources/Tetromino.cpp)

add_library(CommanderLib STATIC 
        sources/Commander.cpp)

add_library(GameInfoLib STATIC 
        sources/GameInfo.cpp)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SFML_INCLUDE_DIRS})

add_executable(TetrisGame sources/main.cpp)

target_link_libraries(MapLib PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(TetrominoLib PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(CommanderLib PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(GameInfoLib PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

target_link_libraries(TetrisGame PUBLIC MapLib TetrominoLib CommanderLib GameInfoLib)
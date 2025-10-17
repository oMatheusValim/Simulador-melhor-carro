cmake_minimum_required(VERSION 3.10)
project(BEST-CAR-SIMULATAR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

add_subdirectory(vendor/imgui)
file(GLOB SOURCES "src/*.cpp")
add_executable(BEST-CAR-SIMULATOR ${SOURCES})

target_link_libraries(BEST-CAR-SIMULATOR
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    imgui
)

#rm -rf build
#cmake -B build -S . -DCMAKE_PREFIX_PATH=~/Downloads/manter/SFML-2.6.1
#cmake --build build
# ./build/SIMULADOR-MELHOR-CARRO
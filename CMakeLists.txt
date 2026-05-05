cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "tp2-asteroids")
project(${PROJECT_NAME})

find_package(SDL2 2.32.2 EXACT REQUIRED)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
set(pybind11_DIR "C:/Users/joaovecruz/Desktop/TCC/.venv/Lib/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 CONFIG REQUIRED)

set(GAME_SOURCES
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponent.cpp
        Source/Components/RigidBodyComponent.cpp
        Source/Actors/Ship.cpp
        Source/Actors/Asteroid.cpp
        Source/Components/CircleColliderComponent.cpp
        Source/Actors/Laser.cpp
        Source/Actors/Particle.cpp
)

pybind11_add_module(asteroids_cpp
        Source/Bindings.cpp
        ${GAME_SOURCES}
)

target_include_directories(asteroids_cpp PRIVATE ${pybind11_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(asteroids_cpp PRIVATE pybind11::module ${SDL2_LIBRARIES})
set_target_properties(asteroids_cpp PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/IA"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/IA"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/IA"
)

add_executable(${PROJECT_NAME}
        Source/Main.cpp
        ${GAME_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_options(asteroids_cpp PRIVATE -static)

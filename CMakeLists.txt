cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "tp2-asteroids")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponent.cpp
        Source/Components/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Actors/Ship.cpp
        Source/Actors/Ship.h
        Source/Actors/Asteroid.cpp
        Source/Actors/Asteroid.h
        Source/Components/CircleColliderComponent.cpp
        Source/Components/CircleColliderComponent.h
        Source/Actors/Laser.cpp
        Source/Actors/Laser.h)

# 1. Alterar <SDL_PATH> para o caminho do binário da SDL no seu computador
target_link_libraries(${PROJECT_NAME} "/Library/Frameworks/SDL2.framework/SDL2")
target_include_directories(${PROJECT_NAME} PRIVATE "/Library/Frameworks/SDL2.framework/Headers")
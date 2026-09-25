from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

sources = [
    "Source/Bindings.cpp",
    "Source/Math.cpp",
    "Source/Random.cpp",
    "Source/Actors/Actor.cpp",
    "Source/Components/Component.cpp",
    "Source/Game.cpp",
    "Source/Components/DrawComponent.cpp",
    "Source/Components/RigidBodyComponent.cpp",
    "Source/Actors/Ship.cpp",
    "Source/Actors/Asteroid.cpp",
    "Source/Components/CircleColliderComponent.cpp",
    "Source/Actors/Laser.cpp",
    "Source/Actors/Particle.cpp",
]

ext_modules = [
    Pybind11Extension(
        "asteroids_cpp",
        sources,
        include_dirs=[
            "Source",
            "/usr/include/SDL2",
        ],
        libraries=["SDL2"],
        cxx_std=17,
        extra_compile_args=["-w"],
    ),
]

setup(
    name="asteroids_cpp",
    version="1.0",
    author="João",
    description="Módulo C++ do jogo Asteroids com suporte a Pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)

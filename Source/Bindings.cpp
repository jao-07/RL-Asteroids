//
// Created by joaovecruz on 23/04/2026.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(asteroids_cpp, m) {
    py::class_<Game>(m, "Game")
        .def(py::init<bool, bool, int, float, float, float, float, float, float>())

        .def("initialize", &Game::Initialize)
        .def("shutdown", &Game::Shutdown)
        .def("step", &Game::Step)
        .def("reset", &Game::Reset)

        .def_readonly("m_steps_done", &Game::mStepsDone);
}
//
// Created by joaovecruz on 23/04/2026.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(asteroids_cpp, m) {
    py::class_<Game>(m, "Game")
        .def(py::init<bool, float, float, float>())

        .def("step", &Game::Step)
        .def("reset", &Game::Reset)

        .def_readwrite("m_visualise", &Game::mVisualize)
        .def_readonly("m_steps_done", &Game::mStepsDone);
}
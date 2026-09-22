//
// Created by joaovecruz on 23/04/2026.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(asteroids_cpp, m) {
    py::class_<Game>(m, "Game")
        .def(py::init<bool, bool, int, float, float, float, float, float, float>())

        .def("initialize", &Game::Initialize)
        .def("shutdown", &Game::Shutdown)
        .def("step", &Game::Step)
        .def("reset", &Game::Reset)
        // .def("get_screen_pixels", [](Game& game) {
        //     int w = 84;
        //     int h = 84;
        //
        //     std::vector<uint8_t> pixels = game.GetImageObservation(w, h);
        //     return py::array_t<uint8_t>(
        //         {1, h, w},        // Shape: 1 Canal, 84 Altura, 84 Largura
        //         {h * w, w, 1},    // Strides de memória
        //         pixels.data()     // Ponteiro para o início dos dados
        //     );
        // })
    .def("get_screen_pixels", [](Game& game, int w = 84, int h = 84) {
        std::vector<uint8_t> pixels = game.GetImageObservation(w, h);

        return py::array_t<uint8_t>(
            {1, h, w},        // Shape dinâmico: (1, height, width)
            {h * w, w, 1},    // Strides correspondentes
            pixels.data()
        );
        }, py::arg("w") = 84, py::arg("h") = 84)

        .def_readonly("m_steps_done", &Game::mStepsDone);
}
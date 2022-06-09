#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>
#include "board.cpp"
#include "random_rules.cpp"

namespace py = pybind11;

PYBIND11_MODULE(board, m) {
    m.doc() = "Plugin to simulate board logic in LtL game";

    py::class_<BoardArgs>(m, "BoardArgs")
            .def(py::init<>())
            .def_readwrite("neighborhoodRadius", &BoardArgs::neighborhoodRadius)
            .def_readwrite("states", &BoardArgs::states)
            .def_readwrite("surviveConds", &BoardArgs::surviveConds)
            .def_readwrite("birthConds", &BoardArgs::birthConds)
            .def_readwrite("isIncludeCenter", &BoardArgs::isIncludeCenter)
            .def_readwrite("isMooreType", &BoardArgs::isMooreType);

    py::class_<Board>(m, "Board")
        .def(py::init<BoardArgs>(), py::arg("boardArgs"))
        .def(py::init<BoardArgs, const cells_t &>(), py::arg("boardArgs"), py::arg("cells"))
        .def("update", &Board::update, "Handles regular updates of the cell states, in accordance with game conditions.")
        .def("getCells", &Board::getCells,"Returns const-reference to a 2-dimensional container with all cell values.")
        .def("getSize", &Board::getSize, "Returns the size of the board.");

}

PYBIND11_MODULE(myrandoms, m){
    m.doc() = "Plugin to generate game rules randomly";
    
    m.def("random_birth", &generate_birth_survive_cond, "A function that generates list of conditions of cell birth");
    m.def("random_survive", &generate_birth_survive_cond, "A function that generates list of conditions of cell survive");
    m.def("random_range", &generate_range, "A function that generates range of neighbourhood");
    m.def("random_middle_include", &generate_middle_included, "A function that generates if middle cell is included in neighbourhood counting");
    m.def("random_states_number", &generate_number_of_states, "A function that generates number of states");
    m.def("random_neighbourhood_type", &generate_neighbourhood, "A function that generates neighbourhood type");
}

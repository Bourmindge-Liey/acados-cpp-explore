#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py=pybind11;

#include "rmpc/rmpc.h"

PYBIND11_MODULE(rmpc_bind, m) {
    py::class_<RMPC>(m, "RMPC", py::dynamic_attr())
        .def(py::init<std::array<double, NX> &>())
        .def("control", &RMPC::control)
        .def("set_reference", &RMPC::set_reference)
        .def("get_control", &RMPC::get_control);
}
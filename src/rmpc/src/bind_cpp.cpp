#include <pybind11/pybind11.h>
namespace py=pybind11;

#include "rmpc/rmpc.h"

namespace py = pybind11;

PYBIND11_MODULE(rmpc_bind, m) {
    py::class_<RMPC>(m, "RMPC")
        .def(py::init<double*>())
        .def("control", &RMPC::control);
}
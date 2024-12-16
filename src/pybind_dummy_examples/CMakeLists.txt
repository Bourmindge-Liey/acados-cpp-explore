cmake_minimum_required(VERSION 3.12)
project(pybind_dummy_examples)

# Find pybind11
find_package(ament_cmake REQUIRED)

find_package(pybind11 CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Add the pybind11 module
pybind11_add_module(pybind_dummy_examples intro.cpp)

ament_export_dependencies(${PKG_DEPENDENCIES})
ament_package()

# Install Pybind Steps


1. install the package into the 

```shell
# 1. clone the git repo
git clone https://github.com/pybind/pybind11.git

# 2. local build
mkdir build
cd build
cmake ..
make check -j 4

# 3. install to /usr/include
# This allows find_package able to find pybind11.
sudo make install
```

2. writing cmake files
* exporting lib name must stay same as the 

```cmake
find_package(pybind11 REQUIRED)
find_package(Python3.11 REQUIRED COMPONENTS Interpreters Development)

#........ later .............
set(MODULE_NAME your_module_name)
pybind11_add_module(${MODULE_NAME} src/bind_cpp.cpp)
target_link_libraries(${MODULE_NAME} PRIVATE ${PROJECT_NAME} pybind11::module)
```


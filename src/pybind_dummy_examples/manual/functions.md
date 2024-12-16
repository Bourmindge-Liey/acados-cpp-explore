# Bind functions

## 1. memory management

```cpp
/* Function declaration */
Data *get_data() { return _data; /* (pointer to a static data structure) */ }
...

/* Binding code */ // wrong!
m.def("get_data", &get_data); // <-- KABOOM, will cause crash when called from Python

// use this instead
m.def("get_data", &get_data, py::return_value_policy::reference);m.def("get_data", &get_data, py::return_value_policy::reference);
```
if return is a pointer, the memory free once in py, once in cpp, causing error.
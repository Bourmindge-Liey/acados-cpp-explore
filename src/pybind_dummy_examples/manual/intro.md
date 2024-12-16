# Pybind 11


# Examples 


<div style="display: flex;">

<div style="flex: 1; padding-right: 10px;">

## cpp class

```cpp
struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }


private:
    std::string name;
};
```

</div>



<div style="flex: 1; padding-left: 10px;">


## cpp pybind
```cpp 
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {

// class definition    
py::class_<Pet>(m, "Pet", py::dynamic_attr())   // add "py::dynamic_attr()"
    .def(py::init<const std::string &>())

    // basic functions
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName)
    .def("__repr__",[](const Pet &a) {return "<example.Pet named '" + a.name + "'>";}

    // class members
    .def_readwrite("name", &Pet::name)      // for public class members
    .def_property("name", &Pet::getName, &Pet::setName); // for private members


}
```

</div>
</div>

## python

```python
import example

p = example.Pet("Molly")
p.getName()
p.setName("Charly")

```




# Inheritance Downcasting

<div style="display: flex;">

<div style="flex: 1; padding-right: 10px;">

## Cpp classs


```cpp
struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
```
</div>



<div style="flex: 1; padding-left: 10px;">

## Cpp bind

```cpp
PYBIND11_MODULE(example, m) {

py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &>())
   .def_readwrite("name", &Pet::name);

// Method 1: template parameter:
py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
}

```
</div>
</div>

To let the pybind11 recognize inheritance, a "virtual" needs to be detected.   




# Overloaded methods

use `overload_cast<arg1, arg2>`

<div style="display: flex;">
<div style="flex: 1; padding-right: 10px;">

## argument overload

```cpp
struct Pet {
    Pet(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};

py::class_<Pet>(m, "Pet")
    .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age")
    .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set the pet's name");
```


</div>
<div style="flex: 1; padding-left: 10px;">


## specifier overload
```cpp
struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};

py::class_<Widget>(m, "Widget")
   .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
   .def("foo_const",   py::overload_cast<int, float>(&Widget::foo, py::const_));
```
the const specifier does not constify variables in python.
</div>
</div>


# Nested types



<div style="display: flex;">
<div style="flex: 1; padding-right: 10px;">

## class def

```cpp
struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age = 0;
    };

    Pet(const std::string &name, Kind type) : name(name), type(type) { }

    std::string name;
    Kind type;
    Attributes attr;
};

```


</div>
<div style="flex: 1; padding-left: 10px;">


## bind
```cpp
py::class_<Pet> pet(m, "Pet");

pet.def(py::init<const std::string &, Pet::Kind>())
    .def_readwrite("name", &Pet::name)
    .def_readwrite("type", &Pet::type)
    .def_readwrite("attr", &Pet::attr);

py::enum_<Pet::Kind>(pet, "Kind")
    .value("Dog", Pet::Kind::Dog)
    .value("Cat", Pet::Kind::Cat)
    .export_values();

py::class_<Pet::Attributes>(pet, "Attributes")
    .def(py::init<>())
    .def_readwrite("age", &Pet::Attributes::age);
```

</div>
</div>

![language](https://img.shields.io/badge/language-C%2B%2B17-red.svg)

# Introduction

This is a short library, written in C++17, for nD array expression manipulation, featuring broadcasting, view and lazy evaluation.

This is not meanted to be used in a real code: this is more like an exercice or a proof of concept.
It is an extension of the original example presented [here](https://linuxfr.org/users/serge_ss_paille/journaux/c-14-expressions-template-pour-les-nuls)
where the expression tree is constructed using lambda and where visitors (shape, evaluation,...) are used to go through this tree.
The syntax is inspired from numpy & xtensor.

A toy example:
```C++
std::vector<double> data;
auto A = array(data, 3, 3);
A << view(range(3), newaxis(), all()) + array({1.5, 2.5, 3.5}, 3);
view(A, 1) << 0;

std::cout << "A = " << A << std::endl;
std::cout << "data = ";
for (auto const& v : data)
  std::cout << v << " ";
std::cout << std::endl;
```
that outputs:
```
A = [
  [1.5 2.5 3.5]
  [0 0 0]
  [3.5 4.5 5.5]
]
data = 1.5 2.5 3.5 0 0 0 3.5 4.5 5.5
```

# Overview

This library is composed of several headers that should be included depending on you needs:

  - `core.hpp` contains the core features: constant value, array, arithmetic operators (as functions), shape and evaluation visitors, assignment, display (as functions) and some type traits.
  - `fancy_op.hpp` add syntactic sugar for arithmetic operators, assignment and display.
  - `uniform.hpp` features a uniform array of given shape.
  - `range.hpp` features integer ranges.
  - `view.hpp` features view of an expression with fixed index, new axis, dimension forward and index from an expression.

# The library components

## The core and fancy operators headers

### Constant value
The simplest expression is a constant value constructed by `cst`:
```C++
#include <EasyLazy/core.hpp>

...

auto c = cst(3);
```

Its **shape** can be obtained by calling an expression with the shape visitor:
```C++
auto s = c(visitor::shape{});
display(std::cout, s); // See the fancy_op header for streaming operator
```
that outputs `[]` as constant values are considered as 0D objects.

To access the expression **value**, you have to use the evaluation visitor:
```C++
auto v = c(visitor::evaluator{});
std::cout << v << std::endl;
```
that outputs `3`.

You can also **display** the expression using the `display` function:
```C++
display(std::cout, c);
```
that outputs `3`.

Note that display using streaming operators is available in the `fancy_op.hpp` header:
```C++
#include <EasyLazy/fancy_op.hpp>

...

std::cout << c << std::endl;
```

### Array
**nD array** can be created in different ways.

Typically, you need to pass a container and the shape of the array:
```C++
std::vector<int> data{1, 2, 3, 4, 5, 6};
auto a = array(data, 2, 3); // 2D array of shape (2, 3)
std::cout << a << std::endl;
```
that outputs
```
[
  [1 2 3]
  [4 5 6]
]
```

In this last example, the container is stored **by reference** so that modifying the array also modifies the original container:
```C++
a(visitor::evaluator{0, 2}) = 10;
std::cout << data[2] << std::endl;
```
that outputs `10`.

A container can also be passed **by value** by declaring it inline or by moving it when calling `array`:
```C++
auto a = array(std::vector<int>{1, 2, 3, 4, 5, 6}, 2, 3); // Self-containing array
```

You can also directly pass an **initializer list** and a `std::vector` will be used internally for the storage:
```C++
auto a = array({1, 2, 3, 4, 5, 6}, 2, 3); // Using an initializer list
```
If needed, the value type of the initializer list can be forced using the first template parameter of `array`:
```C++
auto a = array<int>({1, 2, 3, 4, 5, 6}, 2, 3); // Using an initializer list of int type
```

In fact, you can use **any type of container** if it has a bracket operator, like `std::array` or a pointer:
```C++
std::array<int, 6> data{1, 2, 3, 4, 5, 6};
auto a = array(data, 2, 3);
```

Moreover, if the container features a `resize` method, it can be **automatically resized** depending on the given shape:
```C++
std::vector<int> data;
auto a = array(data, 2, 3);
std::cout << "data.size() = " << data.size() << std::endl;
std::cout << "a = " << a << std::endl;
auto b = array<int>({}, 2, 3); // Empty initializer list of given type
std::cout << "b = " << b << std::endl;
```
that outputs
```
data.size() = 6
a = [
  [0 0 0]
  [0 0 0]
]
b = [
  [0 0 0]
  [0 0 0]
]
```

As already illustrated above, you can get the **shape** of an array using the corresponding visitor:
```C++
std::array<int, 6> data{1, 2, 3, 4, 5, 6};
auto a = array(data, 2, 3);
std::cout << a(visitor::shape{}) << std::endl;
```
that outputs `[2, 3]`, and you can **access an element** by passing its indexes to the `evaluator` visitor:
```C++
std::cout << a(visitor::evaluator{0, 2}) << std::end;
```
that outputs `3`.

If the array is not const (or doesn't refer to a constant storage), you can also **modify an element** through the same visitor:
```C++
a(visitor::evaluator{0, 2}) = 10;
std::cout << a << std::endl;
```
that outputs:
```
[
  [1 2 10]
  [4 5 6]
]
```


### Arithmetic and expressions
Given these two elementary bricks, you can build an expression using the base arithmetic operators `plus`, `minus`, `multiplies` and `divides`:
```C++
auto expr = plus(multiplies(cst(2), a), minus(array({0, 1, 0, 1, 0, 1}, 2, 3), cst(1.5)));
std::cout << expr << std::endl;
```
that outputs:
```
[
  [0.5 3.5 4.5]
  [7.5 8.5 11.5]
]
```

The returned object is a **lazy** representation of the expression and values are calculated only when requested for.

Using the `fancy_op.hpp` header, you can simply use the C++ arithmetic operators:
```C++
auto expr = 2*a + array({0, 1, 0, 1, 0, 1}, 2, 3) - 1.5;
```
Note that constant values are automatically passed to `cst` factory.


### Assignment
You can assign an expression to an array by using the `assign` function:
```C++
std::vector<double> result_data;
auto result = array(result_data, 2, 3);
assign(result, plus(multiplies(cst(2), a), minus(array({0, 1, 0, 1, 0, 1}, 2, 3), cst(1.5))));
```

By including the `fancy_op.hpp` header, you can instead use the `<<` operator:
```C++
std::vector<double> result_data;
array(data_result, 2, 3) << 2*a + array({0, 1, 0, 1, 0, 1}, 2, 3) - 1.5;
for (auto v : result_data)
    std::cout << v << " ";
std::cout << std::endl;
```
that outputs:
```
0.5 3.5 4.5 7.5 8.5 11.5
```


### Broadcasting
When mixing expressions with different shapes are, classical broadcasting rules are applied.
If operands havn't same dimension, the missing dimensions are completed with size 1 (like for constant value in the previous snippets).

Thus, the following code
```C++
auto expr = array({1, 2}, 2, 1) + array({0.1, 0.2, 0.3}, 1, 3) + 0.15;
std::cout << expr << std::endl;
```
outputs:
```
[
  [1.25 1.35 1.45]
  [2.25 2.35 2.45]
]
```

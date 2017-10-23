DAK - Dimensional Analysis Kit for C++11
----------------------------------------

![C++11][cxx-badge]
![Boost License][license-badge]
[![Build Status][travis-badge]][travis-url]

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-11-orange.svg
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/frickiericker/dak.svg?branch=master
[travis-url]: https://travis-ci.org/frickiericker/dak

DAK is a single-file, header-only library for compile-time dimensional analysis of scalar and
vector quantities.

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)
- [License](#license)

```c++
#include <dak.hpp>

namespace units
{
    using mass = dak::scalar<double, dak::dim::mass>;
    using time = dak::scalar<double, dak::dim::time>;
    using velocity = dak::vector<double, dak::dim::velocity, 3>;
    using acceleration = dak::vector<double, dak::dim::acceleration, 3>;
    using force = dak::vector<double, dak::dim::force, 3>;
}

int main()
{
    units::mass mass{1.23};
    units::force force{1.2, 3.4, 5.6};
    units::acceleration accel = force / mass;

    units::velocity velocity = accel * 10.0; // Compile error
    units::velocity velocity = accel * units::time{10.0}; // OK!
}
```

---

## Introduction

DAK is a dimensonal analysis library mainly targeted for physics simulations using reduced units.
It assumes no specific units of measurement, and it supports vectors and points in addition to
scalar quantities.

### No units of measurement

Unlike other libraries such as [units](https://github.com/nholthaus/units) and
[PhysUnits](https://github.com/martinmoene/PhysUnits-CT-Cpp11), DAK does not assume specific units
of measurement. It just checks for dimensional consistency. So,

- DAK has `mass` and `time`, but it does not have `kilogram` or `second`.
- DAK does not have concept of `feet` and `meter`s, so no conversion between them.

```c++
dak::scalar<double, dak::dim::mass> mass; // may be gram, kilogram or electron-mass
dak::scalar<double, dak::dim::time> time;
```

This may sound limiting, but it is actually quite useful for physics simulations using *reduced
units* where every quantities approximately take values within `[-10, 10]`.

### Supports vectors and points

DAK supports `vector`s and `point`s in addition to `scalar` quantities. Dimensional conversions on
multiplications and divisions of scalar and vector quantities are properly checked and handled.

```c++
dak::point<double, dak::dim::length, 3> position{0, 0, 0};
dak::scalar<double, dak::dim::mass> mass{1};
dak::vector<double, dak::dim::force, 3> force{1, 2, 3};
dak::scalar<double, dak::dim::time> time{0.01};
dak::vector<double, dak::dim::acceleration, 3> accel = force / mass;
position += accel * time * time / 2;
```

Points are often identified as position vectors, but these are different things and should be typed
accordingly (see [affine space][affine] and CGAL's [Point][cgalpoint] and [Vector][cgalvector]).
Using `dak::point` can reduce errors in your code.

[affine]: https://en.wikipedia.org/wiki/Affine_space
[cgalpoint]: https://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Point__3.html
[cgalvector]: https://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Vector__3.html

---

## Installation

DAK has no dependency. Just put [dak.hpp][dak.hpp] in your include directory. Change file name
and/or namespace identifier if necessary.

[dak.hpp]: https://github.com/frickiericker/dak/raw/master/dak.hpp

---

## Usage

- [Specifying dimension](#specifying-dimension)
- [Defining custom dimension types](#defining-custom-dimension-types)
- [Alias types to match your problem domain](#alias-types-to-match-your-problem-domain)
- [Converting scalars to/from numbers](#converting-scalars-tofrom-numbers)
- [Using vectors](#using-vectors)
- [Supported operations](#supported-operations)

### Specifying dimension

The `dak::scalar<T, D>` class template requires the underlying number type `T` and a dimension `D`.
`T` is typically `float` or `double` but can be any number-like type. `D` is a tag type identifying
a dimension. Here you may use the reference implementation `dak::mechanical_dimension`:

```c++
using length = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
using speed = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
```

`dak::mechanical_dimension<L, M, T>` is the dimension tag type for quantities with length dimension
`L`, mass dimension `M` and time dimension `T`. Common dimensions are aliased in `dak::dim`
namespace for convenience:

- `dak::dim::number` (L M, T) = (0, 0, 0)
- `dak::dim::length` (1, 0, 0)
- `dak::dim::mass` (0, 1, 0)
- `dak::dim::time` (0, 0, 1)
- `dak::dim::speed` (1, 0, -1)
- `dak::dim::acceleration` (1, 0, -2)
- `dak::dim::momentum` (1, 1, -1)
- `dak::dim::force` (1, 1, -2)
- `dak::dim::energy` (2, 1, -2)

### Defining custom dimension types

Dimensions are just tag types. To introduce your own dimension system, define tag types and
specialize five traits classes for the tag types (named `D`, `D1` and `D2`):

- `dak::dimension_traits<D>`
- `dak::product_dimension<D1, D2>`
- `dak::quotient_dimension<D1, D2>`
- `dak::power_dimension<D1, D2>`
- `dak::root_dimension<D1, D2>`

The following example adds a new dimension system `kinetic_dimension`, which only has the
dimensions of length and time.

```c++
template<int L, int T>
struct kinetic_dimension;

namespace dak
{
    template<int L, int T>
    struct dimension_traits<kinetic_dimension<L, T>>
    {
        static constexpr bool is_zero = (L == 0 && T == 0);
    };

    template<int L1, int T1, int L2, int T2>
    struct product_dimension<kinetic_dimension<L1, T1>, kinetic_dimension<L2, T2>>
    {
        using type = kinetic_dimension<L1 + L2, T1 + T2>;
    };

    template<int L1, int T1, int L2, int T2>
    struct quotient_dimension<kinetic_dimension<L1, T1>, kinetic_dimension<L2, T2>>
    {
        using type = kinetic_dimension<L1 - L2, T1 - T2>;
    };

    template<int L, int T, int N>
    struct power_dimension<kinetic_dimension<L, T>, N>
    {
        using type = kinetic_dimension<L * N, T * N>;
    };

    template<int L, int T, int N>
    struct root_dimension<kinetic_dimension<L, T>, N>
    {
        static_assert(L % N == 0 && T % N == 0, "fractional dimension");
        using type = kinetic_dimension<L / N, T / N>;
    };
}
```

Note: The reference dimension system `dak::mechanical_dimension` does not support fractional
dimensions. Dimension systems with support for fractional dimensions can be defined using
`std::ratio`.

### Alias types to match your problem domain

DAK does not provide predefined instantiations of `dak::scalar`, `dak::vector` or `dak::point`
class templates. This is because (1) a quantity may be scalar or vector depending on the problem
domain and (2) the spatial dimension also depends on the problem domain. So, you may want to alias
instantiations of `dak` class templates like following:

```c++
// In your code
namespace units
{
    using mass = dak::scalar<double, dak::dim::mass>;
    using time = dak::scalar<double, dak::dim::time>;
    using point = dak::point<double, dak::dim::length, 3>;
    using displacement = dak::vector<double, dak::dim::length, 3>;
    using velocity = dak::vector<double, dak::dim::speed, 3>;
    using force = dak::vector<double, dak::dim::force, 3>;
}
```

And use them like this:

```c++
int main()
{
    units::point p;
    units::point q;
    units::displacement r = p - q;
    units::velocity v = r / units::time{1.23};
}
```

### Converting scalars to/from numbers

Implicit conversions of `dak::scalar`s to/from underlying number types are disallowed:

```c++
using length = dak::scalar<double, dak::dim::length>;
length len = 10; // Compile error
len = 20; // Compile error
double val = len; // Compile error
```

Use explicit constructor and `value()` function:

```c++
length len{10};
len = length{20};
double val = len.value();
```

However, dimensionless scalars are convertible to/from underlying number types:

```c++
length x{10};
length y{20};
double ratio = x / y; // OK, dimensionless
```

### Using vectors

Use explicit constructor to assign numbers to a `dak::vector`.

```c++
using displacement = dak::vector<double, dak::dim::length, 3>;
displacement r;
r = {1, 2, 3}; // Compile error
r = displacement{1, 2, 3}; // OK
```

You can use implicit constructor if and only if the components are properly typed scalars:

```c++
using coord = dak::scalar<double, dak::dim::length>;
coord x{1};
coord y{2};
coord z{3};
r = {x, y, z}; // OK
```

### Supported operations

Given `dak::scalar x, y` and a dimensionless number `a`:

- `x == y`
- `x != y`
- `x < y`
- `x > y`
- `x <= y`
- `x >= y`
- `+x`
- `-x`
- `x + y`
- `x - y`
- `x * y`
- `x / y`
- `x * a`
- `x / a`
- `a * x`
- `a / x`
- `x += y`
- `x -= y`
- `x *= a`
- `x /= a`
- `abs(x)`
- `hypot(x, y)`
- `pow<n>(x)` where `n` is an integer
- `sqrt(x)`
- `cbrt(x)`

Given `dak::vector v, w`, `dak::scalar x` and a dimensionless number `a`:

- `v[i]` for `i = 0, 1, ..., N - 1` where `N` is the spatial dimension
- `v == w`
- `v != w`
- `+v`
- `-v`
- `v + w`
- `v - w`
- `v * a`
- `a * v`
- `v / a`
- `v * x`
- `x * v`
- `v / x`
- `v += w`
- `v -= w`
- `v *= a`
- `v /= a`
- `dot(v, w)`
- `squared_norm(v)`
- `norm(v)`
- `cross(v, w)` for three-dimensional vectors

Given `dak::point p, q` and `dak::vector v`:

- `p[i]` for `i = 0, 1, ..., N - 1` where `N` is the spatial dimension
- `p == q`
- `p != q`
- `p - q`
- `p + v`
- `p - v`
- `p += v`
- `p -= v`
- `squared_distance(p, q)`
- `distance(p, q)`

Note that `dak::point` does not support all algebraic operations. Only translations with vectors
are allowed. Operations like point + point do not make sense.

---

## Testing

Move to the repository root and type following commands to run tests:

    git submodule update --init
    mkdir build
    cd build
    cmake ..
    make
    ctest

The last command can alternatively be `tests/run_unittests`.

---

## License

Boost Software License, Version 1.0.

License text is included in the header file. So, you don't need to copy the license file
(LICENSE.txt) into your project if you just use the header file.

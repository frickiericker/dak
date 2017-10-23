DAK - Dimensional Analysis Kit
------------------------------

![C++11][cxx-badge]
![Boost License][license-badge]
[![Build Status][travis-badge]][travis-url]

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-11-orange.svg
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/frickiericker/dak.svg?branch=master
[travis-url]: https://travis-ci.org/frickiericker/dak

DAK is a single-file, header-only library for compile-time dimensional analysis of scalar and
vector quantities. It is targeted mainly for physics simulation using reduced units, so it assumes
no specific units of measurement like CGS or SI.

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

- [Installation](#installation)
- [Basic usage](#basic-usage)
- [Testing](#testing)
- [License](#license)
- [Similar projects](#similar-projectsk)

## Installation

DAK has no dependency. Just put [dak.hpp][dak.hpp] in your include directory. Change file name
and/or namespace identifier if necessary.

[dak.hpp]: https://github.com/frickiericker/dak/raw/master/dak/dak.hpp

## Basic usage

DAK provides three class templates for dimension-aware quantities:

- `dak::scalar<T, D>` for scalar quantities
- `dak::vector<T, D, N>` for N-dimensional vector quantities
- `dak::point<T, D, N>` for N-dimensional points

`T` is the underlying numeric type. It is typically `float` or `double` but can be any number-like
type. `D` is a tag type identifying dimension. Here you may use reference dimension types:

```c++
using length = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
using velocity = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
```

`dak::mechanical_dimension<L, M, T>` is a dimension tag type for quantities with length dimension
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

You can also define your own dimension system. (TODO: documentation)

## Testing

Move to the repository root and type following commands to run tests:

    git submodule update --init
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ctest

The last command can alternatively be `tests/run_unittests`.

## License

Boost Software License, Version 1.0.

License text is included in the header file. So, you don't need to copy the license file
(LICENSE.txt) into your project if you just use the header file.

---

## Similar projects

These are also header-only dimensional analysis libraries:

- [units](https://github.com/nholthaus/units)
- [PhysUnits](https://github.com/martinmoene/PhysUnits-CT-Cpp11)

Unlike DAK, they assume specific units of measurement and support no vector quantities.

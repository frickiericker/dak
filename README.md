dim - Dimensional Analysis for C++
----------------------------------

![C++11][cxx-badge]
![Boost License][license-badge]
[![Build Status][travis-badge]][travis-url]

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-11-orange.svg
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/snsinfu/dim.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/dim

dim is a single-file, header-only library for compile-time dimensional analysis of scalar and
vector quantities. It is targeted mainly for physics simulation using reduced units, so it assumes
no specific units of measurement like CGS or SI.

```c++
#include <dim.hpp>

namespace units
{
    using mass = dim::scalar<double, dim::mech::mass>;
    using time = dim::scalar<double, dim::mech::time>;
    using velocity = dim::vector<double, dim::mech::velocity, 3>;
    using acceleration = dim::vector<double, dim::mech::acceleration, 3>;
    using force = dim::vector<double, dim::mech::force, 3>;
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
- [Similar projects](#similar-projects)

## Installation

dim has no dependency. Just put [dim.hpp][dim.hpp] in your include directory. Change file name
and/or namespace identifier if necessary.

[dim.hpp]: https://raw.githubusercontent.com/snsinfu/dim/master/dim/dim.hpp

## Basic usage

dim provides three class templates for dimension-aware quantities:

- `dim::scalar<T, D>` for scalar quantities
- `dim::vector<T, D, N>` for N-dimensional vector quantities
- `dim::point<T, D, N>` for N-dimensional points

`T` is the underlying numeric type. It is typically `float` or `double` but can be any number-like
type. `D` is a tag type identifying dimension. Here you may use reference dimension types:

```c++
using length = dim::scalar<double, dim::mechanical_dimension<1, 0, 0>>;
using velocity = dim::vector<double, dim::mechanical_dimension<1, 0, -1>, 3>;
```

`dim::mechanical_dimension<L, M, T>` is a dimension tag type for quantities with length dimension
`L`, mass dimension `M` and time dimension `T`. Common dimensions are aliased in `dim::mech`
namespace for convenience:

- `dim::mech::number` (L M, T) = (0, 0, 0)
- `dim::mech::length` (1, 0, 0)
- `dim::mech::mass` (0, 1, 0)
- `dim::mech::time` (0, 0, 1)
- `dim::mech::speed` (1, 0, -1)
- `dim::mech::acceleration` (1, 0, -2)
- `dim::mech::momentum` (1, 1, -1)
- `dim::mech::force` (1, 1, -2)
- `dim::mech::energy` (2, 1, -2)

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

Unlike dim, they assume specific units of measurement and support no vector quantities.

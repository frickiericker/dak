dim - Dimensional Analysis for C++
----------------------------------

![C++11][cxx-badge]
![Boost License][license-badge]
[![Build Status][travis-badge]][travis-url]

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-11-orange.svg
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/snsinfu/dim.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/dim

`dim` is a single-file, header-only library of static dimensional analysis of
scalar and vector quantities. It is targeted mainly for physics simulation
using reduced units; so, no specific units of measurement like CGS or SI are
assumed, and no unit conversion is done.

- [Installation](#installation)
- [Example](#example)
- [Usage](#usage)
- [Testing](#testing)
- [License](#license)
- [Similar projects](#similar-projects)

## Installation

`dim` has no dependency. Just copy [dim.hpp][dim.hpp] to your include directory
and you are done! Change file name and/or namespace identifier if necessary.

[dim.hpp]: https://raw.githubusercontent.com/snsinfu/dim/master/dim/dim.hpp

## Example

```c++
#include <dim.hpp>

namespace units
{
    using mass         = dim::scalar<double, dim::mech::mass>;
    using time         = dim::scalar<double, dim::mech::time>;
    using velocity     = dim::vector<double, dim::mech::velocity, 3>;
    using acceleration = dim::vector<double, dim::mech::acceleration, 3>;
    using force        = dim::vector<double, dim::mech::force, 3>;
}

int main()
{
    units::mass mass{1.23};
    units::force force{1.2, 3.4, 5.6};
    units::acceleration accel = force / mass;

    units::velocity velocity = accel // Compile error
    units::velocity velocity = accel * units::time{1.0}; // OK!
}
```

## Usage

### Scalars

Use `dim::scalar<T, D>` template to define scalar quantities:

```c++
using length = dim::scalar<double, dim::mech::length>;
using time = dim::scalar<double, dim::mech::time>;
using speed = dim::scalar<double, dim::mech::speed>;

length r{1.0};
time t{0.1};
speed v = r / t;
```

The first parameter `T` is the underlying numeric type and the second parameter
`D` is a tag type identifying physical dimension. These predefined tags are
available:

- `dim::mech::number`
- `dim::mech::length`
- `dim::mech::mass`
- `dim::mech::time`
- `dim::mech::speed`
- `dim::mech::acceleration`
- `dim::mech::momentum`
- `dim::mech::force`
- `dim::mech::energy`

Use `dim::mechanilcal_dimension` to define a tag with arbitrary dimension:

```c++
using my_tag = dim::mechanical_dimension<L, M, T>; // Length, Mass, Time
```

### Vectors

Use `dim::vector<T, D, N>` template to define vector quantities:

```c++
using force = dim::vector<double, dim::mech::force, 3>;
using acceleration = dim::vector<double, dim::mech::acceleration, 3>;
using mass = dim::scalar<double, dim::mech::mass>;

mass m{1.0};
force f{1.0, 2.0, 3.0};
acceleration a = f / m;
```

The first parameter `T` is the underlying numeric type, the second parameter
`D` is a tag type identifying physical dimension, and the last parameter `N`
is the spatial dimension.

### Points

`dim` also provides `dim::point<T, D, N>` for representing geometrical points:

```c++
using position = dim::point<double, dim::mech::legnth, 3>;
using displacement = dim::vector<double, dim::mech::length, 3>;

position p1{1.0, 2.0, 3.0};
position p2{4.0, 5.0, 6.0};
displacement r = p1 - p2;
```

Note how different types are used to represent a position and a displacement.
It may look pedantic, but it prevents subtle bugs in complex calculations such
as molecular dynamics simulations.

## Testing

Move to the repository root and type following commands to run tests:

```console
git submodule update --init
mkdir tests/build
cd tests/build
cmake ..
cmake --build .
./run
```

## License

Boost Software License, Version 1.0.

License text is included in the header file. So, you don't need to copy the
license file (LICENSE.txt) into your project.

## Similar projects

These are also header-only dimensional analysis libraries:

- [units](https://github.com/nholthaus/units)
- [PhysUnits](https://github.com/martinmoene/PhysUnits-CT-Cpp11)

These projects focus on scalar quantities and provide implicit unit conversion.

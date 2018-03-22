#include <type_traits>

#include <dim.hpp>
#include <doctest.h>

TEST_CASE("predefined dimensions")
{
    CHECK((std::is_same<dim::mech::length, dim::mechanical_dimension<1, 0, 0>>::value));
    CHECK((std::is_same<dim::mech::mass, dim::mechanical_dimension<0, 1, 0>>::value));
    CHECK((std::is_same<dim::mech::time, dim::mechanical_dimension<0, 0, 1>>::value));
    CHECK((std::is_same<dim::mech::speed, dim::mechanical_dimension<1, 0, -1>>::value));
    CHECK((std::is_same<dim::mech::acceleration, dim::mechanical_dimension<1, 0, -2>>::value));
    CHECK((std::is_same<dim::mech::momentum, dim::mechanical_dimension<1, 1, -1>>::value));
    CHECK((std::is_same<dim::mech::force, dim::mechanical_dimension<1, 1, -2>>::value));
    CHECK((std::is_same<dim::mech::energy, dim::mechanical_dimension<2, 1, -2>>::value));
}

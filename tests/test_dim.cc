#include <type_traits>

#include <dak.hpp>
#include <doctest.h>

TEST_CASE("predefined dimensions")
{
    CHECK((std::is_same<dak::dim::length, dak::mechanical_dimension<1, 0, 0>>::value));
    CHECK((std::is_same<dak::dim::mass, dak::mechanical_dimension<0, 1, 0>>::value));
    CHECK((std::is_same<dak::dim::time, dak::mechanical_dimension<0, 0, 1>>::value));
    CHECK((std::is_same<dak::dim::speed, dak::mechanical_dimension<1, 0, -1>>::value));
    CHECK((std::is_same<dak::dim::acceleration, dak::mechanical_dimension<1, 0, -2>>::value));
    CHECK((std::is_same<dak::dim::momentum, dak::mechanical_dimension<1, 1, -1>>::value));
    CHECK((std::is_same<dak::dim::force, dak::mechanical_dimension<1, 1, -2>>::value));
    CHECK((std::is_same<dak::dim::energy, dak::mechanical_dimension<2, 1, -2>>::value));
}

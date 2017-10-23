#include <type_traits>

#include <dak.hpp>
#include <doctest.h>

TEST_CASE("scalar: is trivially copyable")
{
    using float_length_t = dak::scalar<float, dak::mechanical_dimension<1, 0, 0>>;
    using double_length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_trivially_copyable<float_length_t>::value));
    CHECK((std::is_trivially_copyable<double_length_t>::value));
}

TEST_CASE("scalar: is explicitly constructible with a value")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_constructible<number_t, double>::value));
    CHECK((std::is_constructible<length_t, double>::value));
}

TEST_CASE("scalar: is default constructed to zero")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    length_t x;
    CHECK(x == length_t{0});
}

TEST_CASE("scalar: disallows dimension-altering conversion")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;

    CHECK_FALSE((std::is_convertible<number_t, length_t>::value));
    CHECK_FALSE((std::is_convertible<length_t, number_t>::value));

    CHECK_FALSE((std::is_convertible<length_t, speed_t>::value));
    CHECK_FALSE((std::is_convertible<speed_t, length_t>::value));

    CHECK_FALSE((std::is_convertible<speed_t, number_t>::value));
    CHECK_FALSE((std::is_convertible<number_t, speed_t>::value));
}

TEST_CASE("scalar: is convertible to/from raw number iff dimension is zero")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("static checks")
    {
        CHECK((std::is_convertible<number_t, double>::value));
        CHECK((std::is_convertible<double, number_t>::value));

        CHECK_FALSE((std::is_convertible<length_t, double>::value));
        CHECK_FALSE((std::is_convertible<double, length_t>::value));
    }

    SUBCASE("double -> scalar conversion")
    {
        number_t num = 123;
        CHECK(num == number_t{123});
        num = 456;
        CHECK(num == number_t{456});
    }

    SUBCASE("scalar -> double conversion")
    {
        number_t num = 123;
        double raw = num;
        CHECK(raw == 123);
    }
}

TEST_CASE("scalar: exposes number_type")
{
    using float_length_t = dak::scalar<float, dak::mechanical_dimension<1, 0, 0>>;
    using double_length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<float_length_t::number_type, float>::value));
    CHECK((std::is_same<double_length_t::number_type, double>::value));
}

TEST_CASE("scalar: exposes dimension")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<number_t::dimension, dak::mechanical_dimension<0, 0, 0>>::value));
    CHECK((std::is_same<length_t::dimension, dak::mechanical_dimension<1, 0, 0>>::value));
}

TEST_CASE("scalar: provides raw-number getter")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const len{123};
    double raw = len.value();
    CHECK(raw == 123);
}

TEST_CASE("scalar: provides equality comparison operators")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{123};
    length_t const y{123};
    length_t const z{456};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("scalar: provides order comparison operators")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("comparing strictly different quantities")
    {
        length_t const x{123};
        length_t const y{456};

        CHECK(x < y);
        CHECK_FALSE(y < x);
        CHECK_FALSE(x > y);
        CHECK(y > x);
        CHECK(x <= y);
        CHECK_FALSE(y <= x);
        CHECK_FALSE(x >= y);
        CHECK(y >= x);
    }

    SUBCASE("comparing equal quantities")
    {
        length_t const x{123};
        length_t const y{123};

        CHECK_FALSE(x < y);
        CHECK_FALSE(y < x);
        CHECK_FALSE(x > y);
        CHECK_FALSE(y > x);
        CHECK(x <= y);
        CHECK(y <= x);
        CHECK(x >= y);
        CHECK(y >= x);
    }
}

TEST_CASE("scalar: provides += operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x += length_t{34});
    CHECK(&x == &y);
    CHECK(x == length_t{46});
}

TEST_CASE("scalar: provides -= operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x -= length_t{34});
    CHECK(&x == &y);
    CHECK(x == length_t{-22});
}

TEST_CASE("scalar: provides *= operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x *= 2);
    CHECK(&x == &y);
    CHECK(x == length_t{24});
}

TEST_CASE("scalar: provides /= operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x /= 2);
    CHECK(&x == &y);
    CHECK(x == length_t{6});
}

TEST_CASE("scalar: provides unary + operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const& y = +x;
    CHECK(&x != &y);
    CHECK(y == length_t{12});
}

TEST_CASE("scalar: provides unary - operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y = -x;
    CHECK(y == length_t{-12});
}

TEST_CASE("scalar: provides binary + operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y{34};
    length_t const z = x + y;
    CHECK(z == length_t{46});
}

TEST_CASE("scalar: provides binary - operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y{34};
    length_t const z = x - y;
    CHECK(z == length_t{-22});
}

TEST_CASE("scalar: provides binary * operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    CHECK((2 * x) == length_t{24});
    CHECK((x * 3) == length_t{36});
}

TEST_CASE("scalar: provides binary / operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using wavenumber_t = dak::scalar<double, dak::mechanical_dimension<-1, 0, 0>>;

    length_t const x{12};
    CHECK((x / 3) == length_t{4});
    CHECK((36 / x) == wavenumber_t{3});
}

TEST_CASE("scalar: provides binary * operator for dimensional conversion")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;

    length_t const x{12};
    frequency_t const y{3};
    CHECK((x * y) == speed_t{36});
    CHECK((y * x) == speed_t{36});
}

TEST_CASE("scalar: provides binary / operator for dimensional conversion")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;

    speed_t const x{12};
    length_t const y{3};
    CHECK((x / y) == frequency_t{4});
    CHECK((y / x) == duration_t{3.0 / 12});
}

TEST_CASE("scalar: provides abs function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x = dak::abs(length_t{12});
    length_t const y = dak::abs(length_t{-34});
    CHECK(x == length_t{12});
    CHECK(y == length_t{34});
}

TEST_CASE("scalar: provides hypot function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x = dak::hypot(length_t{3}, length_t{4});
    CHECK(x == length_t{5});
}

TEST_CASE("scalar: provides pow function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;
    using volume_t = dak::scalar<double, dak::mechanical_dimension<3, 0, 0>>;
    using density_t = dak::scalar<double, dak::mechanical_dimension<-3, 0, 0>>;

    length_t const x{4};
    CHECK(dak::pow<2>(x) == area_t{16});
    CHECK(dak::pow<3>(x) == volume_t{64});
    CHECK(dak::pow<-3>(x) == density_t{1.0 / 64});
}

TEST_CASE("scalar: provides sqrt function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    area_t const x{9};
    CHECK(dak::sqrt(x) == length_t{3});
}

TEST_CASE("scalar: provides cbrt function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using volume_t = dak::scalar<double, dak::mechanical_dimension<3, 0, 0>>;

    volume_t const x{125};
    CHECK(dak::cbrt(x) == length_t{5});
}

TEST_CASE("scalar: example: mechanical quantities")
{
    using length_t = dak::scalar<double, dak::dim::length>;
    using mass_t = dak::scalar<double, dak::dim::mass>;
    using duration_t = dak::scalar<double, dak::dim::time>;
    using speed_t = dak::scalar<double, dak::dim::speed>;
    using momentum_t = dak::scalar<double, dak::dim::momentum>;
    using force_t = dak::scalar<double, dak::dim::force>;
    using energy_t = dak::scalar<double, dak::dim::energy>;

    length_t const distance{30};
    mass_t const mass{2};
    duration_t const time{5};
    speed_t const speed = distance / time;
    momentum_t const momentum = speed * mass;
    force_t const force = momentum / time;
    energy_t const energy = force * distance;
    double const some_ratio = dak::sqrt(mass * energy) / momentum;
    (void) some_ratio;
}

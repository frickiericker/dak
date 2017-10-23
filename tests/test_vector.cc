#include <type_traits>

#include <dak.hpp>
#include <doctest.h>

TEST_CASE("vector: defined at least for four dimensions")
{
    using vec1 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using vec2 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using vec3 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using vec4 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    CHECK(sizeof(vec1) > 0);
    CHECK(sizeof(vec2) > 0);
    CHECK(sizeof(vec3) > 0);
    CHECK(sizeof(vec4) > 0);
    (void) vec1{1};
    (void) vec2{1, 2};
    (void) vec3{1, 2, 3};
    (void) vec4{1, 2, 3, 4};
}

TEST_CASE("vector: is trivially copyable")
{
    using float_displace_t = dak::vector<float, dak::mechanical_dimension<1, 0, 0>, 3>;
    using double_displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_trivially_copyable<float_displace_t>::value));
    CHECK((std::is_trivially_copyable<double_displace_t>::value));
}

TEST_CASE("vector: is explicitly constructible with coordinate values")
{
    using number_t = dak::vector<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    number_t{1.0, 2.0, 3.0};
    CHECK((std::is_constructible<number_t, double, double, double>::value));
    CHECK((std::is_constructible<displace_t, double, double, double>::value));
}

TEST_CASE("vector: is default constructed to zero")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    displace_t x;
    CHECK(x == displace_t{0, 0, 0});
}

TEST_CASE("vector: disallows dimension-altering conversion")
{
    using number_t = dak::vector<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    CHECK_FALSE((std::is_convertible<number_t, displace_t>::value));
    CHECK_FALSE((std::is_convertible<displace_t, number_t>::value));

    CHECK_FALSE((std::is_convertible<displace_t, velocity_t>::value));
    CHECK_FALSE((std::is_convertible<velocity_t, displace_t>::value));

    CHECK_FALSE((std::is_convertible<velocity_t, number_t>::value));
    CHECK_FALSE((std::is_convertible<number_t, velocity_t>::value));
}

TEST_CASE("vector: exposes number_type")
{
    using float_displace_t = dak::vector<float, dak::mechanical_dimension<1, 0, -1>, 3>;
    using double_displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    CHECK((std::is_same<float_displace_t::number_type, float>::value));
    CHECK((std::is_same<double_displace_t::number_type, double>::value));
}

TEST_CASE("vector: exposes scalar_type")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    CHECK((std::is_same<displace_t::scalar_type, length_t>::value));
}

TEST_CASE("vector: exposes spatial dimension")
{
    using vec1 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using vec2 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using vec3 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using vec4 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    using vec9 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 9>;
    CHECK((std::integral_constant<unsigned, vec1::dimension>::value) == 1);
    CHECK((std::integral_constant<unsigned, vec2::dimension>::value) == 2);
    CHECK((std::integral_constant<unsigned, vec3::dimension>::value) == 3);
    CHECK((std::integral_constant<unsigned, vec4::dimension>::value) == 4);
    CHECK((std::integral_constant<unsigned, vec9::dimension>::value) == 9);
}

TEST_CASE("vector: provides indexing")
{
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    SUBCASE("mutable")
    {
        velocity_t v;
        v[0] = speed_t{1};
        v[1] = speed_t{2};
        v[2] = speed_t{3};
        CHECK(v[0] == speed_t{1});
        CHECK(v[1] == speed_t{2});
        CHECK(v[2] == speed_t{3});
    }

    SUBCASE("const")
    {
        velocity_t const v{1, 2, 3};
        CHECK(v[0] == speed_t{1});
        CHECK(v[1] == speed_t{2});
        CHECK(v[2] == speed_t{3});
    }
}

TEST_CASE("vector: provides equality comparison operators")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{1, 2, 3};
    displace_t const z{4, 5, 6};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("vector: provides += operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x += displace_t{4, 5, 6});
    CHECK(&x == &y);
    CHECK(x == displace_t{5, 7, 9});
}

TEST_CASE("vector: provides -= operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x -= displace_t{6, 5, 4});
    CHECK(&x == &y);
    CHECK(x == displace_t{-5, -3, -1});
}

TEST_CASE("vector: provides *= operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x *= 2);
    CHECK(&x == &y);
    CHECK(x == displace_t{2, 4, 6});
}

TEST_CASE("vector: provides /= operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{2, 4, 6};
    displace_t& y = (x /= 2);
    CHECK(&x == &y);
    CHECK(x == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides unary + operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const& y = +x;
    CHECK(&x != &y);
    CHECK(y == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides unary - operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y = -x;
    CHECK(y == displace_t{-1, -2, -3});
}

TEST_CASE("vector: provides binary + operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK((x + y) == displace_t{5, 7, 9});
}

TEST_CASE("vector: provides binary - operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{5, 7, 9};
    displace_t const y{1, 2, 3};
    CHECK((x - y) == displace_t{4, 5, 6});
}

TEST_CASE("vector: provides binary * operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    CHECK((2 * x) == displace_t{2, 4, 6});
    CHECK((x * 3) == displace_t{3, 6, 9});
}

TEST_CASE("vector: provides binary / operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{3, 6, 9};
    CHECK((x / 3) == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides binary * operator for dimensional conversion")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    displace_t const x{1, 2, 3};
    frequency_t const y{3};
    CHECK((x * y) == velocity_t{3, 6, 9});
    CHECK((y * x) == velocity_t{3, 6, 9});
}

TEST_CASE("vector: provides binary / operator for dimensional conversion")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    displace_t const x{3, 6, 9};
    duration_t const y{3};
    CHECK((x / y) == velocity_t{1, 2, 3});
}

TEST_CASE("vector: provides dot function")
{
    using force_t = dak::vector<double, dak::mechanical_dimension<1, 1, -2>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using energy_t = dak::scalar<double, dak::mechanical_dimension<2, 1, -2>>;

    force_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK(dak::dot(x, y) == energy_t{4 + 10 + 18});
}

TEST_CASE("vector: provides squared_norm function")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    displace_t const x{1, 2, 3};
    CHECK(dak::squared_norm(x) == area_t{1 + 4 + 9});
}

TEST_CASE("vector: provides norm function")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    displace_t const x{3, 4};
    CHECK(dak::norm(x) == length_t{5});
}

TEST_CASE("vector: provides cross function for three-dimensional vectors")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_vector_t = dak::vector<double, dak::mechanical_dimension<2, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK(dak::cross(x, y) == area_vector_t{2 * 6 - 3 * 5, 3 * 4 - 1 * 6, 1 * 5 - 2 * 4});

    // Invariant
    CHECK(dak::cross(x, x) == area_vector_t{0, 0, 0});
    CHECK(dak::cross(y, y) == area_vector_t{0, 0, 0});
}

TEST_CASE("vector: example: mechanical quantities")
{
    using mass_t = dak::scalar<double, dak::dim::mass>;
    using duration_t = dak::scalar<double, dak::dim::time>;
    using energy_t = dak::scalar<double, dak::dim::energy>;

    using displace_t = dak::vector<double, dak::dim::length, 3>;
    using velocity_t = dak::vector<double, dak::dim::speed, 3>;
    using momentum_t = dak::vector<double, dak::dim::momentum, 3>;
    using force_t = dak::vector<double, dak::dim::force, 3>;

    displace_t const displacement{10, 20, 30};
    mass_t const mass{2};
    duration_t const time{5};
    velocity_t const velocity = displacement / time;
    momentum_t const momentum = velocity * mass;
    force_t const force = momentum / time;
    energy_t const energy = dak::dot(force, displacement);
    double const some_ratio = dak::sqrt(mass * energy) / dak::norm(momentum);
    (void) some_ratio;
}

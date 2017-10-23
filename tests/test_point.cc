#include <type_traits>

#include <dak.hpp>
#include <doctest.h>

TEST_CASE("point: defined at least for four dimensions")
{
    using point1 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using point2 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using point3 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using point4 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    CHECK(sizeof(point1) > 0);
    CHECK(sizeof(point2) > 0);
    CHECK(sizeof(point3) > 0);
    CHECK(sizeof(point4) > 0);
    (void) point1{1};
    (void) point2{1, 2};
    (void) point3{1, 2, 3};
    (void) point4{1, 2, 3, 4};
}

TEST_CASE("point: is trivially copyable")
{
    using float_point_t = dak::point<float, dak::mechanical_dimension<1, 0, 0>, 3>;
    using double_point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_trivially_copyable<float_point_t>::value));
    CHECK((std::is_trivially_copyable<double_point_t>::value));
}

TEST_CASE("vector: is explicitly constructible with coordinate values")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    point_t{1.0, 2.0, 3.0};
    CHECK((std::is_constructible<point_t, double, double, double>::value));
}

TEST_CASE("point: is default constructed to zero")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    point_t x;
    CHECK(x == point_t{0, 0, 0});
}

TEST_CASE("point: disallows dimension-altering conversion")
{
    using tuple_t = dak::point<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    CHECK_FALSE((std::is_convertible<tuple_t, point_t>::value));
    CHECK_FALSE((std::is_convertible<point_t, tuple_t>::value));
}

TEST_CASE("point: exposes number_type")
{
    using float_point_t = dak::point<float, dak::mechanical_dimension<1, 0, -1>, 3>;
    using double_point_t = dak::point<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    CHECK((std::is_same<float_point_t::number_type, float>::value));
    CHECK((std::is_same<double_point_t::number_type, double>::value));
}

TEST_CASE("point: exposes scalar_type")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<point_t::scalar_type, length_t>::value));
}

TEST_CASE("point: exposes vector_type")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_same<point_t::vector_type, displace_t>::value));
}

TEST_CASE("point: exposes spatial dimension")
{
    using point1 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using point2 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using point3 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using point4 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    using point9 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 9>;
    CHECK((std::integral_constant<unsigned, point1::dimension>::value) == 1);
    CHECK((std::integral_constant<unsigned, point2::dimension>::value) == 2);
    CHECK((std::integral_constant<unsigned, point3::dimension>::value) == 3);
    CHECK((std::integral_constant<unsigned, point4::dimension>::value) == 4);
    CHECK((std::integral_constant<unsigned, point9::dimension>::value) == 9);
}

TEST_CASE("point: provides indexing")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("mutable")
    {
        point_t p;
        p[0] = length_t{1};
        p[1] = length_t{2};
        p[2] = length_t{3};
        CHECK(p[0] == length_t{1});
        CHECK(p[1] == length_t{2});
        CHECK(p[2] == length_t{3});
    }

    SUBCASE("const")
    {
        point_t const p{1, 2, 3};
        CHECK(p[0] == length_t{1});
        CHECK(p[1] == length_t{2});
        CHECK(p[2] == length_t{3});
    }
}

TEST_CASE("point: provides equality comparison operators")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{1, 2, 3};
    point_t const y{1, 2, 3};
    point_t const z{4, 5, 6};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("point: provides += operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t x{1, 2, 3};
    point_t& y = (x += displace_t{4, 5, 6});
    CHECK(&x == &y);
    CHECK(x == point_t{5, 7, 9});
}

TEST_CASE("point: provides -= operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t x{1, 2, 3};
    point_t& y = (x -= displace_t{6, 5, 4});
    CHECK(&x == &y);
    CHECK(x == point_t{-5, -3, -1});
}

TEST_CASE("point: provides binary + operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK((x + y) == point_t{5, 7, 9});
}

TEST_CASE("point: provides binary - operator for computing displacement")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{5, 7, 9};
    point_t const y{1, 2, 3};
    CHECK((x - y) == displace_t{4, 5, 6});
}

TEST_CASE("point: provides squared_distance function")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    point_t const p{1, 2, 3};
    point_t const q{6, 5, 4};
    CHECK(dak::squared_distance(p, q) == area_t{5 * 5 + 3 * 3 + 1 * 1});
}

TEST_CASE("point: provides distance function")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    point_t const p{1, 2};
    point_t const q{4, 6};
    CHECK(dak::distance(p, q) == length_t{5});
}

TEST_CASE("point: example: two-body simulation")
{
    using mass_t = dak::scalar<double, dak::dim::mass>;
    using duration_t = dak::scalar<double, dak::dim::time>;
    using grav_constant_t = dak::scalar<double, dak::mechanical_dimension<3, -1, -2>>;

    using displace_t = dak::vector<double, dak::dim::length, 3>;
    using momentum_t = dak::vector<double, dak::dim::momentum, 3>;
    using force_t = dak::vector<double, dak::dim::force, 3>;

    using point_t = dak::point<double, dak::dim::length, 3>;

    point_t a_position{0, 0, 0};
    point_t b_position{1, 2, 3};
    momentum_t a_momentum{1, 2, 3};
    momentum_t b_momentum{0, 0, 0};

    grav_constant_t const G{1};
    mass_t const a_mass{1};
    mass_t const b_mass{1};
    duration_t const dt{0.01};

    displace_t const r = a_position - b_position;
    force_t const force = -G * a_mass * b_mass * r / dak::pow<3>(dak::norm(r));
    a_momentum += force * dt;
    b_momentum += -force * dt;
    a_position += a_momentum * dt / a_mass;
    b_position += b_momentum * dt / b_mass;
}

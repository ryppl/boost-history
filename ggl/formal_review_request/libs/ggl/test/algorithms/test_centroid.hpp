// Generic Geometry Library test file
//
// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_TEST_CENTROID_HPP
#define GGL_TEST_CENTROID_HPP

// Test-functionality, shared between single and multi tests

template<std::size_t D>
struct check_result
{
};

template <>
struct check_result<2>
{
    template <typename Point, typename T>
    static void apply(Point const& p, T const& x, T const& y, T const&)
    {
        BOOST_CHECK_CLOSE(double(ggl::get<0>(p)), double(x), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<1>(p)), double(y), 0.001);
    }
};


template <>
struct check_result<3>
{
    template <typename Point, typename T>
    static void apply(Point const& p, T const& x, T const& y, T const& z)
    {
        BOOST_CHECK_CLOSE(double(ggl::get<0>(p)), double(x), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<1>(p)), double(y), 0.001);
        BOOST_CHECK_CLOSE(double(ggl::get<2>(p)), double(z), 0.001);
    }
};



template <typename CalculationType, typename Geometry, typename Point>
void test_with_other_calculation_type(Geometry const& geometry, Point& c1)
{
    typedef typename ggl::point_type<Geometry>::type point_type;
    // Calculate it with user defined strategy
    point_type c2;
    ggl::centroid(geometry, c2,
        ggl::strategy::centroid::bashein_detmer<point_type, point_type, CalculationType>());

    std::cout << typeid(CalculationType).name() << ": " << std::setprecision(20)
        << ggl::get<0>(c2) << " " << ggl::get<1>(c2)
        << " -> difference: " << ggl::distance(c1, c2)
        << std::endl;
}

template <typename Geometry, typename T>
void test_centroid(std::string const& wkt, T const& x, T const& y, T const& z = T())
{
    Geometry geometry;
    ggl::read_wkt(wkt, geometry);
    typedef typename ggl::point_type<Geometry>::type point_type;
    point_type c1;
    ggl::centroid(geometry, c1);
    check_result<ggl::dimension<Geometry>::type::value>::apply(c1, x, y, z);

#ifdef REPORT_RESULTS
    std::cout << "normal: " << std::setprecision(20) << ggl::get<0>(c1) << " " << ggl::get<1>(c1) << std::endl;

    //test_with_other_calculation_type<long long>(geometry, c1);
    test_with_other_calculation_type<float>(geometry, c1);
    test_with_other_calculation_type<long double>(geometry, c1);
#if defined(HAVE_GMP)
    test_with_other_calculation_type<boost::numeric_adaptor::gmp_value_type>(geometry, c1);
#endif
#if defined(HAVE_CLN)
    test_with_other_calculation_type<boost::numeric_adaptor::cln_value_type>(geometry, c1);
#endif

#endif
}


#endif

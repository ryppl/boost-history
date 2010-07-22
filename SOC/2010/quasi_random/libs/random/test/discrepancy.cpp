// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#include "d_star.hpp"

#include <boost/random/niederreiter_base2.hpp>
#include <boost/random/sobol.hpp>
#include <boost/random/faure.hpp>

#include <boost/random/mersenne_twister.hpp>

#include <boost/random/detail/identity_distribution.hpp>

#include <boost/utility/result_of.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/floating_point_comparison.hpp>

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// There are 3 tests:
// 1) test_halton_discrepancy
// This tests the discrepancy test itself. We give a sequence of points (Halton sequence)
// for which the discrepancy bounds are known and then check whether the computed values
// are what we expect. D_n^*(x) between 0.2666670000 and 0.2724280122
//
// 2) test_qrng_discrepancy
// We compare the discrepancy of the Mersenne twister generator against the discrepancy
// of low discrepancy sequence generators. It must be noted that the Mersenne twister generator
// is a pseudo-random number generator and the sequence that it produces does not have the
// property of low discrepancy, thus any quasi-random number generator must have
// lower discrepancy bounds.
//
// 3) test_niederreiter_base_2_discrepancy
// The Niederreiter base 2 discrepancy satisfies D_n^*(x) = O( (log n)^s∕n ).
// For large dimension s, the theoretical bound (log n)^s∕n may only be meaningful
// for extremely large values of n.
// We check that the discrepancy measurer indeed returns indicated values for
// sufficienty large n of Niederreiter base 2 discrepancy sequence elements.



// This will test the discrepancy test itself. We give a sequence of points (Halton sequence)
// for which the discrepancy bounds are known and then check whether the computed values
// are what we expect.
BOOST_AUTO_TEST_CASE( test_halton_discrepancy )
{
  double halton_seq[10][2] =
  {
    {         0,           0    },
    {       0.5,    0.333333    },
    {      0.25,    0.666667    },
    {      0.75,    0.111111    },
    {     0.125,    0.444444    },
    {     0.625,    0.777778    },
    {     0.375,    0.222222    },
    {     0.875,    0.555556    },
    {    0.0625,    0.888889    },
    {    0.5625,    0.037037    }
  };

  std::pair<double, double> bounds = d_star(halton_seq, 0.01);

  BOOST_REQUIRE_CLOSE(bounds.first, 0.2666670000, 0.0001);
  BOOST_REQUIRE_CLOSE(bounds.second, 0.2724280122, 0.0001);
}


template<typename QEngine, typename Distribution, typename Bounds>
inline void check_discrepancy(const char* name,
    Distribution d,
    std::size_t n_vectors,
    typename boost::result_of<Distribution(QEngine)>::type epsilon,
    Bounds mersenne_twister_discrepancy)
{
  QEngine q; // default-construct

  BOOST_TEST_MESSAGE( "Testing " << name << "[" << q.dimension() << "]" );

  Bounds qrng_discrepancy = d_star(q, d, n_vectors, epsilon);
  BOOST_CHECK_LT( qrng_discrepancy.first, mersenne_twister_discrepancy.first );
  BOOST_CHECK_LE( qrng_discrepancy.second, mersenne_twister_discrepancy.second );
}

template<std::size_t D>
inline void compare_qrng_discrepancy(std::size_t n_vectors, double eps)
{
  // We test with double precision
  typedef double value_t;

  // Default parameterization of D-dimensional qrng generator templates
  typedef typename boost::niederreiter_base2_generator<D>::type niederreiter_base2_t;
  typedef typename boost::sobol_generator<D>::type sobol_t;
  typedef typename boost::faure_generator<D, value_t>::type faure_t;

  typedef std::pair<value_t, value_t> bounds_t;

  BOOST_REQUIRE( n_vectors > 0 );
  BOOST_REQUIRE( eps > 0 && eps < 1 );
  BOOST_TEST_MESSAGE( "Starting tests in dimension " << D << " with sample size " << n_vectors << ", epsilon=" << eps );

  boost::uniform_real<value_t> u;

  // Compute the discrepancy of the Mersenne twister pseudo-random number generator
  bounds_t mt_discrepancy;
  {
    boost::mt19937 mt;

    BOOST_TEST_MESSAGE( "Computing discrepancy bounds for mt19937[" << D << "]" );
    mt_discrepancy = d_star(mt, u, D, n_vectors, eps);
  }

#define UNIT_TEST_CHECK_QRNG_DISCREPANCY(N, U) \
  check_discrepancy<N>(#N, U, n_vectors, eps, mt_discrepancy); \
/**/

  // Compare the discrepancy of quasi-random number generators against the Mersenne twister discrepancy
  UNIT_TEST_CHECK_QRNG_DISCREPANCY(niederreiter_base2_t, u);
  UNIT_TEST_CHECK_QRNG_DISCREPANCY(sobol_t, u);
  UNIT_TEST_CHECK_QRNG_DISCREPANCY(faure_t, boost::random::detail::identity_distribution());

#undef UNIT_TEST_CHECK_QRNG_DISCREPANCY
}

BOOST_AUTO_TEST_CASE( test_qrng_discrepancy )
{
  // Discrepancy computation in high dimensions is quite expensive, so we have
  // to decrease the sample size and/or decrease subdivision precision (increase epsilon)
  // accordingly, so that tests finish reasonably fast.
  // However, sample size and precision cannot be decreased haphazardly. Take too
  // small a sample and intrinsic random number generator properties will vanish.
  // Take too rough a precision and those properties will be determined with
  // a big error margin.
  // From this follows that there are three possible reasons for discrepancy test
  // failures:
  //   1) not enough sample points given;
  //   2) not enough precision (epsilon is too big);
  //   3) genuine qrng implementation errors.
  // All those three points have to be considered carefully, but
  // first thing to do, if some test fails, is to increase sample point size!

  compare_qrng_discrepancy<2>(  400,    0.01  );
  compare_qrng_discrepancy<3>(  100,    0.01  );
  compare_qrng_discrepancy<4>(  100,    0.04  );
  compare_qrng_discrepancy<5>(  100,    0.06  );
  compare_qrng_discrepancy<6>(   60,    0.08  );
  compare_qrng_discrepancy<7>(   60,    0.15  );
  compare_qrng_discrepancy<8>(  100,    0.4   );
  compare_qrng_discrepancy<9> (  60,    0.5   );
  compare_qrng_discrepancy<10>(  80,    0.3   ); // mersenne twister has good discrepancy from dim 10. :-)
  compare_qrng_discrepancy<11>(  500,   0.4   );
  compare_qrng_discrepancy<12>(  500,   0.5   );

  // etc.
}


template<std::size_t D>
inline void check_niederreiter_base_2_discrepancy(std::size_t n_vectors, double eps)
{
  // Default parameterization of D-dimensional qrng generator templates
  typedef typename boost::niederreiter_base2_generator<D>::type niederreiter_base2_t;

  BOOST_REQUIRE( n_vectors > 0 );
  BOOST_REQUIRE( eps > 0 && eps < 1 );

  // D_n^*(x) = O( (log n)^s∕n )
  double expected_discrepancy = std::pow(log(n_vectors), D) / n_vectors;
  BOOST_TEST_MESSAGE( "Expected discrepancy O( (log n)^s∕n ) = " << expected_discrepancy );

  niederreiter_base2_t q;
  std::pair<double, double> bounds = d_star(q, n_vectors, eps);

  BOOST_CHECK_LE( bounds.first, expected_discrepancy );
  BOOST_CHECK_LE( expected_discrepancy, bounds.second );
}

BOOST_AUTO_TEST_CASE( test_niederreiter_base_2_discrepancy )
{
  // The Niederreiter base 2 discrepancy satisfies
  // D_n^*(x) = O( (log n)^s∕n ).
  // Note, that for large dimension s, the theoretical bound (log n)^s∕n may
  // only be meaningful for extremely large values of n.
  // The bound in Koksma-Hlawka inequality gives no relevant information
  // until a very large number of points is used.
  BOOST_TEST_MESSAGE( "Starting Niederreiter base 2 discrepancy checks" );
  check_niederreiter_base_2_discrepancy<2>(10000, 0.01);
  check_niederreiter_base_2_discrepancy<3>(200000, 0.01);
}

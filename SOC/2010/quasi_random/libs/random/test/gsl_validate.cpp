// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#include <boost/random/niederreiter_base2.hpp>
#include <boost/random/sobol.hpp>
#include <boost/random/faure.hpp>

#include <gsl/gsl_qrng.h>

#include <boost/random/uniform_real.hpp>

#include <boost/preprocessor/repeat_from_to.hpp>

#include <boost/shared_ptr.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the quasi-random number generators.
// There are two sets of tests:
// 1) validate_sobol;
// 2) validate_niederreiter_base2.
// These tests compare our results with values produced by qrngs in
// GNU Scientific Library.

struct gsl_deleter
{
  void operator()(gsl_qrng *q) const
  {
    if( q != 0 )
      gsl_qrng_free(q);
  }
};

// Test routine
template<std::size_t D, typename QEngine>
bool test_qrng(const gsl_qrng_type* qrng_type, QEngine& eng)
{
  const int validating_sequence_lenght = 10000;

  BOOST_REQUIRE_EQUAL( eng.dimension(), D ); // paranoid check

  boost::shared_ptr<gsl_qrng> q(gsl_qrng_alloc(qrng_type, D), gsl_deleter());
  // RAII, to ensure that no memory leaks occur
  BOOST_REQUIRE( q.get() != 0 );

  double val[D];
  boost::uniform_real<double> dist;

  for( int i = 0; i < validating_sequence_lenght; ++i )
  {
    gsl_qrng_get(q.get(), val);
    for( std::size_t j = 0; j != D; ++j )
    {
      double n = dist(eng);

      // We want to check that quasi-random number generator values differ no
      // more than 0.0001% of their value.
      BOOST_REQUIRE_CLOSE(val[j], n, 0.0001);
    }
  }

  return true;
}


template<std::size_t D>
void test_sobol()
{
  typedef typename boost::sobol_generator<D>::type sobol_t;
  sobol_t eng;
  eng.discard(D); // GSL shifts sequence by 1
  BOOST_CHECK( (test_qrng<D>(::gsl_qrng_sobol, eng)) );
}

template<std::size_t D>
void test_niederreiter_base2()
{
  typedef typename boost::niederreiter_base2_generator<D>::type niederreiter_base2_t;
  niederreiter_base2_t eng;
  BOOST_CHECK( (test_qrng<D>(::gsl_qrng_niederreiter_2, eng)) );
}


BOOST_AUTO_TEST_CASE( gsl_validate_sobol )
{
#define UNIT_TEST_QRNG_VALIDATE_SOBOL(z, N, text) test_sobol<N>();

  BOOST_PP_REPEAT_FROM_TO(1, 41, UNIT_TEST_QRNG_VALIDATE_SOBOL, unused)

#undef UNIT_TEST_QRNG_VALIDATE_SOBOL
}

BOOST_AUTO_TEST_CASE( gsl_validate_niederreiter_base2 )
{
#define UNIT_TEST_QRNG_VALIDATE_NIEDERREITER_BASE2(z, N, text) test_niederreiter_base2<N>();

  // GSL supports only up to 12 dimensions, so we check only that much
  BOOST_PP_REPEAT_FROM_TO(1, 13, UNIT_TEST_QRNG_VALIDATE_NIEDERREITER_BASE2, unused)

#undef UNIT_TEST_QRNG_VALIDATE_NIEDERREITER_BASE2
}

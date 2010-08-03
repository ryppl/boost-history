// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef BOOST_RANDOM_DETAIL_INTEGER_POWERS_HPP
#define BOOST_RANDOM_DETAIL_INTEGER_POWERS_HPP

#include <boost/static_assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/iteration/local.hpp>

namespace boost { namespace random {

namespace detail {

template<typename Powers, std::size_t Base, std::size_t MaxValue>
struct generate_integer_powers_impl
{
  BOOST_STATIC_CONSTANT(std::size_t, last_power =
    mpl::back<Powers>::type::value );

  typedef typename mpl::push_back<
      Powers
    , mpl::integral_c<std::size_t, last_power * Base>
  >::type powers_t;

  // Computation must be lazy!
  typedef typename mpl::if_c<
      (last_power <= MaxValue)
    , generate_integer_powers_impl<powers_t, Base, MaxValue>
    , mpl::identity<Powers>
  >::type selector_t;
  typedef typename selector_t::type type;
};

template<std::size_t Base>
struct generate_integer_powers
{
  BOOST_STATIC_ASSERT( Base >= 2 );

  BOOST_STATIC_CONSTANT(std::size_t, max_bin_value = ~std::size_t(0) );
  BOOST_STATIC_CONSTANT(std::size_t, max_value = max_bin_value / Base );

  typedef boost::mpl::vector_c<std::size_t, 1> base0_power_t;
  typedef generate_integer_powers_impl<base0_power_t, Base, max_value> gen_powers_t;

  // Here we generate an MPL vector of values
  // Base^0, ..., Base^m where Base^m <= max_value
  typedef typename gen_powers_t::type type;
};


// Performs binary search and returns the lower bound of the range
template<std::size_t Pos>
struct range_not_found
{
  inline static std::size_t apply(std::size_t)
  { return Pos; }
};

template<std::size_t Pos>
struct range_prev
{
  BOOST_STATIC_CONSTANT(std::size_t, value = Pos - 1);
};

template<>
struct range_prev<0>
{
  BOOST_STATIC_CONSTANT(std::size_t, value = 0); // integer_log<Base>(0) == 0!
};

template<typename Powers, std::size_t Low, std::size_t High>
struct log_lower_bound
{
  BOOST_STATIC_CONSTANT(std::size_t, mid = (Low + High) / 2);

  // Termination conditions are described here
  typedef log_lower_bound<Powers, Low, mid - 1> lower_search_t;
  typedef typename mpl::if_c< (Low >= mid)
      , range_not_found<range_prev<mid>::value>
      , lower_search_t
  >::type lower_t;
  typedef log_lower_bound<Powers, mid + 1, High> upper_search_t;
  typedef typename mpl::if_c< (mid >= High)
      , range_not_found<High>
      , upper_search_t
  >::type upper_t;

  typedef typename mpl::at_c<Powers, mid>::type value_t;

  inline static std::size_t apply(std::size_t arg)
  {
    if( arg > value_t::value )
      return upper_t::apply(arg);
    else if( arg < value_t::value )
      return lower_t::apply(arg);
    else
      return mid;
  }
};

// Returns the integer part of the logarithm base Base of arg.
// In erroneous situations, e.g., integer_log<Base>(0) the function
// returns 0 and does not report the error. This is the intended
// behavior.
// The function performs binary range search, so for big args
// it works substantially faster (measured ~5x) than the trivial
//    std::size_t ilog = 0;
//    while( Base <= arg )
//    {
//      arg /= Base; ++ilog;
//    }
template<std::size_t Base>
inline std::size_t integer_log(std::size_t arg)
{
  typedef typename generate_integer_powers<Base>::type powers_t;
  typedef typename mpl::size<powers_t>::type size_t; // always >= 1
  typedef log_lower_bound<powers_t, 0, size_t::value - 1> binlookup_t;
  return binlookup_t::apply(arg);
}


// This is more efficient than naively multiplying the base with itself repeatedly.
// In erroneous situations, e.g., integer_pow<0>(0) the function returns 0
// and does not report the error. This is the intended behavior.

namespace pow_switch_detail {

// D is the number of cases without the default
template<std::size_t D>
struct pow_switch_impl;

// specialize for 0 to appease the compiler,
// but we know there cannot be size 0 vectors
template<>
struct pow_switch_impl<0> {
  template<class V>
  inline static std::size_t apply(std::size_t) {
    return 0;
  }
};

#define BOOST_RANDOM_DETAIL_INTEGER_POW_CASE_IMPL(z, N, data) \
  case N: { \
    return boost::mpl::at_c<data, N>::type::value; \
  } \
/**/

// Here we assume that compilers nowadays are clever enough to optimize this monstrosity
#define BOOST_RANDOM_DETAIL_INTEGER_POW_SWITCH_IMPL(z, N, data) \
  template<> \
  struct pow_switch_impl<N> { \
    template<typename Seq> \
    inline static std::size_t apply(std::size_t arg) { \
      switch( arg ) { \
        BOOST_PP_REPEAT_##z(N, BOOST_RANDOM_DETAIL_INTEGER_POW_CASE_IMPL, Seq) \
        default: return 0; \
      } \
    } \
  }; \
/**/

#define BOOST_PP_LOCAL_LIMITS (1, 64) // up to 2^63
#define BOOST_PP_LOCAL_MACRO(n) BOOST_RANDOM_DETAIL_INTEGER_POW_SWITCH_IMPL(1, n, ~)
#include BOOST_PP_LOCAL_ITERATE()

#undef BOOST_RANDOM_DETAIL_INTEGER_POW_SWITCH_IMPL
#undef BOOST_RANDOM_DETAIL_INTEGER_POW_CASE_IMPL

} // namespace pow_switch_detail

template<std::size_t Base>
struct integer_pow_func
{
  typedef typename generate_integer_powers<Base>::type powers_t;
  typedef pow_switch_detail::pow_switch_impl<boost::mpl::size<powers_t>::value> switch_t;

  inline static std::size_t apply(std::size_t arg)
  {
    return switch_t::template apply<powers_t>(arg);
  }
};

template<>
struct integer_pow_func<1> // 1^p == 1
{
  inline static std::size_t apply(std::size_t)
  { return 1; }
};

template<>
struct integer_pow_func<0> // 0^p == 0, also 0^0 == 0!
{
  inline static std::size_t apply(std::size_t)
  { return 0; }
};

template<std::size_t Base>
inline std::size_t integer_pow(std::size_t arg)
{
  return integer_pow_func<Base>::apply(arg);
}

} // namespace detail

}} // namespace boost::random

#endif // BOOST_RANDOM_DETAIL_INTEGER_POWERS_HPP

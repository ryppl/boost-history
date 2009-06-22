// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdint.hpp>
#include <boost/mp_math/integer.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

static const bool use_debug_mode = true;

typedef boost::mpl::vector<
  boost::mp_math::integer<
    boost::mp_math::unbounded<
      false,
      std::allocator<void>,
      boost::mp_math::unbounded_traits<
        boost::uint8_t, boost::uint16_t, std::size_t, use_debug_mode>
    >
  >,
  boost::mp_math::integer<
    boost::mp_math::unbounded<
      false,
      std::allocator<void>,
      boost::mp_math::unbounded_traits<
        boost::uint16_t, boost::uint32_t, std::size_t, use_debug_mode>
    >
  >,
#ifndef BOOST_NO_INT64_T
  boost::mp_math::integer<
    boost::mp_math::unbounded<
      false,
      std::allocator<void>,
      boost::mp_math::unbounded_traits<
        boost::uint32_t, boost::uint64_t, std::size_t, use_debug_mode>
    >
  >,
#endif
  boost::mp_math::integer<boost::mp_math::unbounded<false> >,
  boost::mp_math::unbounded<false>,
  boost::mp_math::unbounded_uint<>
> UIntTypes2;


struct UIntTypes
:
  boost::mpl::if_c<
    use_debug_mode,
      UIntTypes2,
      boost::mpl::unique<
        UIntTypes2,
        boost::is_same<boost::mpl::_1, boost::mpl::_2>
    >
  >::type
{};


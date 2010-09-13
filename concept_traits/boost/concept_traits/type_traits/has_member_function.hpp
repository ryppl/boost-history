///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_HAS_MEMBER_FUNCTION_HPP
#define BOOST_CT_TT_EXT_HAS_MEMBER_FUNCTION_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval.hpp>
#include <boost/type_traits/pointer_to_member_function.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_member_function
///////////////////////////////////////////////////////////////////////////////

template<
  class T,
  template<class> class MemberFunction,
  class R,
  class P1 = mpl::void_,
  class P2 = mpl::void_,
  class P3 = mpl::void_,
  class P4 = mpl::void_,
  class P5 = mpl::void_
>
struct has_member_function :
  mpl::eval<
    MemberFunction<
      mpl::eval<
        pointer_to_member_function<
          mpl::identity<T>,
          R,
          P1,
          P2,
          P3,
          P4,
          P5
        >
      >
    >
  > {};

} // namespace boost

#endif // BOOST_CT_TT_EXT_HAS_MEMBER_FUNCTION_HPP


// (C) Copyright Steve Cleary, Beman Dawes, Aleksey Gurtovoy,
// Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_SAME_HPP_INCLUDED
#define BOOST_TT_IS_SAME_HPP_INCLUDED

#include "boost/type_traits/config.hpp"
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include "boost/type_traits/detail/yes_no_type.hpp"
#include "boost/type_traits/detail/ice_and.hpp"
#include "boost/type_traits/is_reference.hpp"
#endif
// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

BOOST_TT_AUX_BOOL_TRAIT_DEF2(is_same,T,U,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T,T,true)
#if BOOST_WORKAROUND(__BORLANDC__, < 0x600)
// without this, Borland's compiler gives the wrong answer for
// references to arrays:
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T&,T&,true)
#endif

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

#ifdef BOOST_MSVC
// the following VC6 specific implementation is *NOT* legal
// C++, but has the advantage that it works for incomplete
// types.

template< typename T1 >
struct is_same_part_1
{
    template<typename T2>  struct part_2     { enum { value = false }; };
    template<>             struct part_2<T1> { enum { value = true }; };
};

template< typename T1, typename T2 >
struct is_same_impl
{
    enum { value = detail::is_same_part_1<T1>::template part_2<T2>::value };
};

#else // generic "no-partial-specialization" version

template <typename T>
::boost::type_traits::yes_type
BOOST_TT_DECL is_same_tester(T*, T*);

::boost::type_traits::no_type
BOOST_TT_DECL is_same_tester(...);

template <typename T, typename U>
struct is_same_impl
{
   static T t;
   static U u;

   BOOST_STATIC_CONSTANT(bool, value =
      (::boost::type_traits::ice_and<
         (sizeof(type_traits::yes_type) == sizeof(detail::is_same_tester(&t,&u))),
         (::boost::is_reference<T>::value == ::boost::is_reference<U>::value),
         (sizeof(T) == sizeof(U))
        >::value));
};

#endif // BOOST_MSVC

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF2(is_same,T,U,(::boost::detail::is_same_impl<T,U>::value))

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif  // BOOST_TT_IS_SAME_HPP_INCLUDED


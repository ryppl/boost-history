//-----------------------------------------------------------------------------
// boost mpl/comparison/less.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_COMPARISON_LESS_HPP
#define BOOST_MPL_COMPARISON_LESS_HPP

#include "boost/mpl/bool_t.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

namespace detail {
// workaround for bcc integral constant expressions bugs
template< 
      typename T1
    , typename T2
    >
struct less_impl
{
    // MSVC6.5 workaround: '<' here correlates badly with boost::bind
    BOOST_STATIC_CONSTANT(bool, value = !(T1::value >= T2::value));
};

} // namespace detail

template< 
      typename T1
    , typename T2
    >
struct less
{
 private:
    typedef mpl::detail::less_impl<T1,T2> impl_;
 
 public:
    BOOST_STATIC_CONSTANT(bool, value = impl_::value);
    typedef mpl::bool_t<(impl_::value)> type;
};

// shortcut form for 'less than N'
template<long N>
struct lt
{
    template<typename U> struct apply
//        : less< U,mpl::int_t<N> >
    {
        typedef less< U,mpl::int_t<N> > type;
        BOOST_STATIC_CONSTANT(bool, value = (less< U,mpl::int_t<N> >::value));
    };
};

// limited forms of ET, just for now
template<>
struct less<
      mpl::_1
    , mpl::_2
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename less<U1,U2>::type type;
    };
};

template<>
struct less< 
      mpl::_2
    , mpl::_1
    >
{
    template<typename U1, typename U2> struct apply
    {
        typedef typename less<U2,U1>::type type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COMPARISON_LESS_HPP

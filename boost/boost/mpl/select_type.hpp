//-----------------------------------------------------------------------------
// boost mpl/select_type.hpp header file
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

// (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_MPL_SELECT_TYPE_HPP
#define BOOST_MPL_SELECT_TYPE_HPP

#include "boost/config.hpp"

namespace boost {
namespace mpl {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<bool Condition, typename T1, typename T2>
struct select_type
{
    typedef T1 type;
};

template<typename T1, typename T2>
struct select_type<false, T1, T2>
{
    typedef T2 type;
};

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template<bool Condition>
struct select_type_part1
{
    template<typename T1, typename T2> struct part2
    {
        typedef T1 type;
    };
};

template<>
struct select_type_part1<false>
{
    template<typename T1, typename T2> struct part2
    { 
        typedef T2 type;
    };
};

} // namespace detail

template<bool Condition, typename T1, typename T2>
struct select_type
{
    typedef typename mpl::detail::select_type_part1<Condition>
                ::template part2<T1, T2>::type type;
};

#endif // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template<class Value>
struct value_wrapper : Value
{
};

} // namespace detail

// used through the library as the main workaround of Borland's integral 
// constant expressions bugs
template<class Condition, typename T1, typename T2>
struct select_type_t
{
    typedef typename select_type< 
          ::boost::mpl::detail::value_wrapper<Condition>::value
        , T1
        , T2
        >::type type;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_SELECT_TYPE_HPP

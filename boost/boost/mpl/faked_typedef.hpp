//-----------------------------------------------------------------------------
// boost mpl/faked_typedef.hpp header file
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

#ifndef BOOST_MPL_FAKED_TYPEDEF_HPP
#define BOOST_MPL_FAKED_TYPEDEF_HPP

namespace boost {
namespace mpl {

#define BOOST_MPL_DEFINE_FAKED_TYPEDEF(typedef_name)                          \
    namespace detail {                                                        \
    template<typename T> struct faked_typedef_##typedef_name                  \
    {                                                                         \
        typedef T typedef_name;                                               \
    };                                                                        \
    }                                                                         \
/**/

BOOST_MPL_DEFINE_FAKED_TYPEDEF(type)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(next)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(body)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(loop)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(iterator)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(function)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(executed)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(sequence)
BOOST_MPL_DEFINE_FAKED_TYPEDEF(type_tag)

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FAKED_TYPEDEF_HPP

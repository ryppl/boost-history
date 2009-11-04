// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_UTIL_RANGE_ITERATOR_CONST_IF_C_HPP
#define GGL_UTIL_RANGE_ITERATOR_CONST_IF_C_HPP


#include <boost/mpl/if.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>


namespace ggl
{


/*!
    \brief Meta-function to define a const or non const boost range iterator
    \ingroup utility
    \details Is used to have one implementation for both const and non const
        range iterators
    \note This traits class is completely independant from GGL and might be a
        separate addition to Boost
    \note Used in ggl::for_each
*/
template <bool IsConst, typename Range>
struct range_iterator_const_if_c
{
    typedef typename boost::mpl::if_c
        <
            IsConst,
            typename boost::range_const_iterator<Range>::type,
            typename boost::range_iterator<Range>::type
        >::type type;
};


} // namespace ggl


#endif // GGL_UTIL_RANGE_ITERATOR_CONST_IF_C_HPP

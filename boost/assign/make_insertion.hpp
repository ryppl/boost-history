/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_MAKE_ITERATOR_INSERTION_HPP
#define BOOST_MAKE_ITERATOR_INSERTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost
{
namespace assignment
{

#if BOOST_NO_FUNCTION_TEMPLATE_ORDERING

// do nothing: a default version ***** up vc6 

#else 

    template< typename C, typename V >
    inline void make_insertion( C& c, const V& v )
    {
        c.insert( c.end(), v );
    }

#endif

} // namespace 'assignment'
} // namespace 'boost'

#endif

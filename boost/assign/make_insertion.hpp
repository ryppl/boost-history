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

namespace boost
{
namespace assignment
{

#if defined( _MSC_VER ) && !defined( __COMO__ )

// do nothing: a default version f*** up vc6 

#else 

    template< typename C, typename V >
    inline void make_iterator_insertion( C& c, const V& v )
    {
        c.insert( c.end(), v );
    }

#endif

} // namespace 'assignment'
} // namespace 'boost'

#endif

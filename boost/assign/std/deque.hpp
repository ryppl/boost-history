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

#ifndef BOOST_ASSIGN_STD_DEQUE_HPP
#define BOOST_ASSIGN_STD_DEQUE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/make_insertion.hpp>
#include <boost/config.hpp>
#include <deque>

namespace boost
{
namespace assignment
{

    template< typename V, typename A >
    inline void make_insertion( std::deque<V,A>& c, const V& v )
    {
        c.push_back( v );
    }

} // namespace 'assignment'
} // namespace 'boost'

#include <boost/assign/insert_assigner.hpp>

namespace boost
{
namespace assignment
{

    template< typename V, typename A >
    inline insert_assigner< std::deque<V,A> > operator+=( std::deque<V,A>& c, const V& v )
    {
        return insert_assigner< std::deque<V,A> >( c ),v;
    }
}
}

#endif

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

#ifndef BOOST_ASSIGN_STD_STACK_HPP
#define BOOST_ASSIGN_STD_STACK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/make_insertion.hpp>
#include <boost/config.hpp>
#include <stack>

namespace boost
{
namespace assignment
{                   

    template< typename V, typename C, typename V2 >
    inline void make_insertion( std::stack<V,C>& c, V2 v )
    {
        c.push( v );
    }

} // namespace 'assignment'
} // namespace 'boost'

#include <boost/assign/insert_assigner.hpp>

namespace boost
{
namespace assignment
{

    template< typename V, typename C, typename V2 >
    inline insert_assigner< std::stack<V,C> > operator+=( std::stack<V,C>& c, V2 v )
    {
        return insert_assigner< std::stack<V,C> >( c ),v;
    }
    
    template< typename V, typename C >
    inline insert_assigner< std::stack<V,C> > append( std::stack<V,C>& c )
    {
        return insert_assigner< std::stack<V,C> >( c );
    }

}
}

#endif

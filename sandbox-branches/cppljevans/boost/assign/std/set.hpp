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

#ifndef BOOST_ASSIGN_STD_SET_HPP
#define BOOST_ASSIGN_STD_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/make_insertion.hpp>
#include <boost/config.hpp>
#include <set>

namespace boost
{
namespace assignment
{

    template< typename K, typename C, typename A, typename K2 >
    inline void make_insertion( std::set<K,C,A>& c, K2 k )
    {
        c.insert( k );
    }

    template< typename K, typename C, typename A, typename K2 >
    inline void make_insertion( std::multiset<K,C,A>& c, K2 k )
    {
        c.insert( k );
    }

} // namespace 'assignment'
} // namespace 'boost'

#include <boost/assign/insert_assigner.hpp>

namespace boost
{
namespace assignment
{
    template< typename K, typename C, typename A, typename K2 >
    inline insert_assigner< std::set<K,C,A> > 
    operator+=( std::set<K,C,A>& c, K2 k )
    {
        return insert_assigner< std::set<K,C,A> >( c ),k;
    }

    template< typename K, typename C, typename A, typename K2 >
    inline insert_assigner< std::multiset<K,C,A> > 
    operator+=( std::multiset<K,C,A>& c, K2 k )
    {
        return insert_assigner< std::multiset<K,C,A> >( c ),k;
    }

    template< typename K, typename C, typename A >
    inline insert_assigner< std::set<K,C,A> > 
    insert( std::set<K,C,A>& c )
    {
        return insert_assigner< std::set<K,C,A> >( c );
    }
    
    template< typename K, typename C, typename A >
    inline insert_assigner< std::multiset<K,C,A> > 
    insert( std::multiset<K,C,A>& c )
    {
        return insert_assigner< std::multiset<K,C,A> >( c );
    }

}
}

#endif

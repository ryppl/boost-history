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

#ifndef BOOST_ASSIGN_STD_MAP_HPP
#define BOOST_ASSIGN_STD_MAP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/make_insertion.hpp>
#include <boost/config.hpp>
#include <map>

namespace boost
{
namespace assignment
{

    template< typename K, typename V, typename C, typename A, typename P >
    inline void make_insertion( std::map<K,V,C,A>& c, const P& p )
    {
        c.insert( p );
    }

    template< typename K, typename V, typename C, typename A, typename P >
    inline void make_insertion( std::multimap<K,V,C,A>& c, const P& p )
    {
        c.insert( p );
    }

} // namespace 'assignment'
} // namespace 'boost'

#include <boost/assign/insert_assigner.hpp>
#include <boost/assign/glist.hpp>

#endif

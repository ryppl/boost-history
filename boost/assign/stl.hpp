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

#ifndef BOOST_ASSIGN_STL_HPP
#define BOOST_ASSIGN_STL_HPP

#include "make_insertion.hpp"
#include "fixed_size_assigner.hpp"
#include "insert_assigner.hpp"
#include <boost/config.hpp>

#include <stack>
#include <queue>

#if BOOST_NO_FUNCTION_TEMPLATE_ORDERING

#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

#endif

namespace boost
{
namespace assignment
{

    template< typename V, typename C >
    inline void make_iterator_insertion( std::stack<V,C>& c, const V& v )
    {
        c.push( v );
    }



    template< typename V, typename C >
    inline void make_iterator_insertion( std::queue<V,C>& c, const V& v )
    {
        c.push( v );
    }



    template< typename V, typename C, typename Comp >
    inline void make_iterator_insertion( std::priority_queue<V,C,Comp>& c, const V& v )
    {
        c.push( v );
    }

#if BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    //
    // We can circumvent vc's missing support of partial ordering
    // by making _no_ default template function. We have to write
    // all overloads explicitly :(.
    //
    
    template< typename V, typename A >
    inline void make_iterator_insertion( std::deque<V,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }



    template< typename V, typename A >
    inline void make_iterator_insertion( std::list<V,A>& c, const V& v )
    {
        c.insert( c.end(), v ); 
    }



    template< typename K, typename V, typename Comp, typename A >
    inline void 
    make_iterator_insertion( std::map<K,V,Comp,A>& c, 
                             const typename std::map<K,V,Comp,A>::value_type& v )
    {
        c.insert( c.end(), v );
    }



    template< typename K, typename V, typename Comp, typename A >
    inline void 
    make_iterator_insertion( std::multimap<K,V,Comp,A>& c,
                             const typename std::map<K,V,Comp,A>::value_type& v )
    {
        c.insert( c.end(), v );
    }



    template< typename V, typename Comp, typename A >
    inline void make_iterator_insertion( std::set<V,Comp,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }



    template< typename V, typename Comp, typename A >
    inline void make_iterator_insertion( std::multiset<V,Comp,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }



    template< typename V, typename A >
    inline void make_iterator_insertion( std::vector<V,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }

#endif

} // namespace 'assignment'
} // namespace 'boost'

#endif

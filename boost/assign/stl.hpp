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

#include "fixed_size_assigner.hpp"
#include "insert_assigner.hpp"
#include <boost/config.hpp>

#include <stack>
#include <queue>

#if defined( _MSC_VER ) && !defined( __COMO__ )

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
namespace detail
{

#if defined( _MSC_VER ) && !defined( __COMO__ )

//     template< typename C, typename V >
//     inline void insert( C& c, const V& v )
//     {
//  	c.insert( c.end(), v );
//     }

#else 

    template< typename C, typename V >
    inline void insert( C& c, const V& v )
    {
        c.insert( c.end(), v );
    }


#endif
	
    template< typename V, typename C >
    inline void insert( std::stack<V,C>& c, const V& v )
    {
        c.push( v );
    }



    template< typename V, typename C >
    inline void insert( std::queue<V,C>& c, const V& v )
    {
        c.push( v );
    }
    
    
    
    template< typename V, typename C, typename Comp >
    inline void insert( std::priority_queue<V,C,Comp>& c, const V& v )
    {
        c.push( v );
    }

#if defined( _MSC_VER ) && !defined( __COMO__ )

	//
	// We can circumvent vc's missing support of partial ordering
	// by making _no_ default template function. We have to write
	// all overloads explicitly :(.
	//
#if 1
    template< typename V, typename A >
    inline void insert( std::deque<V,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }
    


    template< typename V, typename A >
    inline void insert( std::list<V,A>& c, const V& v )
    {
        c.insert( c.end(), v ); 
    }


    
    template< typename K, typename V, typename Comp, typename A >
    inline void 
    insert( std::map<K,V,Comp,A>& c, 
	    const typename std::map<K,V,Comp,A>::value_type& v )
    {
        c.insert( c.end(), v );
    }


    
    template< typename K, typename V, typename Comp, typename A >
    inline void 
    insert( std::multimap<K,V,Comp,A>& c,
	    const typename std::map<K,V,Comp,A>::value_type& v )
    {
        c.insert( c.end(), v );
    }


    
    template< typename V, typename Comp, typename A >
    inline void insert( std::set<V,Comp,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }


    
    template< typename V, typename Comp, typename A >
    inline void insert( std::multiset<V,Comp,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }


    
    template< typename V, typename A >
    inline void insert( std::vector<V,A>& c, const V& v )
    {
        c.insert( c.end(), v );
    }

#endif

#endif // defined( _MSC_VER )

 
    } // namespace 'detail'



    template< typename C > 
    inline detail::insert_assigner<C> 
    operator+=( C& c, const typename C::value_type& v )
    {
        return detail::insert_assigner<C>( c, v );
    }
    


    template< typename C >
    inline detail::insert_assigner<C> assign( C& c )
    {
        return detail::insert_assigner<C>( c );
    }

    
    template< typename C >
    inline detail::fixed_size_assigner<typename C::value_type, typename C::iterator>
    assign_all( C& c )
    {
        return detail::fixed_size_assigner<typename C::value_type, 
            typename C::iterator>( c.begin(), c.end() );
    }
    
    // safe_assign( C )() boundschecked
        
    } // namespace 'assignment'
} // namespace 'boost'
#endif

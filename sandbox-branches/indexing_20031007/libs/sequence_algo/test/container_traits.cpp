/**
 * short: test of container traits.
 */
// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include "boost/sequence_algo/container_traits.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <deque>
#include <list>
#include <slist>
#include <string>

template< typename C >
bool is_container()
{
    return boost::container_traits<C>::is_container();
}

template< typename C >
bool is_sequence()
{
    return boost::container_traits<C>::is_sequence();
}

template< typename C >
bool is_contiguous_sequence()
{
    return boost::container_traits<C>::is_contiguous_sequence();
}

template< typename C >
bool is_associative()
{
    return boost::container_traits<C>::is_associative();
}

template< typename C >
bool is_basic_string()
{
    return boost::container_traits<C>::is_basic_string();
}

template< typename C >
bool is_vector()
{
    return boost::container_traits<C>::is_vector();
}

template< typename C >
bool is_deque()
{
    return boost::container_traits<C>::is_deque();
}

template< typename C >
bool is_list()
{
    return boost::container_traits<C>::is_list();
}

template< typename C >
bool is_slist()
{
    return boost::container_traits<C>::is_slist();
}

template< typename C >
bool  is_rope()
{
    return boost::container_traits<C>::is_rope();
}

int main()
{
    using namespace boost;
    using namespace std;
    typedef string        string_t;
    typedef vector<int>   vector_t;
    typedef deque<int>    deque_t;
    typedef list<int>     list_t;
    typedef slist<int>    slist_t;
    typedef map<int,int>  map_t;

    assert( is_container<vector_t>() );
    
    assert( is_sequence<vector_t>() );
    assert( is_sequence<list_t>() );
    assert( !is_sequence<map_t>() );

    assert( is_contiguous_sequence<vector_t>() );
    assert( !is_contiguous_sequence<list_t>() );
    assert( !is_contiguous_sequence<map_t>() );

    assert( is_associative<map_t>() );
    assert( !is_associative<vector_t>() );
    assert( !is_associative<list_t>() );

}

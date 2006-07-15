// Boost.Bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

// std
#include <set>
#include <map>
#include <cstddef>
#include <cassert>
#include <algorithm>

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.Bimap
#include <boost/bimap/detail/manage_bimap_key.hpp>
#include <boost/bimap/detail/manage_additional_parameters.hpp>
#include <boost/bimap/detail/get_value_type.hpp>
#include <boost/bimap/detail/bimap_core.hpp>
#include <boost/bimap/detail/core_iterator_type_of.hpp>

#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/support/data_type_of.hpp>
#include <boost/bimap/support/value_type_by.hpp>

#include <boost/bimap/views/map_view.hpp>
#include <boost/bimap/views/multimap_view.hpp>
#include <boost/bimap/views/unordered_map_view.hpp>
#include <boost/bimap/views/unordered_multimap_view.hpp>

#include <boost/bimap/bimap.hpp>

template< class BimapType, class IterData >
void test_data_insertion(BimapType & b, IterData start, IterData final)
{
    b.insert(start,final);
}


template< class Container, class Data >
void test_container(Container & c, const Data & d)
{
    std::size_t dn = d.size();
    assert( dn > 2 );

    c.clear();

    BOOST_CHECK( c.size() == 0 );
    BOOST_CHECK( c.empty() );

    c.insert(d.begin(),d.end());

    BOOST_CHECK( c.size() == dn );

    BOOST_CHECK( c.size() <= c.max_size() );
    BOOST_CHECK( ! c.empty() );

    c.erase( c.begin() );

    BOOST_CHECK( c.size() == dn - 1 );

    c.insert( *d.begin() );

    BOOST_CHECK( c.size() == dn );

    c.erase( c.begin(), c.end() );

    BOOST_CHECK( c.empty() );

    c.insert( *d.begin() );

    BOOST_CHECK( c.size() == 1 );

    c.insert( c.begin(), *(++d.begin()) );

    BOOST_CHECK( c.size() == 2 );

    BOOST_CHECK( c.begin() != c.end() );

}

template< class Container, class Data >
void test_pair_associative_container(Container & c, const Data & d)
{
    std::size_t dn = d.size();
    assert( dn > 2 );

    c.clear();
    c.insert(d.begin(),d.end());

    for( typename Data::const_iterator di = d.begin(), de = d.end();
         di != de; ++di )
    {
        BOOST_CHECK( c.find(di->first) != c.end() );
    }

    typename Data::const_iterator da =   d.begin();
    typename Data::const_iterator db = ++d.begin();

    c.erase(da->first);

    BOOST_CHECK( c.size() == dn-1 );

    BOOST_CHECK( c.count(da->first) == 0 );
    BOOST_CHECK( c.count(db->first) == 1 );

    BOOST_CHECK( c.find(da->first) == c.end() );
    BOOST_CHECK( c.find(db->first) != c.end() );

    BOOST_CHECK( c.equal_range(db->first).first != c.end() );

    c.clear();

    BOOST_CHECK( c.equal_range(da->first).first == c.end() );
}

template< class Container, class Data >
void test_pair_ordered_associative_container_equality(Container & c, const Data & d)
{
    BOOST_CHECK( std::equal( c. begin(), c. end(), d. begin() ) );

    // TODO
    // Reverse iterators still need to be worked out
    // BOOST_CHECK( std::equal( c.rbegin(), c.rend(), d.rbegin() ) );

    BOOST_CHECK( c.lower_bound( d.begin()->first ) ==   c.begin() );
    BOOST_CHECK( c.upper_bound( d.begin()->first ) == ++c.begin() );
}

template< class Container, class Data >
void test_pair_ordered_associative_container(Container & c, const Data & d)
{
    std::size_t dn = d.size();
    assert( dn > 2 );

    c.clear();
    c.insert(d.begin(),d.end());

    for( typename Container::const_iterator ci = c.begin(), ce = c.end();
         ci != ce; ++ci )
    {
        typename Data::const_iterator di = d.find(ci->first);
        BOOST_CHECK( di != d.end() );
        BOOST_CHECK( ! c.key_comp()(di->first,ci->first) );

        // TODO
        // value_comp() need to be reworked to return a wrapper around the one
        // given by multi_index
        // BOOST_CHECK( c.value_comp()(*ci,*di) );
    }

    test_pair_ordered_associative_container_equality(c, d);

    const Container & cr = c;

    test_pair_ordered_associative_container_equality(cr, d);

}





/*
template< class bm, class IterData >
void test_this_unknown_bimap(bm & b, IterData start, IterData final)
{
    b.clear();

    BOOST_CHECK( b.empty() )
    BOOST_CHECK( b.size() == 0 );

    b.insert(start,final);

    BOOST_CHECK( b.size() == count(start,final) )
    BOOST_CHECK( b.max_size() >= b.size() );
    BOOST_CHECK( ! b.empty() )

    b.clear();
    for( IterData iter = start; iter != final; ++iter )
    {
        b.insert( *iter );
    }

    BOOST_CHECK( b.size() == count(start,final) )
    BOOST_CHECK( ! b.empty() )

    iterator find(const key_type& k)

    size_type count(const key_type& k)
    
    const_iterator find(const key_type& k) const

    iterator insert(iterator pos, const value_type& x)

    void erase(iterator pos)

    size_type erase(const key_type& k)

    void erase(iterator first, iterator last)



    std::pair<iterator, iterator> equal_range(const key_type& k)
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const

}

template< class BimapType, class IterData >
void test_this_ordered_bimap(BimapType & bm, IterData start, IterData final)
{
    iterator begin()

    iterator end()

    const_iterator begin() const

    const_iterator end() const

    reverse_iterator rbegin()

    reverse_iterator rend()

    const_reverse_iterator rbegin() const

    const_reverse_iterator rend() const

    key_compare key_comp() const

    value_compare value_comp() const

    iterator lower_bound(const key_type& k)

    const_iterator lower_bound(const key_type& k) const

    iterator upper_bound(const key_type& k)

    const_iterator upper_bound(const key_type& k) const


}

template< class BimapType, class IterData >
void test_this_unordered_bimap(BimapType & bm, IterData start, IterData final)
{

    size_type bucket_count() const

    size_type max_bucket_count() const

    size_type bucket_size(size_type n) const

    size_type bucket(const key_type& k) const

    local_iterator       begin(size_type n)

    const_local_iterator begin(size_type n) const

    local_iterator       end(size_type n)

    const_local_iterator end(size_type n) const

    float load_factor() const

    float max_load_factor() const

    void max_load_factor(float z)

    void rehash(size_type n)

    BOOST_CHECK( it != d.end() );
    BOOST_CHECK( it->second == "rose" );

}
*/
void test_bimap()
{
    using namespace boost::bimap;

    // Easy first test, two POD data types with out custom configuration
    {
        typedef bimap<int,double> bm;


        std::set< bm::relation > data;
        data.insert( bm::relation(1,0.1) );
        data.insert( bm::relation(2,0.2) );
        data.insert( bm::relation(3,0.3) );
        data.insert( bm::relation(4,0.4) );

        typedef std::map<int,double> left_data_type;
        left_data_type left_data;
        left_data.insert( left_data_type::value_type(1,0.1) );
        left_data.insert( left_data_type::value_type(2,0.2) );
        left_data.insert( left_data_type::value_type(3,0.3) );
        left_data.insert( left_data_type::value_type(4,0.4) );

        typedef std::map<double,int> right_data_type;
        right_data_type right_data;
        right_data.insert( right_data_type::value_type(0.1,1) );
        right_data.insert( right_data_type::value_type(0.2,2) );
        right_data.insert( right_data_type::value_type(0.3,3) );
        right_data.insert( right_data_type::value_type(0.4,4) );

/*
        std::list< bm::relation::right_pair > right_data;
        data.push_back( bm::relation::right_pair(1,0.1) );
        data.push_back( bm::relation::right_pair(2,0.2) );
        data.push_back( bm::relation::right_pair(3,0.3) );
        data.push_back( bm::relation::right_pair(4,0.4) );
*/
        bm b;

        test_container(b,data);

        // TODO
        // A value convertion is needed here
        // test_associative_container(b,data);

        test_container(b.left , left_data);
        test_pair_associative_container(b.left, left_data);
        test_pair_ordered_associative_container(b.left, left_data);

        test_container(b.right,right_data);
        test_pair_associative_container(b.right, right_data);
        test_pair_ordered_associative_container(b.right, right_data);

/*
        test_this_unknown_bimap(aBimap,data.begin(),data.end());
        test_this_ordered_bimap(aBimap,data.begin(),data.end());
*/
    }

}


int test_main( int, char* [] )
{
    test_bimap();
    return 0;
}


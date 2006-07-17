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
#include <boost/bimap/collection/multiset_of.hpp>
#include <boost/bimap/collection/unordered_set_of.hpp>
#include <boost/bimap/collection/unordered_multiset_of.hpp>

template< class Container, class Data >
void test_container(Container & c, const Data & d)
{
    assert( d.size() > 2 );

    c.clear();

    BOOST_CHECK( c.size() == 0 );
    BOOST_CHECK( c.empty() );

    c.insert( *d.begin());
    c.insert(++d.begin(),d.end());

    BOOST_CHECK( c.size() == d.size() );

    BOOST_CHECK( c.size() <= c.max_size() );
    BOOST_CHECK( ! c.empty() );

    c.erase( c.begin() );

    BOOST_CHECK( c.size() == d.size() - 1 );

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
    assert( d.size() > 2 );

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

    BOOST_CHECK( c.size() == d.size()-1 );

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
    assert( d.size() > 2 );

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


template< class Container, class Data >
void test_pair_unordered_associative_container(Container & c, const Data & d)
{
    c.clear();
    c.insert( d.begin(), d.end() );

    BOOST_CHECK( c.bucket_count() * c.max_load_factor() >= d.size() );
    BOOST_CHECK( c.max_bucket_count() >= c.bucket_count() );

    for( typename Data::const_iterator di = d.begin(), de = d.end() ;
         di != de ; ++di )
    {
        // non const
        {
            typename Container::size_type nb = c.bucket(c.find(di->first)->first);
            BOOST_CHECK( c.begin(nb) != c.end(nb) );
        }

        // const
        {
            const Container & const_c = c;

            BOOST_CHECK( const_c.bucket_size(const_c.bucket(di->first)) == 1 );

            typename Container::size_type nb = const_c.bucket(const_c.find(di->first)->first);
            BOOST_CHECK( const_c.begin(nb) != const_c.end(nb) );
        }
    }


    BOOST_CHECK( c.load_factor() < c.max_load_factor() );

    c.max_load_factor(0.75);

    BOOST_CHECK( c.max_load_factor() == 0.75 );

    c.rehash(10);
}


template< class Container, class Data >
void test_unique_container(Container & c, Data & d)
{
    c.clear();
    c.insert(d.begin(),d.end());
    c.insert(*d.begin());
    BOOST_CHECK( c.size() == d.size() );
}

template< class Container, class Data >
void test_non_unique_container(Container & c, Data & d)
{
    c.clear();
    c.insert(d.begin(),d.end());
    c.insert(*d.begin());
    BOOST_CHECK( c.size() == (d.size()+1) );
}


template< class Bimap, class Data, class LeftData, class RightData >
void test_basic_bimap(Bimap & b,
                      const Data & d,
                      const LeftData & ld, const RightData & rd)
{
    using namespace boost::bimap;

    test_container(b,d);

    BOOST_CHECK( &b.left  == &map_by<member_at::left >(b) );
    BOOST_CHECK( &b.right == &map_by<member_at::right>(b) );

    test_container(b.left , ld);
    test_container(b.right, rd);
}

template< class LeftTag, class RightTag, class Bimap, class Data >
void test_tagged_bimap(Bimap & b,
                       const Data & d)
{
    using namespace boost::bimap;

    BOOST_CHECK( &b.left  == &map_by<LeftTag >(b) );
    BOOST_CHECK( &b.right == &map_by<RightTag>(b) );

    b.clear();
    b.insert( *d.begin() );

    BOOST_CHECK( pair_by<LeftTag >( *b.begin() ) == *map_by<LeftTag >(b).begin() );
    BOOST_CHECK( pair_by<RightTag>( *b.begin() ) == *map_by<RightTag>(b).begin() );

    BOOST_CHECK( get<LeftTag >(*b.begin()) == get<LeftTag >(*map_by<RightTag>(b).begin()));
    BOOST_CHECK( get<RightTag>(*b.begin()) == get<RightTag>(*map_by<LeftTag >(b).begin()));
}


template< class Bimap, class Data, class LeftData, class RightData >
void test_set_set_bimap(Bimap & b,
                        const Data & d,
                        const LeftData & ld, const RightData & rd)
{
    using namespace boost::bimap;

    test_basic_bimap(b,d,ld,rd);

    test_pair_associative_container(b.left, ld);
    test_pair_ordered_associative_container(b.left, ld);
    test_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_ordered_associative_container(b.right, rd);
    test_unique_container(b.right, rd);
}


template< class Bimap, class Data, class LeftData, class RightData >
void test_multiset_multiset_bimap(Bimap & b,
                                  const Data & d,
                                  const LeftData & ld, const RightData & rd)
{
    using namespace boost::bimap;

    test_basic_bimap(b,d,ld,rd);

    test_pair_associative_container(b.left, ld);
    test_pair_ordered_associative_container(b.left, ld);
    test_non_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_ordered_associative_container(b.right, rd);
    test_non_unique_container(b.right, rd);
}

template< class Bimap, class Data, class LeftData, class RightData >
void test_unordered_set_unordered_multiset_bimap(Bimap & b,
                                                 const Data & d,
                                                 const LeftData & ld,
                                                 const RightData & rd)
{
    using namespace boost::bimap;

    test_basic_bimap(b,d,ld,rd);

    test_pair_associative_container(b.left, ld);
    test_pair_unordered_associative_container(b.left, ld);
    test_unique_container(b.left, ld);

    test_pair_associative_container(b.right, rd);
    test_pair_unordered_associative_container(b.right, rd);

    // Caution, this side is a non unique container, but the other side is a
    // unique container so, the overall bimap is a unique one.
    test_unique_container(b.right, rd);
}

struct  left_tag {};
struct right_tag {};

void test_bimap()
{
    using namespace boost::bimap;

    // Easy first test, two POD data types with out custom configuration
    {
        typedef std::pair<int,double> std_pair;

        std::set< std_pair > data;
        data.insert( std_pair(1,0.1) );
        data.insert( std_pair(2,0.2) );
        data.insert( std_pair(3,0.3) );
        data.insert( std_pair(4,0.4) );

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

        // Untagged simple bimap
        {
            typedef bimap<int,double> bm;

            bm b;

            test_set_set_bimap(b,data,left_data,right_data);
        }

        // Tagged simple bimap
        {
            typedef bimap< tagged<int,left_tag>, tagged<double,right_tag> > bm;

            bm b;

            test_set_set_bimap(b,data,left_data,right_data);
            test_tagged_bimap<left_tag,right_tag>(b,data);

        }

        // Untagged non-basic bimap
        {
            typedef bimap< multiset_of<int>, multiset_of<double> > bm;

            bm b;

            test_multiset_multiset_bimap(b,data,left_data,right_data);
        }

        // Tagged non-basic bimap
        {
            typedef bimap
            <
                tagged< multiset_of<int>   , left_tag  >,
                tagged< multiset_of<double>, right_tag >

            > bm;

            bm b;

            test_multiset_multiset_bimap(b,data,left_data,right_data);
            test_tagged_bimap<left_tag,right_tag>(b,data);
        }

        // Untagged non-basic bimap
        {
            typedef bimap< unordered_set_of<int>, unordered_multiset_of<double> > bm;

            bm b;

            test_unordered_set_unordered_multiset_bimap(b,data,left_data,right_data);
        }

        // Tagged non-basic bimap
        {
            typedef bimap
            <
                tagged< unordered_set_of<int>        , left_tag  >,
                tagged< unordered_multiset_of<double>, right_tag >

            > bm;

            bm b;

            test_unordered_set_unordered_multiset_bimap(b,data,left_data,right_data);
            test_tagged_bimap<left_tag,right_tag>(b,data);
        }
    }


}


int test_main( int, char* [] )
{
    test_bimap();
    return 0;
}


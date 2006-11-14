// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_BIMAP_DISABLE_SERIALIZATION

// Boost.Test
#include <boost/test/minimal.hpp>

// std
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <functional>

// Boost
#include <boost/assign/list_of.hpp>

// Set type specifications
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/vector_of.hpp>

// bimap container
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>

#include <libs/bimap/test/test_bimap.hpp>

struct  left_tag {};
struct right_tag {};

void test_bimap()
{
    using namespace boost::bimap;

    typedef std::pair<int,double> std_pair;
    std::set< std_pair > data =
        boost::assign::list_of< std_pair >
        (1,0.1) (2,0.2) (3,0.3) (4,0.4);

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


    //--------------------------------------------------------------------
    {
        bimap
        <
            list_of< int >, vector_of< double >

        > b, c;

        test_sequence_container(b,data);
        test_sequence_container(b.left , left_data);
        test_sequence_container(b.right,right_data);

        // Test splice and merge

        b.clear();

        c.left.insert(c.left.begin(),left_data.begin(),left_data.end());
        b.left.splice(b.left.begin(),c.left);

        BOOST_CHECK( c.size() == 0 );
        BOOST_CHECK( b.size() == 4 );

        c.left.splice(c.left.begin(),b.left,++b.left.begin());

        BOOST_CHECK( c.size() == 1 );

        c.splice(c.begin(),b,b.begin(),b.end());

        BOOST_CHECK( b.size() == 0 );

        b.left.merge(c.left);
        c.left.merge(b.left,std::less<int>());

        b.left.sort();
        b.left.sort(std::less<int>());

        b.left.unique();
        b.left.unique(std::equal_to<int>());

        c.sort();
        c.unique();
        b.merge(c);
        b.reverse();

        b.left.remove_if( _key < 3 );

        // Test splice and merge

        b.clear(); c.clear();

        c.left.insert(c.left.begin(),left_data.begin(),left_data.end());
        b.right.splice(b.right.begin(),c.right);

        BOOST_CHECK( c.size() == 0 );
        BOOST_CHECK( b.size() == 4 );

        c.right.splice(c.right.begin(),b.right,++b.right.begin());

        b.right.merge(c.right);
        c.right.merge(b.right,std::less<double>());

        b.right.sort();
        b.right.sort(std::less<double>());

        b.right.unique();
        b.right.unique(std::equal_to<double>());

        b.right.remove_if( _key < 0.3 );

        b.clear();
        b.left.insert(b.left.begin(),left_data.begin(),left_data.end());

        b.left.relocate(b.left.begin(), ++b.left.begin() );
        b.left.relocate(b.left.end(), b.left.begin(), ++b.left.begin() );

        b.right.relocate(b.right.begin(), ++b.right.begin() );
        b.right.relocate(b.right.end(), b.right.begin(), ++b.right.begin() );

        b.relocate(b.begin(), ++b.begin() );
        b.relocate(b.end(), b.begin(), ++b.begin() );
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        typedef bimap
        <
            list_of< short >, list_of< std::string >,
            vector_of_relation

        > bm;

        bm b;
        b.push_back( bm::relation(1,"one"  ) );
        b.push_back( bm::relation(2,"two"  ) );
        b.push_back( bm::relation(3,"three") );
        b.push_back( bm::relation(4,"four" ) );

        b.sort();
        b.unique();
        b.reverse();

        b.relocate( b.begin(), ++b.begin() );
        b.relocate( b.end(), b.begin(), ++b.begin() );
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        typedef bimap
        <
            vector_of< short >, list_of< short >,
            vector_of_relation

        > bimap_type;

        bimap_type b1;
        bimap_type b2;

        BOOST_CHECK(     b1 == b2   );
        BOOST_CHECK( ! ( b1 != b2 ) );
        BOOST_CHECK(     b1 <= b2   );
        BOOST_CHECK(     b1 >= b2   );
        BOOST_CHECK( ! ( b1 <  b2 ) );
        BOOST_CHECK( ! ( b1 >  b2 ) );

        b1.push_back( bimap_type::relation(1,2) );

        b2 = b1;
        BOOST_CHECK( b2 == b1 );

        b2.left = b1.left;
        BOOST_CHECK( b2 == b1 );

        b2.right = b1.right;
        BOOST_CHECK( b2 == b1 );
    }
    //--------------------------------------------------------------------

}


int test_main( int, char* [] )
{
    test_bimap();
    return 0;
}


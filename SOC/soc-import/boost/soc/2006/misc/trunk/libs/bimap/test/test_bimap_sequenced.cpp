// Boost.Bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

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

        > b;

        test_sequence_container(b,data);
        test_sequence_container(b.left , left_data);
        test_sequence_container(b.right,right_data);
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        bimap
        <
            vector_of< short >, list_of< std::string >,
            list_of_relation

        > b;
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


///////////////////////////////////////////////////////////////////////////////
// contingency_table1.cpp                                                    //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/detail/wrapper.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/container/map/detail/sequence_to_map.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/include/factor.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/include/cells.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/include/pearson_chisq/independence.hpp>

#include <boost/statistics/detail/non_parametric/contingency_table/pearson_chisq/independence/tag.hpp>

void test_contingency_table1()
{
    namespace ac = boost::accumulators;
	namespace ct = boost::statistics::detail::contingency_table;
    namespace ps = ct::pearson_chi_square_statistic;

    typedef double val_;
    typedef boost::mpl::int_<0> x_; typedef int data_x_;
    typedef boost::mpl::int_<1> y_; typedef std::string data_y_;
    typedef boost::mpl::int_<2> z_; typedef int data_z_;
 
    typedef boost::fusion::detail::sequence_to_map<
        boost::mpl::vector6<x_,data_x_,y_,data_y_,z_,data_z_>
    >::type sample_;
    
    typedef boost::mpl::vector2<x_,y_> keys_;
    typedef ps::tag::independence_between<keys_> hypothesis_;
    typedef ac::stats< hypothesis_> stats_;
    typedef ac::accumulator_set< sample_, stats_, long int > acc_;
        
    using namespace boost::assign;
    acc_ acc(( ct::_map_of_levels = boost::fusion::make_map<x_,y_>(
        list_of(-1)(1), list_of("a")("b")("c") ) ));
    
    {   
        // Check levels 
     	BOOST_CHECK( ct::cells_count<keys_>( acc ) == 2 * 3 );
     	BOOST_CHECK( ct::extract::levels<x_>( acc ).count( -1  ) == 1 );
     	BOOST_CHECK( ct::extract::levels<x_>( acc ).count(  1  ) == 1 );
     	BOOST_CHECK( ct::extract::levels<x_>( acc ).count(  2  ) == 0 );
     	BOOST_CHECK( ct::extract::levels<y_>( acc ).count( "a" ) == 1 );
     	BOOST_CHECK( ct::extract::levels<y_>( acc ).count( "b" ) == 1 );
     	BOOST_CHECK( ct::extract::levels<y_>( acc ).count( "c" ) == 1 );
     	BOOST_CHECK( ct::extract::levels<y_>( acc ).count( "d" ) == 0 );
    }
    {                                                                        //.....|.....|.....|.....|.....|.....|.....|
                                                                             //     |non- |    x =    |       y =       |
                                                                             //total|empty|...........|.................|
        // Filling the cells                                                 //count|cells| -1  |  1  | "a" | "b" | "c" |
        using namespace ac;                                 //.....|count|.....|.....|.....|.....|.....|
        acc( boost::fusion::make_map<x_,y_,z_>( -1, "a", 1 ), weight = 1 );  //  1  |  1  |  1  |  0  |  1  |  0  |  0  |
        acc( boost::fusion::make_map<x_,y_,z_>(  1, "b", 1 ), weight = 2 );  //  3  |  2  |  1  |  2  |  1  |  2  |  0  |
    }
    {   // Check degrees of freedom
        // lost df = (r-1) + (c-1) = 1 + 2 = 3;
        //  df = rc -r -c + 1 = 3 * 2 - 3  -2 + 1 = 2 
        
        typedef boost::mpl::detail::wrapper<hypothesis_> h0_;
        BOOST_CHECK( ps::lost_degrees_of_freedom( h0_(), acc ) == 3 );
        BOOST_CHECK( ps::degrees_of_freedom( h0_(),acc ) == 2 );
    }
    {
        // Check cells 
        typedef boost::mpl::vector1<x_> vec_x_;
        typedef boost::mpl::vector1<y_> vec_y_;   
        std::size_t n;
        n = ac::extract::weighted_count( acc );
     	BOOST_CHECK( n == 3 );

        n = ct::non_empty_cells_count<vec_x_>(acc);
        BOOST_CHECK( n == 2 );
        n = ct::non_empty_cells_count<vec_y_>(acc);
        BOOST_CHECK( n == 2 );
        n = ct::non_empty_cells_count<keys_>( acc );
        BOOST_CHECK( n == 2 );

        n = ct::cells_count<vec_x_>(acc);
        BOOST_CHECK( n == 2 );
        n = ct::cells_count<vec_y_>(acc);
        BOOST_CHECK( n == 3 );
        n = ct::cells_count<keys_>( acc );
        BOOST_CHECK( n == 6 );
        
        n = ct::count_matching<vec_x_>( 
            acc, boost::fusion::make_map<x_>( -1 ) );
     	BOOST_CHECK( n == 1 );
        n = ct::count_matching<vec_x_>( 
            acc, boost::fusion::make_map<x_>( 1 ) );
     	BOOST_CHECK( n == 2 );
        n = ct::count_matching<vec_y_>( 
            acc, boost::fusion::make_map<y_>( "a" ) );
     	BOOST_CHECK( n == 1 );
        n = ct::count_matching<vec_y_>( 
            acc, boost::fusion::make_map<y_>( "b" ) );
     	BOOST_CHECK( n == 2 );
        n = ct::count_matching<vec_y_>( 
            acc, boost::fusion::make_map<y_>( "c" ) );
     	BOOST_CHECK( n == 0 );
        n = ct::count_matching<keys_>( acc,
            boost::fusion::make_map<x_,y_,z_>( -1, "a", 1 ) );
        BOOST_CHECK( n == 1 );
        n = ct::count_matching<keys_>( acc, 
            boost::fusion::make_map<x_,y_,z_>(  1, "b", 1 ) );
        BOOST_CHECK( n == 2 );
    }

}

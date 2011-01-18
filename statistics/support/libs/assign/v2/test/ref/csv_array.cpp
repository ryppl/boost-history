//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <boost/mpl/int.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/range.hpp>
#include <boost/assign/v2/detail/checking/array.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <libs/assign/v2/test/ref/csv_array.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_csv_array{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        typedef as2::ref::assign_tag::copy copy_;

        using namespace as2::checking::constants;
        {
            typedef as2::ref::nth_result_of::csv_array<8, int const>::type ar_;
            using namespace as2::checking::constants;
            ar_ ar = as2::ref::csv_array( a , b , c , d , e , f , g , h );
            {
                typedef as2::container_tag::array tag_;
                as2::checking::do_check(tag_(), ar );
            }
            {
                namespace ns = as2::checking::iterator;
                ns::do_check( boost::begin( ar ) );
            }
            {
                typedef as2::container_tag::range tag_;
                as2::checking::do_check(tag_(), ar );
            }
		}
		{
			int a1, b1, c1, d1, e1, f1, g1, h1;
            typedef std::vector<int> vec_;
            vec_ vec;
            {
            	using namespace as2::checking::constants;
            	vec.push_back( a );
            	vec.push_back( b );
            	vec.push_back( c );
            	vec.push_back( d );
            	vec.push_back( e );
            	vec.push_back( f );
            	vec.push_back( g );
            	vec.push_back( h );
            }
            boost::copy(
                vec,
                boost::begin(
                    as2::ref::csv_array( 
                    	a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1 
                    ) | as2::ref::_get
                )
            );
            {
                typedef as2::container_tag::array tag_;
                as2::checking::do_check(
                    tag_(),
                    as2::ref::csv_array<int const>(
                        a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1
                    )
                );
            }
		}
    }

}// xxx_csv_array
}// xxx_ref
}// test_assign

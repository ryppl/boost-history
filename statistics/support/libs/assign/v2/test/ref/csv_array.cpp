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
#include <libs/assign/v2/test/ref/csv_array.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_csv_array{

    void test()
    {
    	using namespace boost::assign::v2;
        typedef ref::assign_tag::copy copy_;
        {
            typedef ref::nth_result_of::csv_array<8, int const>::type ar_;
            using namespace checking::constants;
            ar_ ar = ref::csv_array( a , b , c , d , e , f , g , h );
            using namespace checking;
            {
                typedef container_tag::static_array tag_;
                checking::do_check(tag_(), ar );
            }
            {
                typedef container_tag::range tag_;
                checking::do_check(tag_(), ar );
            }
		}
		{
			int a1, b1, c1, d1, e1, f1, g1, h1;
            typedef std::vector<int> vec_;
            vec_ vec;
            {
            	using namespace checking::constants;
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
                    ref::csv_array( a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1 )
                )
            );
            {
                using namespace checking;
                typedef container_tag::static_array tag_;
                do_check(
                    tag_(),
                    ref::csv_array<int const>(
                        a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1
                    )
                );
            }
		}
    }

}// xxx_csv_array
}// xxx_ref
}// test_assign

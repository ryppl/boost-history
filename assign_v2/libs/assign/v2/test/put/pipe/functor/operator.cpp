//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
//#include <map>
#include <deque>
#include <vector>
#include <list>
#include <string>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp> 

#include <libs/assign/v2/test/put/pipe/functor/operator.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_operator{ 

    void test(){
        namespace as2 = boost::assign::v2;
    	{
        	//pipe_str_literal
	        typedef const char* T;
			std::deque<T> cont; typedef std::string str_;
        	BOOST_ASSIGN_V2_CHECK( str_( 
            	( cont | as2::_put( "x" )( "y" )( "z" ) )[0] 
        	) == "x" );
        	BOOST_ASSIGN_V2_CHECK( str_( cont[1] ) == "y" );
        	BOOST_ASSIGN_V2_CHECK( str_( cont[2] ) == "z" );
    	}
		{
            //[pipe_array
            const int sz = 3;
    		typedef boost::array<int, sz>  r_;
    		boost::array<r_, sz>  matrix3x3;
            
            {
            	r_ r0, r1, r2;
                int front = ( /*Calls [^r0[i] = j] for [^( i, j ) = ( 0, 1 ), ( 1, 2 ), ( 2, 3 ) ] and returns r0*/ r0 | as2::_put( 1 )( 2 )( 3 ) ).front();
                int back = r0.back();
                r2 | as2::_put( 7 )( 8 )( 9 );
            	r_ r = ( 
                	matrix3x3 | as2::_put
                		( r0 )  
                    	( /*<<`r1` modified on the fly>>*/r1 | as2::_put( 4 )( 5 )( 6 ) ) 
                    	( r2 )
                )[1];

                BOOST_ASSIGN_V2_CHECK( front == 1    );
                BOOST_ASSIGN_V2_CHECK( back  == 3    );
                BOOST_ASSIGN_V2_CHECK( r   == r1 );
			}
            for(int i = 0; i < 9; i++)
            {
            	BOOST_ASSIGN_V2_CHECK( matrix3x3[ i / 3 ][ i % 3 ] == i + 1 );
            }
			//]
        }
		{
        	//[pipe_seq_var_args
            typedef double elem_; typedef std::list<elem_> r_; typedef std::vector<r_> ragged_array_;
			r_ a, b; elem_ front, back;
            front = ( a | as2::_put( 0.71 )( 0.63 )( 0.85 ) ).front();
            back = a.back();
            
			ragged_array_ ragged_array; 
            r_ r = (
            	ragged_array | as2::_put
            		( a ) 
                	( /*<<`b` modified on the fly>>*/ b | as2::_put( 0.61 )( 0.69 )( 0.92 )( 0.55 ) )
					( 1, -99.99 )
                	( )
            )[2];

			elem_ eps = boost::numeric::bounds<elem_>::smallest();
            BOOST_ASSIGN_V2_CHECK( abs( front - 0.71     ) < eps );
            BOOST_ASSIGN_V2_CHECK( abs( back - 0.85      ) < eps );
            BOOST_ASSIGN_V2_CHECK( abs( r.front() + 99.0 ) < eps ); 
            BOOST_ASSIGN_V2_CHECK( abs( r.back() + 99.0  ) < eps ); 
            //]
            BOOST_ASSIGN_V2_CHECK( ragged_array[0].size() == a.size() );
            BOOST_ASSIGN_V2_CHECK( ragged_array[1].size() == b.size() );
            BOOST_ASSIGN_V2_CHECK( ragged_array[2].size() == 1        );
            BOOST_ASSIGN_V2_CHECK( ragged_array[3].size() == 0        );
		}
		{
            using namespace boost;
            //[pipe_seq_ref_tuple
            typedef const char state_ [3]; state_ ct = "CT", nj = "NJ", ny = "NY";
            typedef int code_; typedef boost::tuple<state_/*<<Notice the reference>>*/&,  code_> area_code_; 
            std::deque< area_code_ > tri_state;
            area_code_ front = (
            	tri_state | as2::_put( nj, 201 )( ct, 203 )( ny, 212 )( ny, 315 )( ny, 347 )( nj, 551 )
            ).front();

            BOOST_ASSIGN_V2_CHECK( get<0>( front ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( front ) == 201 );
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state.back() ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state.back() ) == 551 );
            //]
        }

    }// test()

}// xxx_operator
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/mpl/int.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/modifier/ext/repeat.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <libs/assign/v2/test/put/pipe/functor/container.h>
 
namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_container{

	struct val_pred{
    
    	val_pred(){}
		template<typename T, typename U>
    	void operator()(T const& t, U const& u)const
        {
        	BOOST_ASSIGN_V2_CHECK( t == u );
        }
    };

	struct ref_pred{
    
    	ref_pred(){}
		template<typename T, typename U>
    	void operator()(T const& t, U const& u)const
        {
        	BOOST_ASSIGN_V2_CHECK( &t == &u );
        }
    };

	template<int i, int j, typename T, typename P, typename U>
	void local_assert(T& tmp, P const& pred, U const& u)
    {
    
        using namespace boost;
    	pred(
    		get<j>( 
        		tmp.seq_args().get( boost::mpl::int_<i>() ) 
        	),
            u
        );
    }

	void test()
    {

        using namespace boost;
        using namespace boost::assign::v2;
        using namespace checking::constants;

        typedef boost::mpl::int_<0> zero_;
        {
            typedef put_pipe_aux::container<> container_;
            int x = -1;
            BOOST_AUTO(
            	tmp,
            	( ( container_() %  ( _repeat = 3 ) )( x ) )
            );
            int n = tmp
                .pars()
                .static_lookup( zero_() ).get();
            BOOST_ASSIGN_V2_CHECK( n == 3 );
        }
        {
            #define A -10
            #define B 10
            int a1, b1, c1;
            using namespace checking::constants;
            {
                a1 = a; b1 = b; c1 = c;
            }

            BOOST_AUTO(
            	tmp,
                ( _put
                    ( a1, b1 )( a1, b )( a1, B )
                    ( a, b1 )( a, b )( a, B )
                    ( A, b1 )( A, b )( A, B )
                    ( a1 )
                    ( a )
                    ( A )
                    ()
                )
            );
			local_assert<0, 0>( tmp, ref_pred(), a1);
			local_assert<0, 1>( tmp, ref_pred(), b1);
			local_assert<1, 0>( tmp, ref_pred(), a1); local_assert<1, 1>( tmp, ref_pred(), b);
			local_assert<2, 0>( tmp, ref_pred(), a1); local_assert<2, 1>( tmp, val_pred(), B);
            
			local_assert<3, 0>( tmp, ref_pred(), a ); local_assert<3, 1>( tmp, ref_pred(), b1);
			local_assert<4, 0>( tmp, ref_pred(), a ); local_assert<4, 1>( tmp, ref_pred(), b);
			local_assert<5, 0>( tmp, ref_pred(), a ); local_assert<5, 1>( tmp, val_pred(), B);

			local_assert<6, 0>( tmp, val_pred(), A); local_assert<6, 1>( tmp, ref_pred(), b1);
			local_assert<7, 0>( tmp, val_pred(), A); local_assert<7, 1>( tmp, ref_pred(), b);
			local_assert<8, 0>( tmp, val_pred(), A); local_assert<8, 1>( tmp, val_pred(), B);

			local_assert<9, 0>( tmp, ref_pred(), a1);
			local_assert<10, 0>( tmp, ref_pred(), a);
			local_assert<11, 0>( tmp, val_pred(), A);
            
#undef A
#undef B
        }
        {
            BOOST_AUTO(tmp, _put( "x" ) );
			typedef boost::mpl::int_<0> int_;
			typedef std::string str_;
			BOOST_ASSIGN_V2_CHECK(
            	str_( get<0>( tmp.seq_args().get( int_() ) ) ) == "x"
            );
		}
    }


}// xxx_container
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2

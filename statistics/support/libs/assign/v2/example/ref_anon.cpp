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
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/ref_anon.h>

namespace example_assign_v2{
namespace xxx_ref_anon{

	void run()
    {

        using namespace boost::assign::v2;
        std::cout << "---xxx_ref_anon " << std::endl;
        {
			std::cout << "ref::assign_copy::csv_anon" << std::endl;
        	//[assign_copy_read
            namespace ns = ref::assign_copy;
            boost::for_each( ns::csv_anon( x, y, z ), printer(std::cout) );
            //outputs (1,0)(NULL,0)(1,5)
            //]
        	//[assign_copy_write
            T x1, y1, z1;
            ns::csv_anon( x1, y1, z1 ).assign( T( b, a ) );
            BOOST_ASSERT( y1 == T( b, a ) );
            //]
        	//[csv_anon_empty
           		ns::csv_anon<T const>( _nil );
            //]
		}    
        {
        	//[assign_rebind_read
            namespace ns = ref::assign_rebind;
            typedef ns::nth_result_of::csv_anon<3, T const>::type result_;
            result_ result = ns::csv_anon( x, y, z );
            BOOST_ASSERT( &result[ 1 ] == &y );
            //]
            //[assign_rebind_write
            T w = T( b, a ); result.assign( w );
            BOOST_ASSERT( &result[ 1 ] == &w );
        	//]
        }
        {
        	//[anon_read_empty
            namespace ns = ref::assign_rebind;
            typedef ns::nth_result_of::anon<2, T const>::type result1_;
            result1_ result1 = ns::anon( x )( y );
            //]
            //[anon_read_fill
            typedef ns::nth_result_of::anon<3, T const>::type result2_;
            result2_ result2 = result1( z );
            //]
            //[anon_read_print
                boost::for_each( result2, printer(std::cout) );
            //]
		}
        std::cout << std::endl;
    }

}// xxx_ref_anon
}// example_assign_v2


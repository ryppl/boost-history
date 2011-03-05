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
#include <deque>
#include <boost/typeof/typeof.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <libs/assign/v2/test/put/modulo.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modulo{
namespace xxx_fun{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
            //[put_cont_modulo_fun
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<int> cont;

            // TODO fix BUG >
            (
                as2::put( cont ) % ( as2::_fun = ( lambda::_1 + 1 ) )
            )( x )( y )( z ); 
            // < LLVM 1.5 - Release mode, EXC_BAD_ACCESS, stl_vector.h #602
            // not for only ( x )!

            //BOOST_ASSIGN_V2_CHECK( cont.front() == ( x + 1 ) );
            //BOOST_ASSIGN_V2_CHECK( cont.back() == ( z + 1 ) );
            //]
        }
        
// TODO uncomment when bug above is fixed
/*        
        {
            //[put_deque_modulo_fun
            typedef int T; T x = 1, y = 2, z = 0;
            BOOST_AUTO(
                cont, (
                    as2::deque<T>( as2::_nil ) % (
                        as2::_fun = ( lambda::_1 + 1 )
                    )
                )( x )( y )( z )
            );

            BOOST_ASSIGN_V2_CHECK( cont.front() == ( x + 1 ) );
            BOOST_ASSIGN_V2_CHECK( cont.back() == ( z + 1 ) );
            //]
        }
 */

    }

}// xxx_fun
}// xxx_modulo
}// xxx_put
}// test_assign_v2

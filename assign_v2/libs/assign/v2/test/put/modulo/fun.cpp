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
#include <boost/spirit/home/phoenix.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <libs/assign/v2/test/put/modulo.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modulo{
namespace xxx_fun{

#ifdef __llvm__ 

    struct f{
        
        typedef int result_type;
        
        template<typename T>
        result_type operator()(T const& i)const{ return i + 1; };
        
    };
    
    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            std::vector<int> incr_fact;
            (
             as2::put( cont ) % ( as2::_fun = f() )
             )( 1 )( 2 )( 6 )( 24 )( 120 ); 
            
            BOOST_ASSIGN_V2_CHECK( incr_fact.front() == ( 2 ) );
            BOOST_ASSIGN_V2_CHECK( incr_fact.back() == ( 121 ) );
            // TODO fix Bug :
            // LLVM 1.5 - Release mode, EXC_BAD_ACCESS, stl_vector.h #602
        }        
        
    }
    
#else

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::phoenix;
        {
            //[modulo_fun
            std::vector<int> incr_fact;
            (
            	as2::put( incr_fact ) % ( as2::_fun = lambda::arg_names::arg1 + 1 )
            )/*<<1!, 2!, 3!, 4!, 5!>>*/( 1 )( 2 )( 6 )( 24 )( 120 ); 

            BOOST_ASSIGN_V2_CHECK( incr_fact.front() == ( 2 ) );
            BOOST_ASSIGN_V2_CHECK( incr_fact.back() == ( 121 ) );
            //]
        }
        {
            //[modulo_fun_deque
            BOOST_AUTO(
                incr_fact, (
                    as2::deque<int>( as2::_nil ) % (
                        as2::_fun = ( lambda::arg_names::arg1 + 1 )
                    )
                )/*<<1!, 2!, 3!, 4!, 5!>>*/( 1 )( 2 )( 6 )( 24 )( 120 )
            );

            BOOST_ASSIGN_V2_CHECK( incr_fact.front() == ( 2 ) );
            BOOST_ASSIGN_V2_CHECK( incr_fact.back() == ( 121 ) );
            //]
        }

    }

#endif


}// xxx_fun
}// xxx_modulo
}// xxx_put
}// test_assign_v2

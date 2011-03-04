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
#include <boost/assign/v2/detail/check/equal_val.hpp>
#include <boost/assign/v2/detail/check/equal_ref.hpp>
#include <boost/assign/v2/put/ext/repeat.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp> // rhs?
#include <boost/assign/v2/utility/csv.hpp>
#include <libs/assign/v2/test/put/pipe/functor/rhs.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_rhs{

    void test()
    {
        namespace as2 = boost::assign::v2;
        typedef boost::mpl::int_<0> zero_;
        {
            typedef as2::put_pipe_aux::rhs<> rhs_;
            int x = -1;
            BOOST_AUTO( rhs, ( ( rhs_() %  ( as2::_repeat = 3 ) )( x ) ) );
            int n = as2::ref::at<0>( rhs.pars() ).arg();
            BOOST_ASSIGN_V2_CHECK( n == 3 );
        }
        namespace chk = as2::check_aux;
        namespace aux_ = as2::put_pipe_aux;
        typedef chk::equal_ref r_;
        typedef chk::equal_val v_;
        {
/* rvalue */
#define A -10
#define B 10
            /* lvalue */
            int a1 = -1, b1 = 0;
            typedef const int cint_;
            cint_ a = 4, b = 5;

            BOOST_AUTO(
                rhs,
                ( as2::_put
                    ( a1, b1 )( a1, b )( a1, B )
                    ( a, b1 )( a, b )( a, B )
                    ( A, b1 )( A, b )( A, B )
                    ( a1 )
                    ( a )
                    ( A )
                    ()
                )
            );

            aux_::check_rhs<0, 0>( rhs, r_(), a1);
            aux_::check_rhs<0, 1>( rhs, r_(), b1);
            aux_::check_rhs<1, 0>( rhs, r_(), a1); aux_::check_rhs<1, 1>( rhs, r_(), b);
            aux_::check_rhs<2, 0>( rhs, r_(), a1); aux_::check_rhs<2, 1>( rhs, v_(), B);

            aux_::check_rhs<3, 0>( rhs, r_(), a ); aux_::check_rhs<3, 1>( rhs, r_(), b1);
            aux_::check_rhs<4, 0>( rhs, r_(), a ); aux_::check_rhs<4, 1>( rhs, r_(), b);
            aux_::check_rhs<5, 0>( rhs, r_(), a ); aux_::check_rhs<5, 1>( rhs, v_(), B);

            aux_::check_rhs<6, 0>( rhs, v_(), A); aux_::check_rhs<6, 1>( rhs, r_(), b1);
            aux_::check_rhs<7, 0>( rhs, v_(), A); aux_::check_rhs<7, 1>( rhs, r_(), b);
            aux_::check_rhs<8, 0>( rhs, v_(), A); aux_::check_rhs<8, 1>( rhs, v_(), B);

            aux_::check_rhs<9, 0>( rhs, r_(), a1);
            aux_::check_rhs<10, 0>( rhs, r_(), a);
            aux_::check_rhs<11, 0>( rhs, v_(), A);
#undef A
#undef B
        }
        {
            // str_literal
            BOOST_AUTO(rhs, as2::_put( "x" ) );
            typedef boost::mpl::int_<0> int_;
            typedef std::string str_;
            #if BOOST_ASSIGN_V2_ENABLE_CPP0X
            using namespace boost::assign::v2::ref;
            #else
            using namespace boost;
            #endif
            aux_::check_rhs<0, 0>( rhs, v_(), str_( "x" ) );
        }
    }


}// xxx_rhs
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2

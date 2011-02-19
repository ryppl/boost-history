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
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <libs/assign/v2/test/put/pipe/functor/rhs.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_rhs{

    template<int i, int j, typename T, typename P, typename U>
    void xxx_assert(T& rhs, P const& pred, U const& u)
    {
        // TODO name lookup?
        using namespace boost; // tuple<> (cpp03)
        using namespace boost::assign::v2; // tuple (cpp0x)
        pred(
            get<j>(
                rhs.seq_args().get( boost::mpl::int_<i>() )
            ),
            u
        );
    }

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

            namespace chk = as2::check_aux;
            typedef chk::equal_ref r_;
            typedef chk::equal_val v_;

            xxx_assert<0, 0>( rhs, r_(), a1);
            xxx_assert<0, 1>( rhs, r_(), b1);
            xxx_assert<1, 0>( rhs, r_(), a1); xxx_assert<1, 1>( rhs, r_(), b);
            xxx_assert<2, 0>( rhs, r_(), a1); xxx_assert<2, 1>( rhs, v_(), B);

            xxx_assert<3, 0>( rhs, r_(), a ); xxx_assert<3, 1>( rhs, r_(), b1);
            xxx_assert<4, 0>( rhs, r_(), a ); xxx_assert<4, 1>( rhs, r_(), b);
            xxx_assert<5, 0>( rhs, r_(), a ); xxx_assert<5, 1>( rhs, v_(), B);

            xxx_assert<6, 0>( rhs, v_(), A); xxx_assert<6, 1>( rhs, r_(), b1);
            xxx_assert<7, 0>( rhs, v_(), A); xxx_assert<7, 1>( rhs, r_(), b);
            xxx_assert<8, 0>( rhs, v_(), A); xxx_assert<8, 1>( rhs, v_(), B);

            xxx_assert<9, 0>( rhs, r_(), a1);
            xxx_assert<10, 0>( rhs, r_(), a);
            xxx_assert<11, 0>( rhs, v_(), A);

#undef A
#undef B
        }
        {
            BOOST_AUTO(rhs, as2::_put( "x" ) );
            typedef boost::mpl::int_<0> int_;
            typedef std::string str_;
            // TODO find a simpler way:
            #if BOOST_ASSIGN_V2_ENABLE_CPP0X
            using namespace boost::assign::v2::ref;
            #else
            using namespace boost;
            #endif
            BOOST_ASSIGN_V2_CHECK(
                str_(
                    get<0>(
                        rhs.seq_args().get( int_() )
                    )
                ) == "x"
            );
        }
    }


}// xxx_rhs
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2

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
#include <iostream>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_defaults.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/selection/min.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/config/limit_lvalue_const_arity.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <libs/assign/v2/test/detail/functor.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_functor{

    void test()
    {
        using namespace boost::assign::v2;

// Note : 10 is the max tuple size
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#define BOOST_ASSIGN_V2_n BOOST_PP_MIN( \
        10, \
        BOOST_PP_SUB( \
            BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY), \
            1 \
        ) \
    ) \
/**/
#else
#define BOOST_ASSIGN_V2_n BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY
#endif

        {
#define BOOST_ASSIGN_V2_lvalue(z, n, data) int const&
#define BOOST_ASSIGN_V2_trail \
    BOOST_PP_ENUM_TRAILING( \
        BOOST_PP_DEC(BOOST_ASSIGN_V2_n), \
        BOOST_ASSIGN_V2_lvalue, \
        ~ \
    ) \
/**/
            // The first element is an LValue and the others are const to
            // test that LValue and const can be mixed
            typedef boost::tuple<
                int&
                BOOST_ASSIGN_V2_trail
            > t_;
#undef BOOST_ASSIGN_V2_lvalue
#undef BOOST_ASSIGN_V2_trail

            typedef functor_aux::constructor<t_> f_;

int BOOST_PP_ENUM_PARAMS(
    BOOST_ASSIGN_V2_n,
    x
);

#define BOOST_ASSIGN_V2_check(z, i, data ) \
    BOOST_ASSIGN_V2_CHECK_EQUAL( \
            & boost::get< i >( t ), \
            & BOOST_PP_CAT(x,i) \
    ); \
/**/
            f_ f;
            t_ t = f( BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_n, x) );

            BOOST_PP_REPEAT(
                BOOST_ASSIGN_V2_n,
                BOOST_ASSIGN_V2_check,
                ~
            )

            typedef std::string str_;
            const str_ cpp
                = BOOST_ASSIGN_V2_ENABLE_CPP0X ? "C++0x" : "C++03";
            const str_ str = str_("test_assign_v2::xxx_detail::xxx_functor")
                + "{ " + cpp + ", arity = ";

            std::cout
                << str
                << BOOST_ASSIGN_V2_n
                << " }"
                << std::endl;

#undef BOOST_ASSIGN_V2_check
#undef BOOST_ASSIGN_V2_n
        }
    }

}// xxx_functor
}// xxx_detail
}// test_assign_v2

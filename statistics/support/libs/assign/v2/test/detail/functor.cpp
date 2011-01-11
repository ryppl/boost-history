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
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/assign/v2/detail/config/tuple_limit_arity.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
#include <boost/assign/v2/detail/config/limit_lvalue_const_arity.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <libs/assign/v2/test/detail/functor.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_functor{

    void test(){
        using namespace boost::assign::v2;

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#define BOOST_ASSIGN_V2_n BOOST_ASSIGN_V2_CPP03_TUPLE_LIMIT_ARITY
// That's because we are constructing a boost::tuple<>.
#else
#define BOOST_ASSIGN_V2_n BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY

#if BOOST_ASSIGN_V2_n > BOOST_ASSIGN_V2_CPP03_TUPLE_LIMIT_ARITY
#error
#endif

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        {
            typedef int& x_ref_;
            typedef int const& y_ref_;
            typedef int z_;

#define MACRO(Z, n, data) data

            typedef boost::tuple<
                BOOST_PP_EXPR_IF(
                    BOOST_ASSIGN_V2_n,
                    x_ref_
                )
                BOOST_PP_ENUM_TRAILING(
                    BOOST_PP_IF(BOOST_PP_DEC(BOOST_ASSIGN_V2_n),1,0),
                    MACRO,
                    y_ref_
                )
                BOOST_PP_ENUM_TRAILING(
                    BOOST_PP_SUB(BOOST_ASSIGN_V2_n,2),
                    MACRO,
                    z_
                )
            > t_;
#undef MACRO

            typedef functor_aux::constructor<t_> f_;
            f_ f;

            int x = -1;
            int const y = 0;

#define MACRO(z, n, data) data

            t_ t = f(
                BOOST_PP_EXPR_IF(
                    BOOST_ASSIGN_V2_n,
                    x
                )
                BOOST_PP_ENUM_TRAILING(
                    BOOST_PP_IF(BOOST_PP_DEC(BOOST_ASSIGN_V2_n),1,0),
                    MACRO,
                    y
                )
                BOOST_PP_ENUM_TRAILING(
                    BOOST_PP_SUB(BOOST_ASSIGN_V2_n, 2),
                    MACRO,
                    1
                )
            );
#undef MACRO
            BOOST_PP_EXPR_IF(
                BOOST_ASSIGN_V2_n,
                BOOST_ASSIGN_V2_CHECK(
                    &boost::get<0>( t ) == &x
                );
            )
            BOOST_PP_EXPR_IF(
                BOOST_PP_DEC(BOOST_ASSIGN_V2_n),
                BOOST_ASSIGN_V2_CHECK(
                    &boost::get<1>( t ) == &y
                );
            )

#define MACRO(z, i, data ) \
    BOOST_ASSIGN_V2_CHECK( \
        boost::get< BOOST_PP_ADD(i,2) >( t ) == 1 \
    ); \
/**/

            BOOST_PP_REPEAT(
                BOOST_PP_SUB(BOOST_ASSIGN_V2_n,2),
                MACRO,
                ~
            )
#undef MACRO

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

#undef BOOST_ASSIGN_V2_n
        }
    }
    
}// xxx_functor
}// xxx_detail
}// test_assign_v2

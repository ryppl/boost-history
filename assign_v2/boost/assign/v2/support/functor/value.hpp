//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_VALUE_ER_2010_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/assign/v2/support/config/limit_arity.hpp>
#include <boost/assign/v2/support/functor/crtp_unary_and_up.hpp>
#include <boost/mpl/always.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
//syntax_detail_functor_value
namespace functor_aux{

    template<typename T = nil_>
    class value/*<-*/
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
    : public functor_aux::crtp_unary_and_up<
        functor_aux::value<T>,
        ::boost::mpl::always<T>
    >
#endif
    /*->*/
    {
//<-
        typedef functor_aux::value<T> this_;
//->
        public:

        value()/*<-*/{}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
        template<typename... Args>
        T operator()(Args&&...args)const/*<-*/
        {
            return T( std::forward<Args>(args)... );
        }/*->*/BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
//<-

#else

        protected:
            typedef ::boost::mpl::always<T> meta_result_;
            typedef functor_aux::crtp_unary_and_up<this_, meta_result_> super_;

        public:

        using super_::operator();
        T operator()()const{ return T(); }

#define BOOST_ASSIGN_V2_MACRO(z, N,data) \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    T impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, &_) )const{ \
        return T( BOOST_PP_ENUM_PARAMS(N, _) ); \
    } \
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

//->
    };

}// functor_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_VALUE_ER_2010_HPP

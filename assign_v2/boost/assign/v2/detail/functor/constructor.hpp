//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CONSTRUCTOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CONSTRUCTOR_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
//#include <boost/assign/v2/detail/traits/container/value.hpp>// TODO remove
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/range/reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/always.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

    template<typename T>
    class constructor
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
    : public functor_aux::crtp_unary_and_up<
        functor_aux::constructor<T>,
        ::boost::mpl::always<T>
    >
#endif
    {
        typedef functor_aux::constructor<T> this_;

        public:

        constructor(){}


#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        template<typename... Args>
        T operator()(Args&&...args)const
        {
            return T( std::forward<Args>(args)... );
        }
#else
        protected:
            typedef ::boost::mpl::always<T> meta_result_;
            typedef functor_aux::crtp_unary_and_up<this_, meta_result_> super_;

        public:

        using super_::operator();
        T operator()()const{ return T(); }

#define BOOST_ASSIGN_V2_MACRO(z,N,data) \
    template<BOOST_PP_ENUM_PARAMS(N,typename T)> \
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

#endif
    };

/*
    TODO remove
    template<typename V>
    struct deduce_constructor
    {
        typedef typename container_traits::value<V>::type value_;
        typedef functor_aux::constructor<value_> type;
        static type call(){ return functor_aux::constructor<value_>(); }
    };
*/

}// functor_aux
namespace result_of{

    template<typename T>
    struct constructor
    {
        typedef functor_aux::constructor<T> type;
    };

}// result_of

    template<typename T>
    typename result_of::constructor<T>::type
    constructor()
    {
        typedef typename result_of::constructor<T>::type result_;
        return result_();
    }

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_FUNCTOR_CONSTRUCTOR_ER_2010_HPP

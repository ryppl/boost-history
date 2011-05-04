//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_STD_ER_2011_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_STD_ER_2011_HPP
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/option/modifier/framework.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost{
namespace assign{
namespace v2{

BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(std_modifier)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(std_modifier, Arg)

}// v2
}// assign
}// boost

#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#define BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_IMPL(FUN)\
    template<typename C, typename T>\
        void impl(C& cont, T&& t, data_tag::value)const{\
        cont.FUN( std::forward<T>( t ) );\
    }\
    template<typename C, typename T>\
    void impl(C& cont, T&& t, data_tag::ptr)const{\
        typedef typename container_aux::value<C>::type value_;\
        cont.FUN( new value_( std::forward<T>( t ) ) );\
    }\
/**/
#else
#define BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_IMPL(FUN)\
    template<typename C, typename T>\
        void impl(C& cont, T& t, data_tag::value)const{\
        cont.FUN( t );\
    }\
    template<typename C, typename T>\
    void impl(C& cont, T& t, data_tag::ptr)const{\
        typedef typename container_aux::value<C>::type value_;\
        cont.FUN( new value_( t ) );\
    }\
/**/
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X


#define BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_TAG(FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace modifier_tag{ struct FUN{}; }\
}\
}\
}\
/**/

// Must be preceded by BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_TAG(FUN)
#define BOOST_ASSIGN_V2_OPTION_STD_MODIFIER(FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace interpreter_aux{\
\
    template<>\
    class interpreter_modifier<modifier_tag::FUN>\
    {\
        public:\
        interpreter_modifier(){}\
        interpreter_modifier( ignore_, ignore_ ){}\
        BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_IMPL(FUN)\
    };\
\
}\
namespace{\
\
    interpreter_aux::option_modifier<\
        interpreter_aux::keyword_std_modifier,\
        modifier_tag::FUN\
    > const\
        BOOST_PP_CAT(_,FUN) = ( \
        _std_modifier = modifier_tag::FUN() \
    );\
\
}\
namespace result_of{\
\
    template<typename D>\
    struct BOOST_PP_CAT(option_, FUN)\
        : result_of::option_std_modifier<D, modifier_tag::FUN>\
    {};\
\
}\
}\
}\
}\
/**/

BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_TAG(push)
BOOST_ASSIGN_V2_OPTION_STD_MODIFIER(push)

BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_TAG(push_back)
BOOST_ASSIGN_V2_OPTION_STD_MODIFIER(push_back)

BOOST_ASSIGN_V2_OPTION_STD_MODIFIER_TAG(push_front)
BOOST_ASSIGN_V2_OPTION_STD_MODIFIER(push_front)

#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_STD_ER_2011_HPP

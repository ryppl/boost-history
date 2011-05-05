//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_DATA_ER_2011_HPP
#define BOOST_ASSIGN_V2_OPTION_DATA_ER_2011_HPP
#include <boost/assign/v2/support/functor/value.hpp>
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/interpreter/replace.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_data
namespace interpreter_aux{

    template<
        typename C      // Value- or pointer-container
        , typename F    // Functor or keyword    
    >
    struct data_generator/*<-*/
    {
        typedef F type;
    }/*->*/;

    template<
        typename C // Multi-array
    >
    struct data_generator<C, element_>/*<-*/
    {
        typedef typename container_aux::element<C>::type element_;
        typedef functor_aux::value<element_> type;
    }/*->*/;

    template<typename C>
    struct data_generator<C, key_>/*<-*/
    {
        typedef typename container_aux::key<C>::type key_;
        typedef functor_aux::value<key_> type;
    }/*->*/;

    template<typename C>
    struct data_generator<C, map_>/*<-*/
    {
        typedef typename container_aux::key<C>::type key_;
        typedef typename container_aux::mapped<C>::type mapped_;
        typedef typename container_aux::value<C>::type value_;
        typedef functor_aux::pair<value_, key_, mapped_> type;
    }/*->*/;

    template<typename C>
    struct data_generator<C, use_default_>/*<-*/
        : deduce_data_generator<C>
    {}/*->*/;

    template<typename C>
    struct data_generator<C, value_>/*<-*/
    {
        typedef functor_aux::value<
            typename container_aux::value<C>::type
        > type;
    }/*->*/;

namespace result_of{

    template<typename D, typename C, typename F = use_default_>
    struct option_data/*<-*/
        : ::boost::mpl::apply1<
            interpreter_aux::replace_data_generator<D>, 
            typename data_generator<C, F>::type
        >
    {}/*->*/;
        
}// result_of

    // F is either a functor or one of the keywords : 
    //  element_, key_, map_, use_default_, value_
    template<typename F = ignore_>
    struct option_data/*<-*/
        : option_crtp<
            option_data<F> 
        >
    {
        option_data(){}
        option_data(F f) : f_( f ){}

        template<typename C>
        F const& get()const{ return this->f_; }
        
        private:
        F f_;
    }/*->*/;

//<-
    template<typename Kwd>
    struct option_data_helper
        : option_crtp<
            option_data<Kwd> 
        >
    {
        template<typename C>
        typename data_generator<C, Kwd>::type 
        get()const
        { 
            return typename data_generator<C, Kwd>::type(); 
        }
    };

#define BOOST_ASSIGN_V2_MACRO(Kwd)\
    template<>\
    struct option_data<Kwd>\
        : option_data_helper<Kwd>\
    {\
        option_data(){}\
        option_data(ignore_){}\
    };\
/**/    
BOOST_ASSIGN_V2_MACRO(element_)
BOOST_ASSIGN_V2_MACRO(key_)
BOOST_ASSIGN_V2_MACRO(map_)
BOOST_ASSIGN_V2_MACRO(use_default_)
BOOST_ASSIGN_V2_MACRO(value_)
#undef BOOST_ASSIGN_V2_MACRO
//->

    // Overrides data generator
    template<typename D, typename C, typename F, typename MTag
        , typename DTag, typename F1>
    typename result_of::option_data<D, C, F1>::type
    operator%(
        interpreter_crtp<D, C, F, MTag, DTag> const& lhs,
        option_data<F1> const& rhs
    )/*<-*/
    {
        typedef typename result_of::option_data<
            D, C, F1
        >::type result_;
        return result_( 
            lhs.container(), 
            rhs.template get<C>(), 
            lhs.modifier 
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    struct keyword_data
    {

        template<typename F>
        option_data<F> operator=(F const& f)const/*<-*/{
            return option_data<F>( f );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    };

}// interpreter_aux
namespace{
    const interpreter_aux::keyword_data _data/*<-*/ = {}/*->*/;
}
namespace result_of{

    using interpreter_aux::result_of::option_data;

}// result_of
//]
}// v2
}// assign
}// boost

#include <boost/preprocessor/cat.hpp>

#define BOOST_ASSIGN_V2_OPTION_DATA_GENERATE(NAME, FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace interpreter_aux{\
\
    template<typename T>\
    option_data< FUN > NAME()\
    {\
        return ( v2::_data = FUN() );\
    }\
\
}\
using interpreter_aux::NAME;\
}\
}\
}\
/**/

#include <boost/assign/v2/support/functor/value.hpp>
BOOST_ASSIGN_V2_OPTION_DATA_GENERATE(value, v2::functor_aux::value<T>)

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/add_const.hpp>
#define BOOST_ASSIGN_V2_OPTION_DATA_KEYWORD(NAME, EXPR)\
namespace boost{\
namespace assign{\
namespace v2{\
    typedef BOOST_TYPEOF( ( _data = EXPR ) ) BOOST_PP_CAT(type_of,NAME);\
namespace{\
\
    boost::add_const<BOOST_PP_CAT(type_of,NAME)>::type BOOST_PP_CAT(_,NAME) = ( _data = EXPR );\
}\
}\
}\
}\

#include <boost/lambda/lambda.hpp>
BOOST_ASSIGN_V2_OPTION_DATA_KEYWORD(identity, ::boost::lambda::_1)

#endif // BOOST_ASSIGN_V2_OPTION_DATA_ER_2011_HPP

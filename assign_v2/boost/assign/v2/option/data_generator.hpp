//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_DATA_GENERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_DATA_GENERATOR_ER_2010_HPP
#include <boost/assign/v2/detail/functor/value.hpp>
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/data_generator.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/interpreter/replace.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
//[syntax_option_data_generator
namespace interpreter_aux{

    template<
        typename C // Value or pointer-container
    >
    struct deduce_key_generator/*<-*/
    {
        typedef typename container_aux::key<C>::type key_;
        typedef functor_aux::value<key_> type;
    }/*->*/;

namespace result_of{

    template<typename D, typename C, typename F = keyword_aux::use_default>
    struct option_data_generator/*<-*/
        : ::boost::mpl::apply1<
            interpreter_aux::replace_data_generator<D>, 
            F
        >
    {}/*->*/;

    template<typename D, typename C>
    struct option_data_generator<D, C, keyword_aux::use_default>/*<-*/
        : option_data_generator<
            D, C, typename deduce_data_generator<C>::type
        >
    {}/*->*/;
    
    template<typename D, typename C>
    struct option_data_generator<D, C, keyword_aux::key>/*<-*/
        : option_data_generator<
            D, C, typename deduce_key_generator<C>::type
        >
    {}/*->*/;

    
}// result_of

    template<typename F/*<-*/= keyword_aux::ignore/*->*/>
    struct option_data_generator/*<-*/
    {
        option_data_generator(){}
        option_data_generator(F f) : f_( f ){}

        template<typename C>
        F const& data_generator()const{ return this->f_; }
        
        private:
        F f_;
    }/*->*/;

    template<>
    struct option_data_generator<use_default>/*<-*/
    {
        option_data_generator(){}
        option_data_generator(keyword_aux::ignore){}

        template<typename C>
        typename deduce_data_generator<C>::type 
        data_generator()const
        { 
            return typename deduce_data_generator<C>::type(); 
        }
        
    }/*->*/;

    template<>
    struct option_data_generator<keyword_aux::key>/*<-*/
    {
        option_data_generator(){}
        option_data_generator(keyword_aux::ignore){}

        template<typename C>
        typename deduce_key_generator<C>::type 
        data_generator()const
        { 
            return typename deduce_key_generator<C>::type(); 
        }
        
    }/*->*/;

    // Overrides data generator
    template<typename C, typename F, typename ModifierTag
        , typename DataTag, typename D, typename F1>
    typename result_of::option_data_generator<D, C, F1>::type
    operator%(
        interpreter_crtp<C, F, ModifierTag, DataTag, D> const& lhs,
        option_data_generator<F1> const& rhs
    )/*<-*/
    {
        typedef typename result_of::option_data_generator<
            D, C, F1
        >::type result_;
        return result_( 
            lhs.container(), 
            rhs.template data_generator<C>(), 
            lhs.modifier 
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    struct keyword_data_generator
    /*<-*/{

        template<typename F>
        option_data_generator<F> operator=(F const& f)const{
            return option_data_generator<F>( f );
        }

    }/*->*/;

}// interpreter_aux
namespace{
    const interpreter_aux::keyword_data_generator _data/*<-*/ 
        = interpreter_aux::keyword_data_generator()/*->*/;
}
namespace result_of{

    template<typename D, typename C, typename F>
    struct option_data_generator/*<-*/
        : interpreter_aux::result_of::option_data_generator<D, C, F>
    {}/*->*/;

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
    option_data_generator< FUN > NAME()\
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

#include <boost/assign/v2/detail/functor/value.hpp>
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

#endif // BOOST_ASSIGN_V2_OPTION_DATA_GENERATOR_ER_2010_HPP

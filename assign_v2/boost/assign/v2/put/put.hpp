//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PUT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_PUT_ER_2011_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/interpreter/data.hpp>
#include <boost/assign/v2/interpreter/modifier.hpp>
#include <boost/assign/v2/interpreter/replace.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put
namespace interpreter_aux{

    template<typename C, typename F, typename MTag, typename DTag>
    class put_interpreter :
//<-
        protected ref::copy_wrapper< C >::type,
//->
        public interpreter_aux::interpreter_crtp< 
            put_interpreter<C, F, MTag, DTag> 
            , C, F, MTag, DTag
        >
    {
//<-
        typedef interpreter_aux::interpreter_crtp< 
            put_interpreter
            , C, F, MTag, DTag 
        > super2_t;
//->
        public:

        typedef /*<-*/ typename super2_t::result_type 
            BOOST_ASSIGN_V2_IGNORE(/*->*/ unspecified /*<-*/)/*->*/result_type;
//<-
        protected:

        typedef interpreter_aux::interpreter_modifier<MTag> modifier_;
        typedef ref::assign_tag::copy assign_tag_;
        typedef ref::wrapper<assign_tag_,C> super1_t;
//->
        public:

        put_interpreter()/*<-*/
            {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit put_interpreter( C& cont )/*<-*/
            : super1_t( cont )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        put_interpreter( C& cont, F const& f, modifier_ const& m )/*<-*/
            : super1_t( cont ), super2_t( f, m )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        C& container()const/*<-*/{
            return static_cast<super1_t const&>(*this).get();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    };

    template<typename C, typename F, typename MTag, typename DTag>
    struct replace_data_generator< 
        interpreter_aux::put_interpreter<C, F, MTag, DTag> 
    >{
        template<typename F1>
        struct apply/*<-*/{ 
            typedef interpreter_aux::put_interpreter<
                C, F1, MTag, DTag
            > type; 
        }/*->*/;
    };

    template<typename C, typename F, typename MTag, typename DTag>
    struct replace_modifier_tag< 
        interpreter_aux::put_interpreter<C, F, MTag, DTag> 
    >{
        template<typename MTag1>
        struct apply/*<-*/{ 
            typedef interpreter_aux::put_interpreter<
                C, F, MTag1, DTag
            > type; 
        }/*->*/;
    };

    template<typename C, typename F, typename MTag, typename DTag>
    struct replace_data_tag< 
        interpreter_aux::put_interpreter<C, F, MTag, DTag> 
    >{
        template<typename DTag1>
        struct apply/*<-*/{ 
            typedef interpreter_aux::put_interpreter<
                C, F, MTag, DTag1
            > type; 
        }/*->*/;
    };

}// interpreter_aux
namespace result_of{

    template<
        typename C/*<-*/
        , typename DGen 
            = typename interpreter_aux::deduce_data_generator<C>::type 
    >
    struct put/*<-*/{
        typedef 
            interpreter_aux::put_interpreter<
                C
                , DGen
                , typename interpreter_aux::deduce_modifier_tag<C>::type
                , typename interpreter_aux::deduce_data_tag<C>::type
            > 
        type;
    }/*->*/;

}// result_of

    template<typename C>
    typename result_of::put<C>::type
    put( C& cont )/*<-*/
    {
        typedef typename result_of::put<C>::type result_;
        return result_( cont );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PUT_ER_2011_HPP

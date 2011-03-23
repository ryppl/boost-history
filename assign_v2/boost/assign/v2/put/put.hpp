//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PUT_ER_2010_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/interpreter/data.hpp>
#include <boost/assign/v2/interpreter/modifier.hpp>
#include <boost/assign/v2/interpreter/replace.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_put
namespace aux{

    template<typename C, typename F, typename Tag>
    class put_interpreter
        : protected ref::wrapper< ref::assign_tag::copy, C >,
//<-
        public aux::interpreter_crtp< C, F, Tag, put_interpreter<C, F, Tag> >
//->
    {
//<-
        typedef aux::interpreter_crtp< C, F, Tag, put_interpreter > super2_t;
//->
        public:

        typedef /*<-*/ typename super2_t::result_type BOOST_ASSIGN_V2_IGNORE(/*->*/ unspecified /*<-*/) /*->*/result_type;
//<-
        protected:

        typedef aux::adapter_modifier<Tag> modifier_;
        typedef ref::assign_tag::copy assign_tag_;
        typedef ref::wrapper<assign_tag_,C> super1_t;
//->
        public:

        put_interpreter()/*<-*/
            {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit put_interpreter( C& cont )/*<-*/
            : super1_t( cont )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit put_interpreter( C& cont, F const& f, modifier_ const& m )/*<-*/
            : super1_t( cont ), super2_t( f, m )
            {
                // This constructor is required in conjunction with modulo
            }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        C& container()const/*<-*/{
            return static_cast<super1_t const&>(*this).get();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    };

    template<typename C, typename F, typename Tag>
    struct /*<<Metafunction class>>*/replace_data_generator< aux::put_interpreter<C, F, Tag> >{/*<-*/
        template<typename F1>
        struct apply{ typedef aux::put_interpreter<C, F1, Tag> type; };
    /*->*/};

    template<typename C, typename F, typename Tag>
    struct /*<<Metafunction class>>*/replace_modifier_tag< aux::put_interpreter<C, F, Tag> >{/*<-*/
        template<typename Tag1>
        struct apply{ typedef aux::put_interpreter<C, F, Tag1> type; };
    /*->*/};

}// aux
namespace result_of{

    template<typename C>
    struct /*<<Metafunction>>*/put/*<-*/{
        typedef typename aux::deduce_data_generator<C>::type f_;
        typedef typename aux::deduce_modifier_tag<C>::type modifier_tag_;
        typedef aux::put_interpreter<C, f_, modifier_tag_> type;
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

#endif // BOOST_ASSIGN_V2_PUT_ER_2010_HPP

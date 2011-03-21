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
#include <boost/assign/v2/framework/adapter_crtp.hpp>
#include <boost/assign/v2/framework/modifier.hpp>
#include <boost/assign/v2/framework/replace_parameter.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/fun/deduce.hpp>
#include <boost/assign/v2/optional/deduce.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_put
namespace aux{

    template<typename C, typename F, typename Tag>
    class container_adapter
//<-
        : protected ref::wrapper< ref::assign_tag::copy, C >
        , public aux::adapter_crtp< C, F, Tag, container_adapter<C, F, Tag> >
//->
    {
//<-
        typedef aux::adapter_crtp< C, F, Tag, container_adapter > super2_t;
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

        container_adapter()/*<-*/
            {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit container_adapter( C& cont )/*<-*/
            : super1_t( cont )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit container_adapter( C& cont, F const& f, modifier_ const& m )/*<-*/
            : super1_t( cont ), super2_t( f, m )
            {
                // This constructor is required in conjunction with modulo
            }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        C& container()const/*<-*/{
            return static_cast<super1_t const&>(*this).get();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    };

    template<typename C, typename F, typename Tag>
    struct /*<<Meta-function class>>*/replace_fun< aux::container_adapter<C, F, Tag> >{/*<-*/
        template<typename F1>
        struct apply{ typedef aux::container_adapter<C, F1, Tag> type; };
    /*->*/};

    template<typename C, typename F, typename Tag>
    struct /*<<Meta-function class>>*/replace_modifier_tag< aux::container_adapter<C, F, Tag> >{/*<-*/
        template<typename Tag1>
        struct apply{ typedef aux::container_adapter<C, F, Tag1> type; };
    /*->*/};

}// aux
namespace result_of{

    template<typename C>
    struct /*<<Meta-function>>*/put{/*<-*/
        typedef typename aux::deduce_fun<C>::type f_;
        typedef typename aux::deduce_modifier_tag<C>::type modifier_tag_;
        typedef aux::container_adapter<C, f_, modifier_tag_> type;
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

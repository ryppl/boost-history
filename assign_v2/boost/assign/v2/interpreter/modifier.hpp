//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/support/switch.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/option/modifier/insert.hpp>
#include <boost/assign/v2/option/modifier/iterate.hpp>
#include <boost/assign/v2/option/modifier/row_major.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_modifier
namespace modifier_tag{

    // Namespace in which to declare template arguments of interpreter_modifier<>

}//modifier_tag
namespace interpreter_aux{


    template<typename MTag>
    struct interpreter_modifier
    {
        // Specialize on Tag to model ConceptModifier 
    };

    template<typename MTag, typename DTag, typename C, typename T>
    struct ConceptModifier
    {

        BOOST_CONCEPT_USAGE(ConceptModifier)
        {
            m.impl( cont, t, DTag() ); 
        }

        private:
        static interpreter_aux::interpreter_modifier<MTag>& m;
        static C& cont;
        static T t;
    };

}//interpreter_aux
/*<-*/

namespace switch_tag{
    struct modifier_tag{};
}// switch_tag

#define BOOST_ASSIGN_V2_SWITCH_TAG modifier_tag
BOOST_ASSIGN_V2_SWITCH_CASE( 0, container_aux::is_multi_array, modifier_tag::row_major<> )
BOOST_ASSIGN_V2_SWITCH_CASE( 1, container_aux::is_associative, modifier_tag::insert )
BOOST_ASSIGN_V2_SWITCH_CASE( 2, container_aux::is_array, modifier_tag::iterate<> )
BOOST_ASSIGN_V2_SWITCH_CASE( 3, container_aux::has_push_deduced_value, modifier_tag::push )
BOOST_ASSIGN_V2_SWITCH_CASE_DEFAULT( 4, modifier_tag::push_back )
#undef BOOST_ASSIGN_V2_SWITCH_TAG

/*->*/
namespace interpreter_aux{

    template<
        typename C // Container
    >
    struct deduce_modifier_tag/*<-*/
        : switch_aux::result< switch_tag::modifier_tag, C>
    {}/*->*/;

//<-
    // For testing purposes
    template<typename C, typename X>
    void check_deduce()
    {
        typedef typename interpreter_aux::deduce_modifier_tag<C>::type found_;
        BOOST_MPL_ASSERT(( boost::is_same<found_, X> ));
    }

//->

    template<
        typename C // Container
    >
    struct deduce_data_tag/*<-*/{
    
        typedef typename ::boost::mpl::if_<
            container_aux::is_ptr_container<C>,
            data_tag::storage_ptr,
            data_tag::storage_value
        >::type storage_;
    
        typedef typename ::boost::mpl::if_<
            container_aux::is_map<C>,
            data_tag::assign_map,
            data_tag::assign
        >::type assign_;

        typedef data_tag::plus<storage_, assign_> type;    
    }/*->*/;

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2011_HPP

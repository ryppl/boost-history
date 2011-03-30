//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/option/modifier/insert.hpp>
#include <boost/assign/v2/option/modifier/iterate.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
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


    template<typename ModifierTag>
    struct interpreter_modifier
    {
        // Specialize on Tag to model ConceptModifier 
    };

    template<typename ModifierTag, typename DataTag, typename C, typename T>
    struct ConceptModifier
    {

        BOOST_CONCEPT_USAGE(ConceptModifier)
        {
            m.impl( cont, t, DataTag() ); 
        }

        private:
        static interpreter_aux::interpreter_modifier<ModifierTag>& m;
        static C& cont;
        static T t;
    };

}//interpreter_aux
/*<-*/

namespace switch_tag{
    struct deduce_put{};
}// switch_tag

#ifdef BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE
#error
#else
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(Tag, BooleanMetaF, CaseNumber)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, CaseNumber> :\
        switch_aux::helper<\
            v2::modifier_tag::Tag,\
            BooleanMetaF\
        >{};\
}\
/**/
#endif

#ifdef BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_DEFAULT
#error
#else
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_DEFAULT(CaseNumber)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, CaseNumber> :\
        switch_aux::helper<v2::modifier_tag::push_back>\
    {};\
}\
/**/
#endif

#ifdef BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH
#warning
#else
BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(insert, container_aux::is_associative, 0)
BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(iterate<>, container_aux::is_array, 1)
BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(push, container_aux::has_push_deduced_value, 2)
BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_DEFAULT(3)
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH
#endif

/*->*/
namespace interpreter_aux{

    template<
        typename C // Container
    >
    struct deduce_modifier_tag/*<-*/
        : switch_aux::result< switch_tag::deduce_put, C>
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

#endif // BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2010_HPP

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
#include <boost/assign/v2/detail/traits/value_container/has_push.hpp>
#include <boost/assign/v2/detail/traits/value_container/category.hpp>
#include <boost/assign/v2/detail/traits/ptr_container/meta.hpp>
#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/option/insert.hpp>
#include <boost/assign/v2/option/iterate.hpp>
#include <boost/assign/v2/option/push.hpp>
#include <boost/assign/v2/option/push_back.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/assert.hpp>
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

    template<typename Tag>
    struct interpreter_modifier
    {
        // Specialize on Tag to model ConceptModifier 
    };

    template<typename Tag, typename C, typename T>
    struct ConceptModifier
    {

        BOOST_CONCEPT_USAGE(ConceptModifier)
        {
            m.impl( cont, t ); 
        }

        private:
        static interpreter_aux::interpreter_modifier<Tag>& m;
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
            container_aux::through_value_container<BooleanMetaF>::apply\
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
}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_MODIFIER_ER_2010_HPP

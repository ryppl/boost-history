//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_FRAMEWORK_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_FRAMEWORK_MODIFIER_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/traits/value_container/has_push.hpp>
#include <boost/assign/v2/detail/traits/value_container/category.hpp>
#include <boost/assign/v2/detail/traits/ptr_container/meta.hpp>
#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/optional/insert.hpp>
#include <boost/assign/v2/optional/iterate.hpp>
#include <boost/assign/v2/optional/push.hpp>
#include <boost/assign/v2/optional/push_back.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_framework_modifier
namespace /*<< Template arguments to `aux::adapter_modifier<>` have to be declared within this `namespace`>>*/modifier_tag{}
namespace aux{

    template<typename Tag>
    struct /*<<Specialize on Tag>>*/ adapter_modifier{};

    template<typename C>
    struct /*<<Meta-function returning a modifier-tag>>*/deduce_modifier_tag;

}// aux
namespace put_concept{

    template<typename Tag, /*<<Container>>*/ typename C, /*<<Input>>*/ typename T>
    struct Modifier
    {

        BOOST_CONCEPT_USAGE(Modifier)
        {
            m.impl( cont, t ); 
        }

        private:
        static aux::adapter_modifier<Tag>& m;
        static C& cont;
        static T t;
    };

}
//]
namespace switch_tag{
    struct deduce_put{};
}// switch_tag

#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_CASE
#error
#else
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_CASE(Tag, MetaF, Number)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, Number> :\
        switch_aux::helper<\
            v2::modifier_tag::Tag,\
            ptr_container_aux::through_value_container<MetaF>::apply\
        >{};\
}\
/**/
#endif


#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ASSOCIATIVE
#error
#else
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ASSOCIATIVE(CaseNumber)\
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_CASE(insert, value_container_aux::is_sorted, CaseNumber)\
/**/
#endif

#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ARRAY
#error
#else
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ARRAY(CaseNumber)\
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_CASE(iterate<>, value_container_aux::is_array, CaseNumber)
/**/
#endif

#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ADAPTER
#error
#else
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ADAPTER(CaseNumber)\
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_CASE(push, value_container_aux::has_push_deduced_value, CaseNumber)
/**/
#endif

#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_DEFAULT
#error
#else
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_DEFAULT(CaseNumber)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, CaseNumber> :\
        switch_aux::helper<v2::modifier_tag::push_back>\
    {};\
}\
/**/
#endif

//[example_put_modifier_deduce_switch
#ifdef BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH
#warning
#else
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ASSOCIATIVE(0)
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ARRAY(1)
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_ADAPTER(2)
BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH_DEFAULT(3)
#define BOOST_ASSIGN_V2_OPTIONAL_MODIFIER_SWITCH
//]
#endif

namespace aux{

    template<typename C>
    struct deduce_modifier_tag
        : switch_aux::result< switch_tag::deduce_put, C>
    {};

    // For testing purposes
    template<typename C, typename X>
    void check_deduce()
    {
        typedef typename aux::deduce_modifier_tag<C>::type found_;
        BOOST_MPL_ASSERT(( boost::is_same<found_, X> ));
    }

}// aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_FRAMEWORK_MODIFIER_ER_2010_HPP

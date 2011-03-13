//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/traits/container/has_push.hpp>
#include <boost/assign/v2/detail/traits/container/is_associative.hpp>
#include <boost/assign/v2/detail/traits/container/is_array.hpp>
#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/put/modifier/insert.hpp>
#include <boost/assign/v2/put/modifier/iterate.hpp>
#include <boost/assign/v2/put/modifier/push.hpp>
#include <boost/assign/v2/put/modifier/push_back.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{
    struct deduce_put{};
}// switch_tag

#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_CASE
#error
#else
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_CASE(Tag, MetaFun, Number)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, Number> :\
        switch_aux::helper<\
            v2::modifier_tag::Tag,\
            MetaFun\
        >{};\
}\
/**/
#endif

#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ASSOCIATIVE
#error
#else
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ASSOCIATIVE(CaseNumber)\
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_CASE(insert, container_traits::is_associative, CaseNumber)\
/**/ 
#endif

#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ARRAY
#error
#else
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ARRAY(CaseNumber)\
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_CASE(iterate<>, container_traits::is_array, CaseNumber)
/**/
#endif

#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ADAPTER
#error
#else
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ADAPTER(CaseNumber)\
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_CASE(push, container_traits::has_push_deduced_value, CaseNumber)
/**/
#endif

#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_DEFAULT
#error
#else
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_DEFAULT(CaseNumber)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::deduce_put, CaseNumber> :\
        switch_aux::helper<v2::modifier_tag::push_back>\
    {};\
}\
/**/
#endif

//[example_put_modifier_deduce_switch
#ifdef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH
#warning  
#else
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ASSOCIATIVE(0)
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ARRAY(1)
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_ADAPTER(2)
BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH_DEFAULT(3)
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_SWITCH 
//]
#endif

//[syntax_put_modifier_deduce
namespace put_aux{

    template<typename C>
    struct /*<<Meta-function returning a modifier-tag>>*/deduce_modifier_tag
//<-    
        : switch_aux::result< switch_tag::deduce_put, C>
//->
    {};

}// put_aux
//]

namespace put_aux{

    // For testing purposes
    template<typename C, typename X>
    void check_deduce()
    {
        typedef typename put_aux::deduce_modifier_tag<C>::type found_;
        BOOST_MPL_ASSERT(( boost::is_same<found_, X> ));
    }

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_ER_2010_HPP

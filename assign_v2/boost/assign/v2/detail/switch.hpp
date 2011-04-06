//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_SWITCH_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_SWITCH_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{

    // Each tag in this namespace identifies a switch statement.

}//switch_tag
namespace switch_aux{

    template<typename SwitchTag, int CaseNumber> struct case_
    {
    	// Specialize like this:
        // template<>
    	// struct case_<SwitchTag, CaseNumber> : helper<Value, Predicate>{};
    };

    template<typename Expression>
    struct always_true_ 
    	: ::boost::mpl::true_
    {};

    template<typename Value, template<typename> class Predicate = always_true_>
    struct helper
    {
        typedef Value value;
        
        template<typename Expression> 
        struct apply 
        	: Predicate<Expression>
        {};
        
    };

    template<typename Helper>
    struct extract_value
    {
        typedef typename Helper::value type;
    };

    template<typename SwitchTag, typename Expression, int CaseNumber = 0>
    struct result : ::boost::mpl::eval_if<
        ::boost::mpl::apply1< 
        	switch_aux::case_<SwitchTag, CaseNumber>, 
            Expression 
        >,
        switch_aux::extract_value< 
        	switch_aux::case_<SwitchTag, CaseNumber> 
        >,
        switch_aux::result< 
        	SwitchTag, Expression, CaseNumber + 1
        >
    >{};

}// switch_aux

#define BOOST_ASSIGN_V2_SWITCH_CASE(CaseNumber, BooleanMetaF, Value)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::BOOST_ASSIGN_V2_SWITCH_TAG, CaseNumber>\
        : switch_aux::helper<\
            Value,\
            BooleanMetaF\
        >{};\
}\
/**/

#define BOOST_ASSIGN_V2_SWITCH_CASE_DEFAULT(CaseNumber, Value)\
namespace switch_aux{\
    template<>\
    struct case_<switch_tag::BOOST_ASSIGN_V2_SWITCH_TAG, CaseNumber>\
        : switch_aux::helper<Value>\
    {};\
}\
/**/

// Usage:
// 	#define BOOST_ASSIGN_V2_SWITCH_TAG unspecified
// for i = 0,...,n-1
// 	BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(i, Fi, Valuei)
// and
// 	BOOST_ASSIGN_V2_OPTION_MODIFIER_SWITCH_CASE(n, Valuen)

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_TRAITS_SWITCH_ER_2010_HPP

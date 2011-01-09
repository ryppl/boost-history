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
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/assign/v2/detail/mpl/switch.hpp>

#include <boost/assign/v2/detail/type_traits/container/is_static_array.hpp>
#include <boost/assign/v2/detail/type_traits/container/has_push.hpp>
#include <boost/assign/v2/detail/type_traits/container/is_associative.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace switch_tag{
	struct deduce_put{};
}// switch_tag
namespace modifier_tag{
	//struct at_next;
    struct iterate;
    struct insert;
    struct push;
    struct push_back;
}// modifier_tag
namespace mpl{

    template<>
    struct case_<switch_tag::deduce_put,0> :
        v2::mpl::wrap<
            v2::modifier_tag::insert,
            v2::container_type_traits::is_associative
        >{};

    template<>
    struct case_<switch_tag::deduce_put,1> :
        v2::mpl::wrap<
//            v2::modifier_tag::at_next,
			v2::modifier_tag::iterate,
            v2::container_type_traits::is_static_array
        >{};

    template<>
    struct case_<switch_tag::deduce_put,2> :
        v2::mpl::wrap<
        	v2::modifier_tag::push,
            container_type_traits::has_push_deduced_value
        >{};

    template<>
    struct case_<switch_tag::deduce_put,3> : 
        v2::mpl::wrap<v2::modifier_tag::push_back>{};

}// mpl
namespace put_aux{

    template<typename T>
    struct deduce_modifier : v2::mpl::switch_<
    	v2::switch_tag::deduce_put,
    	T
    >
    {};

	struct kwd_deduce{}; 

	template<typename T>
    typename T::result_of_modulo::deduce::type
	operator%(
    	T const& t, 
        kwd_deduce const& kwd
    )
    {
    	return t.modulo_deduce();
    }

	template<typename Option1,typename Option2>
	struct deduce : boost::mpl::eval_if<
    	boost::is_same<Option1, boost::use_default>,
    	boost::mpl::identity<Option2>,
    	boost::mpl::identity<Option1>
	>{};

	// For testing purposes
	template<typename C, typename X>
	void check_deduce()
    {
        typedef typename put_aux::deduce_modifier<C>::type found_;
        BOOST_MPL_ASSERT(( boost::is_same<found_, X> ));
    }

}// put_aux
namespace{
	const put_aux::kwd_deduce _deduce = put_aux::kwd_deduce();
}	
}// v2
}// assign
}// boost

#endif

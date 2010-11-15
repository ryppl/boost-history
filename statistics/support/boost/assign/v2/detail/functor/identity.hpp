//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_IDENTITY_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_IDENTITY_ER_2010_HPP
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

    struct result_of_identity{

        template<typename V>
		struct is_ok : boost::mpl::equal_to<
       		boost::mpl::int_<1>,
       		boost::mpl::size<V>
    	>{};

    	template<typename V>
        struct enable
        {
        	typedef typename boost::mpl::at_c<V,0>::type t_;
            typedef typename boost::add_reference<t_>::type type;
        };

    	template<typename V>
        struct disable{};

        template<typename V>
        struct apply : boost::mpl::eval_if<
            is_ok<V>,
            enable<V>,
            disable<V>
        >{};

    };

	struct identity : functor_aux::crtp_unary_and_up<
        functor_aux::identity,
        functor_aux::result_of_identity
    >
    {
    	identity(){}

    	template<typename T> T& impl(T& t)const{ return t; }
    	template<typename T> T const& impl(T const& t)const{ return t; }

    };

}// functor_aux
namespace{
	functor_aux::identity const _identity = functor_aux::identity();
}
}// v2
}// assign
}// boost

#endif

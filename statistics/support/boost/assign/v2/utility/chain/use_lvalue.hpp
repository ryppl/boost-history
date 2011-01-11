//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_USE_LVALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_USE_LVALUE_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/reference.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename U1,typename U2,typename Tag = use_default> 
    struct use_lvalue
    {
    	
        typedef typename boost::range_reference<U1>::type r1_;
        typedef typename boost::range_reference<U2>::type r2_;
        typedef boost::mpl::and_<
            boost::is_reference<r1_>,
            boost::is_reference<r2_>
        > are_refs_;
        
        typedef boost::is_same<r1_,r2_> are_same_;
        
        typedef boost::mpl::and_<
             are_refs_,
             are_same_
        > are_same_refs_;
        
        typedef boost::is_const<
            typename boost::remove_reference<r1_>::type
        > is_const_;
        
        typedef typename boost::mpl::eval_if<
            is_const_,
            boost::mpl::identity<boost::mpl::false_>,
            are_same_refs_
        >::type type;
        
        BOOST_STATIC_CONSTANT(bool, value = type::value);
    };

}// chain_aux
}// v2
}// assign
}// boost

#endif

//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_TRAITS_RESULT_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_TRAITS_RESULT_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/assign/v2/chain/traits/use_lvalue.hpp>
#include <boost/assign/v2/chain/traits/range.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename R1,typename R2,typename Tag = use_default> 
    struct result{
    	typedef typename  boost::mpl::eval_if<
        	chain_traits::use_lvalue<R1,R2,Tag>,
        	boost::mpl::identity< chain_traits::range_l<R1, R2, Tag> >,
        	boost::mpl::identity< chain_traits::range_r<R1, R2, Tag> >
    	>::type caller_;
        
        typedef typename caller_::type type;
        
        static type call(R1& r1, R2& r2)
        {
            return caller_::call( r1, r2 );
        }
    };

}// chain_aux
}// v2
}// assign
}// boost

#endif

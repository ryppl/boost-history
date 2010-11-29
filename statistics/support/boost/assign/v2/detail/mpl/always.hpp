//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_MPL_ALWAYS_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_MPL_ALWAYS_ER_2010_HPP
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

// TODO remove file

namespace boost{
namespace assign{ 
namespace v2{
namespace mpl{
	
    // This is a variant of mpl::always that is also nullary.

	template<typename T>
	struct always : boost::mpl::always<T>{};
    
}// mpl
}// v2
}// assign
namespace mpl{

    template<typename T>
    struct apply0< 
    	assign::v2::mpl::always<T> 
    > : boost::mpl::identity<T>{};

}// mpl
}// boost

#endif

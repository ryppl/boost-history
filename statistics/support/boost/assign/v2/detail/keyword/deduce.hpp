//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_DEDUCE_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/identity.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace keyword_aux{ 
	struct deduce{}; 

	template<typename T>
    typename T::result_of_modulo::deduce::type
	operator%(
    	T const& t, 
        keyword_aux::deduce const& deduce
    )
    {
    	return t.modulo_deduce();
    }

}// keyword_aux
namespace{
	const keyword_aux::deduce _deduce = keyword_aux::deduce();
}	
namespace detail{

	template<typename Option1,typename Option2>
	struct deduce : boost::mpl::eval_if<
    	boost::is_same<Option1,boost::use_default>,
    	boost::mpl::identity<Option2>,
    	boost::mpl::identity<Option1>
	>{};

}// detail
}// v2
}// assign
}// boost

#endif

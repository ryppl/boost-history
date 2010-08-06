//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_LOOKUP_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_LOOKUP_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace keyword_aux{ 

	struct lookup{};
    struct incr_lookup{};

	template<typename T>
	typename T::result_of_modulo::incr_lookup::type
	operator%(
    	T const& t, 
        keyword_aux::incr_lookup const& dummy
    )
    {	
    	return t.modulo_incr_lookup();
    }

}// detail

	keyword_aux::lookup const _lookup = keyword_aux::lookup();
	keyword_aux::incr_lookup const _incr_lookup = keyword_aux::incr_lookup();

}// v2
}// assign
}// boost

#endif

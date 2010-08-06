//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_REPEAT_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_REPEAT_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace keyword_aux{ 

	struct repeat{
    	
        typedef std::size_t size_type;

        repeat() : value( 0 ){}

		repeat operator=( size_type const& n)const{ 
        	repeat result;
            result.value = n;
			return result;
        }
        
        size_type value;
    };

	template<typename T>
	typename T::result_of_modulo::repeat::type
	operator%(
    	T const& t, 
        keyword_aux::repeat const& h
    )
    {	
		return t.modulo_repeat( h.value );
    }

}// detail

	keyword_aux::repeat const _repeat = keyword_aux::repeat();

}// v2
}// assign
}// boost

#endif

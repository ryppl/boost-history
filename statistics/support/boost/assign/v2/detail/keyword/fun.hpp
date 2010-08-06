//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_FUN_ER_2010_HPP

// Usage
//	_fun = f
// return fun_holder<F>

namespace boost{
namespace assign{ 
namespace v2{
namespace detail{ 

    template<typename F>
    struct fun_holder{
    	
        fun_holder(const F& f):value(f){}
        
        F value;
    };

	template<typename T,typename F1>
    typename T::result_of_modulo::template fun<F1>::type
	operator%(
    	T const& t, 
        detail::fun_holder<F1> const& h
    )
    {
    	return t.modulo_fun( h.value );
    }

}// detail
namespace keyword_aux{

	struct fun{
    
		template<typename F>
    	detail::fun_holder<F>
		operator=(F const& f)const
    	{
			return detail::fun_holder<F>( f );
    	}

    }; 
}// keyword_aux
namespace{
	const keyword_aux::fun _fun = keyword_aux::fun();
}	
}// v2
}// assign
}// boost

#endif

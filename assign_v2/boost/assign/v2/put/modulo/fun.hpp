//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_FUN_ER_2010_HPP
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{

	template<typename D> struct fun/* Specialize on D(erived) */{
		template<typename F> struct apply{};
    };

}// result_of_modulo
namespace put_modulo_aux{

    template<typename F>
    struct fun
    {
        fun(const F& f) : value(f){}
        F value;
    };

    template<typename T, typename F1>
    typename ::boost::mpl::apply1<result_of_modulo::fun<T>, F1>::type
	operator%(T const& t, put_modulo_aux::fun<F1> const& h)
    {
    	typedef result_of_modulo::fun<T> meta_;
    	typedef typename ::boost::mpl::apply1<meta_, F1>::type result_;
        return result_( t.container(), h.value, t.modifier );
    }

    struct kwd_fun{
    
        template<typename F>
    	put_modulo_aux::fun<F> operator=(F const& f)const
    	{
            return put_modulo_aux::fun<F>( f );
    	}

    }; 
    
}// put_modulo_aux
namespace{
	const put_modulo_aux::kwd_fun _fun = put_modulo_aux::kwd_fun();
}	
}// v2
}// assign
}// boost

#endif

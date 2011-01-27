//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_RESULT_OF_MODULO_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_RESULT_OF_MODULO_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace result_of_modulo{

	//  Specialize on T's that derives from crtp
	template<typename T> struct new_fun{
    
    	template<typename F> struct apply{};
    
    };

	//  Specialize on T's that derives from crtp
	template<typename T> struct new_modifier{
    
    	template<typename Tag> struct apply{};

    };

	template<typename T> struct new_fun_modifier
    {
    
    	template<typename F, typename Tag>
        struct apply
        {
        	typedef typename ::boost::mpl::apply1<
            	new_fun<T>,
                F
            >::type new_fun_;
            typedef result_of_modulo::new_modifier<new_fun_> new_modifier_;
            typedef typename ::boost::mpl::apply1<
            	new_modifier_,
                Tag
            >::type type;
        };
    
    };
    
    template<typename T>
    struct generic_
    {
    
    	template<typename P>
        struct apply
        {
        	static T t;
            static P p;
            typedef BOOST_TYPEOF_TPL( t % p ) type;
        };
    
    };
    
    
}// result_of_modulo
}// v2
}// assign
}// boost

#endif

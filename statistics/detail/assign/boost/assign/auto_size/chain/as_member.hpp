//////////////////////////////////////////////////////////////////////////////
// assign::detail::array::chainable.hpp                                     //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CHAINABLE_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CHAINABLE_ER_2010_HPP

namespace boost{
namespace assign{

namespace result_of{
namespace chain_auto_convert{
    template<typename R1,typename R2>
    struct convert_wrapper;  
}// chain_auto_convert
}// result_of

template<typename R1,typename R2> 
typename result_of::chain_auto_convert::convert_wrapper<R1,R2>::type
chain_auto_convert_convert_wrapper(R1& r1,R2& r2);

namespace detail{

    // D is a derived type
	template<typename D>
	struct chain_as_member{

        template<typename D1,typename R>
        struct result_of_chain_auto_convert 
            : assign::result_of::chain_auto_convert::convert_wrapper<D1,R>{}; 

		template<typename R>
        typename result_of_chain_auto_convert<D,R>::type
		chain_auto_convert(R& r){ 
            return assign::chain_auto_convert_convert_wrapper(
                static_cast<D&>(*this),
                r
            ); 
        }

		template<typename R>
        typename result_of_chain_auto_convert<const D,R>::type
		chain_auto_convert(R& r)const{ 
            return assign::chain_auto_convert_convert_wrapper(
                static_cast<const D&>(*this),
                r
            ); 
        }

	};
    
}// detail
}// assign
}// boost

#endif
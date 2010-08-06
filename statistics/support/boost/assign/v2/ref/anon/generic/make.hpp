//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_GENERIC_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_GENERIC_MAKE_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/anon/generic/nth_result_of.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename Tag2,typename T>
    struct empty_generic_anon 
    	: nth_result_of::generic_anon<0, Tag1, Tag2, T>{};

	template<typename Tag1,typename Tag2,typename T>
    struct generic_anon : nth_result_of::generic_anon<1, Tag1, Tag2, T>{};
    
}// result_of

	template<typename Tag1,typename Tag2,typename T>
    typename result_of::empty_generic_anon<Tag1, Tag2, T>::type
	generic_anon( keyword_aux::nil )
    {
    	typedef typename result_of::empty_generic_anon<
        	Tag1, Tag2, T>::type result_;
    	return result_();
    }

    template<typename Tag1,typename Tag2,typename T>
	typename result_of::generic_anon<Tag1, Tag2, T>::type
    generic_anon(T& t)
    {
    	return generic_anon<Tag1, Tag2, T>( v2::_nil )( t );
    }

    template<typename Tag1,typename Tag2,typename T>
	typename result_of::generic_anon<Tag1, Tag2, T const>::type
    generic_anon(T const& t)
    {
    	return generic_anon<Tag1, Tag2, T const>( v2::_nil )( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif

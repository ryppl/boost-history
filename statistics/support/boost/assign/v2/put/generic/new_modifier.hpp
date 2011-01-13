//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_NEW_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_NEW_MODIFIER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/void.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

	template<typename Tag> struct set_modifier
    {
    
    	set_modifier(){}
        set_modifier(Tag const& t) : modifier_tag( t ){}

    	Tag modifier_tag;
    };

	struct new_modifier
    {

		template<typename Tag>    	
        set_modifier<Tag> operator=( Tag const& t)const
        {
        	return set_modifier<Tag>( t );
        }

    };

	template<typename T,typename NewTag>
    typename boost::mpl::apply1<
    	v2::result_of_modulo::new_modifier<T>,
        NewTag
    >::type
    operator%(
    	T const& t, 
        put_aux::set_modifier<NewTag> const& h
    )
    {
    	typedef typename boost::mpl::apply1<
    		v2::result_of_modulo::new_modifier<T>,
        	NewTag
    	>::type result_;
        typedef put_aux::modifier<NewTag> modifier_;
        return result_( t.get(), t.fun, modifier_( h.modifier_tag ) );
    }

}// put_aux
namespace{
	put_aux::new_modifier const _modifier = put_aux::new_modifier();
}
}// v2
}// assign
}// boost

#endif

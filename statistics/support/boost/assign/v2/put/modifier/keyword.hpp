//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_KEYWORD_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_KEYWORD_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

	template<typename Tag> struct put_tag{};

	struct keyword{

		template<typename Tag>    	
        put_tag<Tag> operator=( Tag const& dummy)const
        {
        	return put_tag<Tag>();
        }

    };

	template<typename T,typename NewTag>
    typename T::result_of_modulo::template put_tag<NewTag>::type
    operator%(
    	T const& t, 
        put_aux::put_tag<NewTag> const& h
    )
    {
    	return t.template modulo_put_tag<NewTag>();
    }

}// put_aux
namespace{
	put_aux::keyword const _put_tag = put_aux::keyword();
}
}// v2
}// assign
}// boost

#endif

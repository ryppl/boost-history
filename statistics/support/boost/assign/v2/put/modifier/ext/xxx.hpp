//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_XXX_EXT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_XXX_EXT_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo_ext{

    template<typename T, template<typename> class Tag>
    struct same_fun : result_of_modulo_aux::traits<T, Tag>{

		typedef traits<T,Tag> super_t;
		typedef result_of_modulo::new_modifier<T> meta_;

        typedef typename boost::mpl::apply1<meta_, 
        	typename super_t::new_tag_
        >::type type;

		template<typename P>
        static type call(const T& t, P const& p)
        {
            return type( 
            	t.unwrap(), 
                t.fun, 
                p.pop() 
            );
        }

    };

}// result_of_modulo_aux
}// v2
}// assign
}// boost

#define BOOST_ASSIGN_V2_PUT_MODIFIER_XXX_EXT(Param)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace result_of_modulo{\
\
	template<typename T>\
	struct Param \
    	: result_of_modulo_aux::same_fun<T, modulo_tag::Param>\
    {};\
\
}\
namespace put_aux{\
\
	template<typename T>\
    typename boost::lazy_enable_if<\
    	boost::is_base_of<put_aux::put_base, T>,\
    	result_of_modulo::Param<T>\
    >::type\
	operator%(\
    	T const& t,\
        put_aux::Param const& p\
    )\
    {\
    	typedef result_of_modulo::Param<T> caller_;\
        return caller_::call( t, p );\
    }\
\
}\
namespace{\
	put_aux::Param const BOOST_PP_CAT(_,Param) = put_aux::Param();\
}\
}\
}\
}\
/**/

#endif

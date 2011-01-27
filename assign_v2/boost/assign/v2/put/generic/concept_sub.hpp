//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_CONCEPT_SUB_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_CONCEPT_SUB_ER_2010_HPP
#include <boost/concept_check.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_concept{
namespace sub{

	// X models Pre1 wrt to V
	template<typename V, typename X>
	struct Pre1{
    
		BOOST_CONCEPT_USAGE(Pre1)
        {
			X x( v );
            V& ref = x.container();
        }
        
        private:
        static V& v;

	};

	template<typename V, typename F,typename X>
	struct Pre2 : Pre1<V, X>{
    
		BOOST_CONCEPT_USAGE(Pre2)
        {
			X x( v, f );
        }
        
        private:
        static V& v;
        static F const &f;

	};

	template<typename V, typename F, typename Tag,typename X>
	struct Pre3 : Pre2<V, F, X>{
    
        typedef put_aux::modifier<Tag> modifier_;

		BOOST_CONCEPT_USAGE(Pre3)
        {
			X x( v, f, m );
        }
        
        private:
        static V& v;
        static F const &f;
        static modifier_ m;

	};

	template<typename V, typename F, typename Tag, typename X>
	class Post : Pre3<V, F, Tag, X>
    {

		typedef Pre3<V, F, Tag, X> super_t;
        typedef typename super_t::modifier_ modifier_;

		BOOST_CONCEPT_USAGE(Post)
        {
			F const& f = x.fun;
			modifier_ const& m = x.modifier;
        }
        
        private:
        static X const& x;
        static V& v;
        static F const &f;
        static modifier_ m;

    };

}// sub
}// put_concept
}// v2
}// assign
}// boost

#endif

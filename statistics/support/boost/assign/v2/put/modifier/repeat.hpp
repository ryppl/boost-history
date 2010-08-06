//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_REPEAT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_REPEAT_ER_2010_HPP
#include <boost/assign/v2/put/modifier/def.hpp>

namespace boost{
namespace assign{ 
namespace v2{

// lookup_key
namespace put_tag{ template<typename Tag> struct repeat{}; }
namespace put_aux{

	template<typename Tag>
    struct modifier<v2::put_tag::repeat<Tag> >{

        typedef std::size_t size_type;

		modifier( ){}
		explicit modifier( size_type const& n_ ):n( n_ ){};
            
    	template<typename V,typename T>
        void impl(V& v,T& t)const{ 
        	size_type m = n;
        	while(m--) this->inner.impl( v, t );
        }
        
        private:
        size_type n;
        modifier<Tag> inner;
    };

	// Warning : does not check against nested repeat 

}// assign_aux
}// v2
}// assign
}// boost

#endif

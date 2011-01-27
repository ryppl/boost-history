//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_ADAPTER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_ADAPTER_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/frame/crtp.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/container/functor/fwd.hpp> // consistency

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

    template<typename C,typename F, typename Tag> 
    class adapter 
    	: protected ref::wrapper< ref::assign_tag::copy, C > 
        , public put_aux::crtp< C, F, Tag, adapter<C, F, Tag> >
    {
    	typedef put_aux::crtp< C, F, Tag, adapter > super2_t;

		public:

		typedef typename super2_t::result_type result_type;

		protected:

		typedef put_aux::modifier<Tag> modifier_;
		typedef ref::assign_tag::copy assign_tag_;
		typedef ref::wrapper<assign_tag_,C> super1_t;

		public:

		adapter(){}
		explicit adapter( C& v ) : super1_t( v ) {}
		explicit adapter( C& v, F const& f ) 
        	: super1_t( v ), super2_t( f )
            {
        		// This constructor is required by crtp 
                // when Tag or F is modified.
        	}

		explicit adapter( C& v, F const& f, modifier_ const& m ) 
        	: super1_t( v ), super2_t( f, m )
            {
        		// This constructor is required by crtp 
                // when Tag or F is modified.
        	}

		C& container()const{ 
        	return static_cast<super1_t const&>(*this).get(); 
        }

    };

}// put_aux
}// v2
}// assign
}// boost

#endif

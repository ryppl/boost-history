//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_SUB_FUNCTOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_SUB_FUNCTOR_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/generic/crtp.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

    template<typename V,typename F, typename Tag> class sub;

}// put_aux
namespace result_of_modulo{
	
    template<typename V,typename F,typename Tag>
    struct new_fun<put_aux::sub<V,F,Tag> >
    {

    	template<typename F1>
        struct apply{ typedef put_aux::sub<V,F1,Tag> type; };
        
    };

    template<typename V, typename F, typename Tag>
    struct new_modifier<put_aux::sub<V, F, Tag> >
    {
    	template<typename NewTag>
        struct apply{ typedef put_aux::sub<V, F, NewTag> type; };
    };
    
}//result_of_modulo
namespace put_aux{

    template<typename V,typename F, typename Tag> class sub;
    
    template<typename V,typename F, typename Tag> 
    class sub 
    	: protected ref::wrapper< // must be the first base.
        	ref::assign_tag::copy,
            V
        >, 
        public put_aux::crtp<
        	V, 
            F, 
            Tag, 
            sub<V,F,Tag> 
        >
    {

    	typedef put_aux::crtp<
        	V, 
            F, 
            Tag, 
            sub<V,F,Tag>
        > super2_t;

		public:

		typedef typename super2_t::result_type result_type;

		protected:

		typedef put_aux::modifier<Tag> modifier_;
		typedef ref::assign_tag::copy assign_tag_;
		typedef ref::wrapper<assign_tag_,V> super1_t;

		public:

		sub(){}
		explicit sub( V& v ) : super1_t( v ) {}
		explicit sub( V& v, F const& f ) 
        	: super1_t( v ), super2_t( f )
            {
        		// This constructor is required by crtp 
                // when Tag or F is modified.
        	}

		explicit sub( V& v, F const& f, modifier_ const& m ) 
        	: super1_t( v ), super2_t( f, m )
            {
        		// This constructor is required by crtp 
                // when Tag or F is modified.
        	}

		V& get()const{ 
        	return static_cast<super1_t const&>(*this).get(); 
        }

    };

}// put_aux
namespace result_of{

	template<typename V>
    struct put
    {
        typedef put_aux::crtp_traits<V> traits_;
        typedef typename traits_::functor_type f_;
        typedef typename traits_::modifier_tag modifier_tag_;
        typedef put_aux::sub<V,f_,modifier_tag_> type;
    };

}// result_of

	template<typename V>
    typename result_of::put<V>::type
	put( V& v )
    {
    	typedef typename result_of::put<V>::type result_;
        return result_( v );
    }

}// v2
}// assign
}// boost

#endif

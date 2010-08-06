//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_WRAPPER_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_WRAPPER_MAKE_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/wrapper/crtp.hpp>

// Modifiers:
// object % ( _put_tag = Tag() );	Component 1-
// object % ( _fun = F() );			Component 2-
// object % ( _incr_lookup );		++v[ f( xi1, ..., xik(i) ) ];
// object % ( _repeat = n );		

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

    template<typename V,typename F, typename Tag> 
    class put_wrapper;
    
	template<typename V,typename F,typename Tag>
    struct put_wrapper_modifier_traits
    {
		template<typename Tag1>
        struct new_tag{
        	typedef put_wrapper<V,F,Tag1> type;
        };
		template<typename F1>
        struct new_fun{
        	typedef put_wrapper<V,F1,Tag> type;
        };
        
        template<typename F1,typename Tag1>
        struct new_fun_tag{
        	typedef put_wrapper<V,F1,Tag1> type;
        };
    };

    template<typename V,typename F, typename Tag> 
    class put_wrapper 
    	: 
        protected ref::wrapper< // must be the first base.
        	ref::assign_tag::copy,
            V
        >, 
        public put_aux::crtp<
        	V, 
            F, 
            Tag, 
            put_wrapper<V,F,Tag>, 
            put_wrapper_modifier_traits<V,F,Tag> 
        >
    {

    	typedef put_aux::crtp<
        	V, 
            F, 
            Tag, 
            put_wrapper<V,F,Tag>, 
            put_wrapper_modifier_traits<V,F,Tag> 
        > super2_t;

		public:

		typedef typename super2_t::result_type result_type;

		protected:

		typedef put_aux::modifier<Tag> modifier_;
		typedef ref::assign_tag::copy assign_tag_;
		typedef ref::wrapper<assign_tag_,V> super1_t;

		public:

		put_wrapper(){}
		explicit put_wrapper( V& v ) : super1_t( v ) {}
		explicit put_wrapper( V& v, F const& f1 ) 
        	: super1_t( v ), super2_t( f1 ){
        		// This constructor is required by crtp 
                // when Tag or F is modified.
        	}

		V& unwrap()const{ 
        	return static_cast<super1_t const&>(*this).unwrap(); 
        }


    };

}// put_aux
namespace result_of{

	template<typename V>
    struct put
    {
        typedef put_aux::crtp_traits<V> traits_;
        typedef typename traits_::functor_type f_;
        typedef typename traits_::put_tag put_tag_;
        typedef put_aux::put_wrapper<V,f_,put_tag_> type;
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

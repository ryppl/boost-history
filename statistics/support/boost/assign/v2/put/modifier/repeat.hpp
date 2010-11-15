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
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/put/generic/base.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace modifier_tag{ 
	template<typename Tag> struct repeat{}; 
}// modifier_tag
namespace put_aux{

	template<typename Tag>
    class modifier<v2::modifier_tag::repeat<Tag> > : v2::ref::wrapper<
    	v2::ref::assign_tag::copy,
        std::size_t const
    >
    {
		typedef modifier<Tag> inner_;
		typedef v2::ref::wrapper<
    		v2::ref::assign_tag::copy,
        	std::size_t const
    	> super_t;

    	public:

        typedef std::size_t size_type;

		modifier(){}
		explicit modifier( size_type const& n ):super_t( n ){};
            
    	template<typename V,typename T>
        void impl(V& v, T& t)const{ 
        	size_type m = static_cast<super_t const&>(*this).unwrap();
        	while(m--) this->inner.impl( v, t );
        }
        
        private:
        inner_ inner;
    };

	class modulo_repeat : v2::ref::wrapper<
        v2::ref::assign_tag::copy,
        std::size_t const
    >
    {
    	typedef v2::ref::wrapper<
        	v2::ref::assign_tag::copy,
        	std::size_t const
    	> super_t;

		public:
        
        typedef std::size_t size_type;

        modulo_repeat(){}
        modulo_repeat( size_type const& n) : super_t( n ){}

		modulo_repeat operator=( size_type const& n)const
        {
        	modulo_repeat result( n );
            return result;
        }
        
        size_type const& unwrap()const
        { 
        	return static_cast<super_t const&>(*this).unwrap();
        }
        
    };

}// put_aux
namespace result_of_modulo{
    
    template<typename T>
    struct repeat
    {
    	typedef typename T::modifier_tag old_tag_;
        typedef v2::modifier_tag::repeat<old_tag_> new_tag_;
        typedef put_aux::modifier<new_tag_> modifier_;
        typedef typename boost::mpl::apply1<
        	result_of_modulo::new_modifier<T>,
            new_tag_
        >::type type;
        
        static type call(const T& t, put_aux::modulo_repeat const& h)
        {
        	modifier_ m( h.unwrap() );
            return type( t.unwrap(), t.fun, m ); 
        }
        
    };

}// result_of_modulo
namespace put_aux{ 

	template<typename T>
    typename boost::lazy_enable_if<
    	boost::is_base_of<put_aux::put_base, T>,
    	result_of_modulo::repeat<T>
    >::type
	operator%(
    	T const& t, 
        put_aux::modulo_repeat const& h
    )
    {	
    	typedef result_of_modulo::repeat<T> caller_;
        return caller_::call( t, h );
    }

}// put_aux
namespace{
	put_aux::modulo_repeat const _repeat = put_aux::modulo_repeat();
}
}// v2
}// assign
}// boost

#endif

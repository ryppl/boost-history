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
//#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{
	template<typename Tag> struct repeat{};
}// modifier_tag
namespace put_aux{

	template<typename Tag>
    class modifier<v2::modifier_tag::repeat<Tag> > 
    /*: v2::ref::wrapper<
    	v2::ref::assign_tag::copy,
        std::size_t const
    >*/
    {
		typedef modifier<Tag> inner_;
		/*typedef v2::ref::wrapper<
    		v2::ref::assign_tag::copy,
        	std::size_t const
    	> super_t;*/

    	public:

        typedef std::size_t size_type;

		modifier() : n( 1 ){}
		// explicit modifier( size_type const& n ):super_t( n ){};
		explicit modifier( size_type const& n_ ) : n( n ){};

    	template<typename V, typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const
        {

        	size_type m = this->n; //static_cast<super_t const&>(*this).unwrap();
        	while(m--) this->inner.impl(
                v,
                BOOST_ASSIGN_V2_forward_arg(T, t)
            );
        }

		size_type const& size()const{ return this->n; }

        protected:
        inner_ inner;
        size_type n;
    };

	class param_repeat 
    /*: v2::ref::wrapper<
        v2::ref::assign_tag::copy,
        std::size_t const
    >*/
    {

    	/*typedef v2::ref::wrapper<
        	v2::ref::assign_tag::copy,
        	std::size_t const
    	> super_t;*/

		public:

        typedef std::size_t size_type;

        param_repeat(){}
        /*param_repeat( size_type const& n) : super_t( n ){}*/
        param_repeat( size_type const& n_) : n( n_ ){}

		param_repeat operator=( size_type const& n_)const
        {
        	param_repeat result( n_ );
            return result;
        }

		/*
        size_type const& unwrap()const
        {
        	return static_cast<super_t const&>(*this).unwrap();
        }
        */
        
        size_type const& pop()const{ return this->n; }

        protected:
        size_type n;

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

		typedef put_aux::param_repeat param_;
        static type call(const T& t, param_ const& p)
        {
            return type( t.unwrap(), t.fun, p.pop() );
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
        put_aux::param_repeat const& h
    )
    {
    	typedef result_of_modulo::repeat<T> caller_;
        return caller_::call( t, h );
    }

}// put_aux
namespace{
	put_aux::param_repeat const _repeat = put_aux::param_repeat();
}
}// v2
}// assign
}// boost

#endif

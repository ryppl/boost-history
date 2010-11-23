//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_INCR_LOOKUP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_INCR_LOOKUP_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/detail/type_traits/value.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/generic/new_fun.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
#include <boost/assign/v2/put/generic/base.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{ struct incr_lookup{}; }
namespace put_aux{

	template<>
    struct modifier<v2::modifier_tag::incr_lookup>
    {
		typedef std::size_t size_type;
		modifier():n( 0 ){}
		modifier(size_type const& m):n( m ){};

    	template<typename V,typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const{
        	v[ BOOST_ASSIGN_V2_forward_arg(T, t) ] += this->n;
        }

        private:
        size_type n;
    };

    struct param_incr_lookup
    {

		typedef std::size_t size_type;

    	param_incr_lookup() : n( 1 ){}
        param_incr_lookup(std::size_t m) : n( m ){}

        param_incr_lookup
        operator=(size_type const& m)const{
        	typedef param_incr_lookup result_;
            return result_( m );
        }

		size_type const& pop()const{ return this->n; }

		private:

    	size_type n;
    };

}// put_aux
namespace result_of_modulo{

    template<typename T>
    struct incr_lookup
    {
    	typedef result_of_modulo::new_fun_modifier<T> meta_;
        typedef functor_aux::identity fun_;
        typedef v2::modifier_tag::incr_lookup tag_;
    	typedef typename boost::mpl::apply2<
        	meta_,
            fun_,
            tag_
        >::type type;

        typedef put_aux::modifier<tag_> new_modifier_;

		typedef put_aux::param_incr_lookup param_;
        static type call(const T& t, param_ const& p)
        {
			return type( t.unwrap(), _identity, param.pop() );
		}

    };

}// result_of_modulo
namespace put_aux{

	template<typename T>
    typename boost::lazy_enable_if<
    	boost::is_base_of<put_aux::put_base, T>,
    	result_of_modulo::incr_lookup<T>
    >::type
	operator%(
    	T const& t,
        put_aux::param_incr_lookup const& kwd
    )
    {
    	typedef result_of_modulo::incr_lookup<T> caller_;
        return caller_::call( t, kwd );
	}

}// put_aux
namespace{
	put_aux::param_incr_lookup const _incr_lookup
    	= put_aux::param_incr_lookup();
}
}// v2
}// assign
}// boost

#endif

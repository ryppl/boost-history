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
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/detail/type_traits/value.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/put/generic/new_fun.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>

// Warning : Not yet supported.
// TODO fix compile errors using put adaptor.

namespace boost{
namespace assign{ 
namespace v2{

// lookup_key
namespace modifier_tag{ struct incr_lookup{}; }
namespace put_aux{

	template<>
    struct modifier<v2::modifier_tag::incr_lookup>
    {

		modifier():n( 0 ){}
		modifier(std::size_t m):n( m ){};
    
    	template<typename V,typename T>
        void impl(V& v, T& t)const{ 
        	v[ t ] += this->n; 
        }
        
        private:
        std::size_t n;
    };

    struct modulo_incr_lookup{

    	modulo_incr_lookup() : n( 1 ){}
        modulo_incr_lookup(std::size_t m) : n( m ){}

        modulo_incr_lookup 
        operator=(std::size_t m)const{ 
        	typedef modulo_incr_lookup result_;
            return result_( m );
        }

    	std::size_t n;
    };

}// put_aux
namespace result_of_modulo{

    template<typename T>
    struct incr_lookup
    {
    	typedef typename boost::mpl::apply2<
        	result_of_modulo::new_fun_modifier<T>,
            functor_aux::identity,
            v2::modifier_tag::incr_lookup
        >::type type;

        typedef v2::modifier_tag::incr_lookup new_tag_;
        typedef put_aux::modifier<new_tag_> modifier_;
        
        static type call(const T& t, put_aux::modulo_incr_lookup const& kwd)
        {
			return type( t.unwrap(), _identity, kwd.n );
		}

    };

}// result_of_modulo
namespace put_aux{

	template<typename T>
    typename result_of_modulo::incr_lookup<T>::type
	operator%(
    	T const& t,
        put_aux::modulo_incr_lookup const& kwd
    )
    {
    	typedef result_of_modulo::incr_lookup<T> caller_;
        return caller_::call( t, kwd );
	}

}// put_aux
namespace{
	put_aux::modulo_incr_lookup const _incr_lookup 
    	= put_aux::modulo_incr_lookup();
}
}// v2
}// assign
}// boost

#endif

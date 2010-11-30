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
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/generic/parameter.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
#include <boost/assign/v2/put/modifier/ext/xxx.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{ 

	struct incr_lookup{};

}// modifier_tag
namespace put_parameter{

	template<typename OldFun,typename OldTag>
    struct incr_lookup : put_parameter::pair<
    	functor_aux::identity,
        modifier_tag::incr_lookup
    >{};

}// put_parameter
namespace put_aux{

	template<>
    struct modifier<modifier_tag::incr_lookup>
    {
		typedef std::size_t size_type;
		modifier():n( 0 ){}
		modifier(size_type const& m):n( m ){};

    	template<typename V,typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const
        {
        	v[ BOOST_ASSIGN_V2_forward_arg(T, t) ] += this->n;
        }

		// TODO T*, but is there a use for it?

        private:
        size_type n;
    };

    struct incr_lookup
    {

		typedef std::size_t size_type;

    	incr_lookup() : n( 1 ){}
        incr_lookup(size_type const& m) : n( m ){}

        incr_lookup
        operator=(size_type const& m)const{
        	typedef incr_lookup result_;
            return result_( m );
        }

		size_type const& get()const{ return this->n; }

		private:

    	size_type n;
    };

}// put_aux

BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_XXX(incr_lookup)

}// v2
}// assign
}// boost

#endif

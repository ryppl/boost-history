//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_ITERATE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_ITERATE_ER_2010_HPP
#include <cstddef>
#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/put/generic/parameter.hpp>
#include <boost/assign/v2/put/modifier/ext/xxx.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{

    struct iterate{};

}// put_aux
namespace put_parameter{

    template<typename Arg>
    struct iterate{

        template<typename OldFun, typename OldTag>
        struct apply : put_parameter::pair<
            functor_aux::identity,
            modifier_tag::iterate
        >{};
    };

}// put_parameter
namespace put_aux{

	template<>
    class modifier<modifier_tag::iterate>
    {

    	public:

        typedef std::size_t size_type;

        modifier() : i( 0 ){}
        explicit modifier( size_type const& i_ )
           : i( i_ )
        {}

    	template<typename V, typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const
        {
            v.at( i++ ) = BOOST_ASSIGN_V2_forward_arg(T, t);
        }

    	template<typename V, typename T>
        void impl(V& v, T* t)const
        {
            v.replace( i++ , t);
        }

        protected:
        mutable std::size_t i;
    };

    template<typename Arg = boost::mpl::na>
    class iterate
    {

        public:

        typedef std::size_t size_type;
		
        iterate():i( 0 ){}
        iterate( size_type const& i_)
           : i( i_ )
        {}

        size_type const& get()const{ return this->i; }

        protected:
        size_type i;

    };

    class iterate_keyword : public iterate<>
    {

        typedef iterate<> super_t;

		public:

        typedef std::size_t size_type;

        iterate_keyword(){}

        iterate<> operator=( size_type const& n_)const
        {
            typedef iterate<> result_;
            return result_( n_ );
        }

    };

}// put_aux

BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_XXX(iterate)

}// v2
}// assign
}// boost

#endif

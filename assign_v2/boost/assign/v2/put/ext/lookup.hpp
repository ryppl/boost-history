//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_EXT_LOOKUP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_EXT_LOOKUP_ER_2010_HPP
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>

#include <boost/assign/v2/put/ext/xxx.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{ template<typename Arg> struct lookup{}; }
namespace put_parameter{

    template<typename Arg>
    struct lookup{

        template<typename OldFun, typename OldTag>
        struct apply : put_parameter::traits<
            functor_aux::identity,
            modifier_tag::lookup<Arg>
        >{};
    };

}// put_parameter
namespace put_aux{

	template<typename Arg>
    struct modifier<modifier_tag::lookup<Arg> >
    {

        typedef Arg type;
        typedef boost::shared_ptr<type> ptr_;

        modifier(){}
        explicit modifier(
            typename boost::call_traits<type>::param_type arg
        ) : ptr( new type( arg ) )
        {}

    	template<typename V,typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, key) )const{
            v[ key ] = (*this->ptr)( v[ key ] );
        }

        private:
        ptr_ ptr;
    };

    template<typename Arg>
    struct lookup
    {
        typedef Arg const type;

        explicit lookup(type& k) : key( k ){}

        type& arg()const{ return this->key; }

        private:
        type& key;

    };

    struct lookup_keyword
    {
        lookup_keyword(){}

        template<typename Arg1>
        lookup<Arg1> operator=(Arg1 const& arg1)const
        {
            return lookup<Arg1>( arg1 );
        }


    };

}// put_aux

BOOST_ASSIGN_V2_PUT_EXT_XXX(lookup)

}// v2
}// assign
}// boost

#endif

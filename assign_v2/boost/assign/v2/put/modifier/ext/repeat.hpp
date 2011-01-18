//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_REPEAT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_REPEAT_ER_2010_HPP
#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/put/generic/parameter.hpp>
#include <boost/assign/v2/put/modifier/ext/xxx.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{

    template<typename OldTag> struct repeat{};

}// modifier_tag
namespace put_parameter{

    template<typename Arg>
    struct repeat{

        template<typename OldFun, typename OldTag>
        struct apply : put_parameter::pair<
            OldFun,
            modifier_tag::repeat<OldTag>
        >{};
    };

}// put_parameter
namespace put_aux{

	template<typename Tag>
    class modifier<modifier_tag::repeat<Tag> >
    {
        typedef modifier<Tag> inner_;

    	public:

        typedef std::size_t size_type;

        modifier() : n( 0 ){}
        explicit modifier( size_type const& n_ )
            : n( n_ )
        {
        }

#define MACRO(arg)\
        size_type m = this->n;\
        while(m--) this->inner.impl(\
            v,\
            arg\
        );\
/**/
    	template<typename V, typename T>
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const
        {

            MACRO( BOOST_ASSIGN_V2_forward_arg(T, t) )
        }

    	template<typename V, typename T>
        void impl(V& v, T* t)const
        {
            MACRO( t )
        }

#undef MACRO

        size_type const& size()const{ return this->n; }

        protected:
        inner_ inner;
        size_type n;
    };

    template<typename Arg = ::boost::mpl::na>
    class repeat
    {

        public:

        typedef std::size_t size_type;

        repeat( size_type const& n_)
            : n( n_ )
        {}

        size_type const& get()const{ return this->n; }

        protected:
        size_type n;

    };

    class repeat_keyword
    {

        public:

        typedef std::size_t size_type;

        repeat_keyword(){}

        repeat<> operator=( size_type const& n_)const
        {
            typedef repeat<> result_;
            return result_( n_ );
        }

    };

}// put_aux

BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_XXX(repeat)

}// v2
}// assign
}// boost

#endif

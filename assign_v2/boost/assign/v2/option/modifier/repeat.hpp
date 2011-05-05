//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_REPEAT_ER_2011_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_REPEAT_ER_2011_HPP
#include <cstddef>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif
#include <boost/assign/v2/option/modifier/framework.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_repeat
namespace modifier_tag{ 

    template<typename Tag> struct repeat/*<-*/{}/*->*/; 

}// modifier_tag
namespace interpreter_aux{

    template<typename Arg>
    class interpreter_modifier<modifier_tag::repeat<Arg> >/*<-*/
    {
        typedef interpreter_modifier<Arg> inner_type;

        public:

        typedef std::size_t size_type;

        interpreter_modifier() : n_( 0 ){}
        interpreter_modifier( inner_type inner, size_type n )
            : inner_( inner ), n_( n )
        {}

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
#define BOOST_ASSIGN_V2_arg T&& t
#define BOOST_ASSIGN_V2_forward std::forward<T>( t )
//<-
#else
#define BOOST_ASSIGN_V2_arg T& t
#define BOOST_ASSIGN_V2_forward t
#endif
//->
        template<typename C, typename T, typename DTag>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, DTag tag )const
        {
            size_type m = this->size();\
            while(m--) this->inner_.impl( cont, BOOST_ASSIGN_V2_forward, tag );
        }

//<-
#undef BOOST_ASSIGN_V2_arg
#undef BOOST_ASSIGN_V2_forward
//->

        size_type const& size()const{ return this->n_; }

        protected:
        inner_type inner_;
        size_type n_;
    }/*->*/;

}// interpreter_aux
BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(repeat)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(repeat, modifier_tag::repeat<typename D::modifier_tag>)
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_REPEAT_ER_2011_HPP

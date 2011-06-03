//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_ITERATE_ER_2011_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_ITERATE_ER_2011_HPP
#include <cstddef>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/support/functor/iterate.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/option/modifier/framework.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_iterate
namespace modifier_tag{

    template<typename Arg = functor_aux::iterate> struct iterate{};

}// modifier_tag
namespace interpreter_aux{

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

    template<typename Arg>
    class interpreter_modifier< modifier_tag::iterate<Arg> >/*<-*/
    {

        typedef Arg arg_;
        // storing a copy of lambda::something has caused pbs, hence ptr
        typedef boost::shared_ptr<arg_> ptr_;

        public:

        interpreter_modifier()
            : ptr( new arg_() )
        {}
        interpreter_modifier(
            kwd_ignore_,
            typename boost::call_traits<arg_>::param_type arg
        ) : ptr( new arg_( arg ) )
        {}

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::value )const
        {
            cont.at( (*this->ptr)() ) = BOOST_ASSIGN_V2_forward;
        }

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::ptr )const
        {
            typedef typename container_aux::value<C>::type value_;
            cont.replace( (*this->ptr)(), new value_( BOOST_ASSIGN_V2_forward ) );
        }

        protected:
        ptr_ ptr;

    }/*->*/;

//<-
#undef BOOST_ASSIGN_V2_arg
#undef BOOST_ASSIGN_V2_forward
//->

}// interpreter_aux
BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(iterate)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(iterate, modifier_tag::iterate<Arg>)
BOOST_ASSIGN_V2_OPTION_MODIFIER_ALIAS(iterate)
//]
}// v2
}// assign
}// boost


#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_ITERATE_ER_2011_HPP

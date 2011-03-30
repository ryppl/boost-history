//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_MAPPED_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_MAPPED_ER_2010_HPP
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/option/modifier/framework.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_mapped
namespace modifier_tag{ 

    template<
        typename Arg
    > 
    struct mapped; 

}// modifier_tag
namespace interpreter_aux{
                
    template<typename Arg>
    class interpreter_modifier<modifier_tag::mapped<Arg> >/*<-*/
    {

        typedef Arg arg_;
        // storing a copy of lambda::something has caused pbs, hence ptr
        typedef boost::shared_ptr<arg_> ptr_; 
        typedef keyword_aux::ignore ignore_;
        
        public:

        interpreter_modifier() : ptr( new arg_() ){}
        explicit interpreter_modifier(
            ignore_,
            typename boost::call_traits<arg_>::param_type arg
        ) : ptr( new arg_( arg ) )
        {}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#define BOOST_ASSIGN_V2_arg T&& t
#define BOOST_ASSIGN_V2_forward std::forward<T>( t )
#else
#define BOOST_ASSIGN_V2_arg T& t
#define BOOST_ASSIGN_V2_forward t
#endif

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::storage )const
        {
            cont[ t/*key*/ ] = (*this->ptr)( cont[ BOOST_ASSIGN_V2_forward ] );
        }

#undef BOOST_ASSIGN_V2_arg
#undef BOOST_ASSIGN_V2_forward

        private:
        ptr_ ptr;
    }/*->*/;

}// interpreter_aux
BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(mapped)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(mapped, modifier_tag::mapped<Arg>)
//]
}// v2
}// assign
}// boost


#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_MAPPED_ER_2010_HPP

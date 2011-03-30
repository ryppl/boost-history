//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_INSERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_INSERT_ER_2010_HPP
#include <cstddef>
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/option/modifier/framework.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/type_traits/is_reference.hpp>
#include <boost/utility/enable_if.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_insert
namespace modifier_tag{ 

    struct insert{}; 

}// modifier_tag
namespace interpreter_aux{
                
    template<>                
    class interpreter_modifier< modifier_tag::insert >/*<-*/
    {

        typedef keyword_aux::ignore ignore_;
        
        public:
        interpreter_modifier(){}
        interpreter_modifier( ignore_, ignore_ ){}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#define BOOST_ASSIGN_V2_arg T&& t
#define BOOST_ASSIGN_V2_forward std::forward<T>( t )
#else
#define BOOST_ASSIGN_V2_arg T& t
#define BOOST_ASSIGN_V2_forward t
#endif

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::value )const
        {
            cont.insert( BOOST_ASSIGN_V2_forward );
        }
        
        template<typename C, typename T>
        void impl(C& cont, T& t, data_tag::value_map )const
        {
            cont.insert( BOOST_ASSIGN_V2_forward );
        }

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::ptr )const
        {
            typedef typename container_aux::value<C>::type value_;
            cont.insert( new value_( BOOST_ASSIGN_V2_forward ) );
        }

        template<typename C, typename T>
        void impl( C& cont, BOOST_ASSIGN_V2_arg, data_tag::ptr_map )const
        {
            typedef typename container_aux::key<C>::type key_;
			// non-const key necessary. See ptr_map.
            key_ k = BOOST_ASSIGN_V2_forward.first; 
            typedef typename container_aux::mapped<C>::type m_;
            cont.insert( k , new m_( BOOST_ASSIGN_V2_forward.second ) );
        }

    };

#undef BOOST_ASSIGN_V2_arg
#undef BOOST_ASSIGN_V2_forward

}// interpreter_aux
BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(insert)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(insert, modifier_tag::insert)
//]
}// v2
}// assign
}// boost


#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_ITERATE_ER_2010_HPP

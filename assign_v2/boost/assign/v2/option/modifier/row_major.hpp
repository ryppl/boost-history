//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_MODIFIER_ROW_MAJOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_MODIFIER_ROW_MAJOR_ER_2010_HPP
#include <cstddef>
#include <boost/assert.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/functor/iterate.hpp>
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/option/modifier/framework.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

    

namespace boost{
namespace assign{
namespace v2{
//[syntax_option_row_major
namespace modifier_tag{ 

    template<typename Arg = functor_aux::iterate> struct row_major{}; 

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

    template<std::size_t Dim, std::size_t NumDims, bool exit = Dim == NumDims>
    struct row_major
    {
        typedef std::size_t size_type;
        typedef row_major<Dim + 1, NumDims> nested_;
        
        template<typename Array, typename T>
        static void assign(Array& array, size_type remainder, BOOST_ASSIGN_V2_arg )
        {
            size_type n = array.num_elements() / array.size();
            size_type j = remainder / n;
            typename Array::reference sub = array[ j ];
            nested_::assign( sub, remainder - ( j * n ), BOOST_ASSIGN_V2_forward );
        }
    };

    template<std::size_t Dim, std::size_t NumDims>
    struct row_major<Dim, NumDims, true>
    {
        typedef std::size_t size_type;
        
        template<typename E, typename T>
        static void assign(E& elem, size_type index, BOOST_ASSIGN_V2_arg)
        {
            elem = BOOST_ASSIGN_V2_forward;
        }
    };

    template<typename Array, typename T>
    void row_major_assign(Array& array, std::size_t index,  BOOST_ASSIGN_V2_arg)
    {
        typedef row_major<0, Array::dimensionality> row_major_;
        row_major_::assign( array, index, BOOST_ASSIGN_V2_forward );
    }

//->

    template<typename Arg>
    class interpreter_modifier< modifier_tag::row_major<Arg> >/*<-*/
    {

        typedef Arg arg_;
        typedef boost::shared_ptr<arg_> ptr_; 

        public:
        interpreter_modifier()
            : ptr( new arg_() )
        {}
        interpreter_modifier(
            ignore_,  
            typename boost::call_traits<arg_>::param_type arg 
        ) 
            : ptr( new arg_( arg ) )
        {}

        template<typename C, typename T>
        void impl(C& cont, BOOST_ASSIGN_V2_arg, data_tag::value )const
        {
            row_major_assign( cont, (*this->ptr)(), BOOST_ASSIGN_V2_forward );
        }

        private:
        mutable ptr_ ptr;
    }/*->*/;

//<-
#undef BOOST_ASSIGN_V2_arg
#undef BOOST_ASSIGN_V2_forward
//->
    
}// interpreter_aux
//]
BOOST_ASSIGN_V2_OPTION_MODIFIER_KEYWORD(row_major)
BOOST_ASSIGN_V2_OPTION_MODIFIER_META_MODIFIER_TAG(row_major, modifier_tag::row_major<Arg>)
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_OPTION_MODIFIER_ROW_MAJOR_ER_2010_HPP

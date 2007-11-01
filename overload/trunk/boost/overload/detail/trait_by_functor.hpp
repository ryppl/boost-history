/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_TRAIT_BY_FUNCTOR_HPP_
#define _BOOST_OVERLOAD_DETAIL_TRAIT_BY_FUNCTOR_HPP_

#include "final_overload_base.hpp"
#include "functor_has_signature.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// it provides the base that inherits directly by boost::function<Sig> where Sig
// is the first signature defined in Overload that matches the functor signature,
// if it's not found returns final_overload_base as type

template< 
    typename Functor, 
    typename Overload,
    bool FOUND
        = functor_has_signature<Functor, typename Overload::base_type::signature_type >::value
>
struct base_by_functor
{
  private:
    typedef typename Overload::base_type base_type;
  public:
    typedef typename base_by_functor<Functor, base_type>::type type;
};

template< typename Functor, typename Overload >
struct base_by_functor<Functor, Overload, true>
{
    typedef typename Overload::base_type type;
};

template< typename Functor, bool FOUND >
struct base_by_functor<Functor, final_overload_base, FOUND>
{
    typedef final_overload_base type;
};

///////////////////////////////////////////////////////////////////////////////
// it provides the first signature of an overload type that matches with
// the functor signature, if it's not found it returns no_signature as type

template< typename Functor, typename Overload>
struct signature_by_functor
{
    typedef typename base_by_functor<Functor, Overload>::type::signature_type 
            type;
};

///////////////////////////////////////////////////////////////////////////////
// it provides the type of the boost::function embedded object whose signature 
// matches with the functor signature, if it's not found it returns
// boost::function<no_signature> as type;

template< typename Functor, typename Overload >
struct function_by_functor
{
    typedef 
        typename base_by_functor<Functor, Overload>::type::function_type 
        type;
};

///////////////////////////////////////////////////////////////////////////////
// it provides the index of the first signature of an overload type that 
// matches with the functor signature, if it's not found it returns -1

template< typename Functor, typename Overload >
struct index_by_functor
{
    static const unsigned int value 
        =  base_by_functor<Functor, Overload>::type::index;
};

///////////////////////////////////////////////////////////////////////////////
// utility to discover if a given functor has a signature included in 
// an overload type

template< typename Functor, typename Overload >
struct has_functor_signature
{
    static const bool value 
        = !( index_by_functor<Functor, Overload>::value 
             == final_overload_base::index );
};

} } } // end namespaces

#endif // _BOOST_OVERLOAD_DETAIL_TRAIT_BY_FUNCTOR_HPP_


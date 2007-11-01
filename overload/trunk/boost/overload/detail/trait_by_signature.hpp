/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_TRAIT_BY_SIGNATURE_HPP_
#define _BOOST_OVERLOAD_DETAIL_TRAIT_BY_SIGNATURE_HPP_

#include <boost/type_traits/is_same.hpp>

#include "final_overload_base.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// it provides the base that inherits directly by boost::function<Sig>
// if it's not found returns final_overload_base as type

template< 
    typename Sig, 
    typename Overload,
    bool FOUND
        = boost::is_same<Sig, typename Overload::base_type::signature_type >::value
>
struct base_by_sig
{
  private:
    typedef typename Overload::base_type base_type;
  public:
    typedef typename base_by_sig<Sig, base_type>::type type;
};

template< typename Sig, typename Overload >
struct base_by_sig<Sig, Overload, true>
{
    typedef typename Overload::base_type type;
};

template< typename Sig, bool FOUND >
struct base_by_sig<Sig, final_overload_base, FOUND>
{
    typedef final_overload_base type;
};

///////////////////////////////////////////////////////////////////////////////
// it provedes the index of a signature, and returns -1 if it's not found

template< typename Sig, typename Overload >
struct index
{
    static const unsigned int value =  base_by_sig<Sig, Overload>::type::index;
};

///////////////////////////////////////////////////////////////////////////////
// utility to discover if a given signature is included in an overload

template< typename Sig, typename Overload >
struct has_signature
{
    static const bool value 
        = !( index<Sig, Overload>::value == final_overload_base::index );
};

} } } // end namespaces


#endif // _BOOST_OVERLOAD_DETAIL_TRAIT_BY_SIGNATURE_HPP_


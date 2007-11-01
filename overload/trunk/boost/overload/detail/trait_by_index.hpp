/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_TRAIT_BY_INDEX_HPP_
#define _BOOST_OVERLOAD_DETAIL_TRAIT_BY_INDEX_HPP_

#include "final_overload_base.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// it provides the amount of signatures supported by the overload type

template< typename Overload >
struct extent
{
  private:
    typedef typename Overload::base_type base;
  public:
    static const int value = extent<base>::value + 1;
};

template<>
struct extent<final_overload_base>  
{
    static const int value = final_overload_base::index;
};

///////////////////////////////////////////////////////////////////////////////
// it provides the N-th base for an overload type


template< unsigned int N, typename Overload, 
          bool FOUND = (N == Overload::base_type::index) >
struct base_by_idx
{
    typedef typename base_by_idx<N, typename Overload::base_type>::type type;
};

template< unsigned int N, typename Overload >
struct base_by_idx<N, Overload, true>
{
    typedef typename Overload::base_type type;
};

///////////////////////////////////////////////////////////////////////////////
// it provides the N-th signature for an overload type

template< unsigned int N, typename Overload>
struct signature
{
    typedef typename base_by_idx<N, Overload>::type::signature_type type;
};


///////////////////////////////////////////////////////////////////////////////
// it provides the type of the N-th boost::function embedded object

template< unsigned int N, typename Overload >
struct function
{
    typedef typename base_by_idx<N, Overload>::type::function_type type;
};

} } } // end namespaces


#endif // _BOOST_OVERLOAD_DETAIL_TRAIT_BY_INDEX_HPP_


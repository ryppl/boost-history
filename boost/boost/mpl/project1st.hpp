
#ifndef BOOST_MPL_PROJECT1ST_HPP_INCLUDED
#define BOOST_MPL_PROJECT1ST_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2002
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$


#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost {
namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(T1)
    , typename BOOST_MPL_AUX_NA_PARAM(T2)
    >
struct project1st
{
    typedef T1 type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,project1st,(T1,T2))
};

BOOST_MPL_AUX_NA_SPEC(2, project1st)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PROJECT1ST_HPP_INCLUDED

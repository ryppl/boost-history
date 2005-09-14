#ifndef BOOST_METAMATH_TANGENT_HPP
#define BOOST_METAMATH_TANGENT_HPP

#include <boost/mpl/tag.hpp>
#include <boost/mpl/apply.hpp>
#include <math.h>

namespace boost {namespace metamath {

template< typename Tag > struct tangent_impl;

template< typename T > struct tangent_tag
{
    typedef typename mpl::tag<T>::type type;
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(N)
    >
struct tangent 
    : mpl::apply1<
        tangent_impl<typename tangent_tag<N>::type>,
        N
    >::type
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1, tangent, (N))
};

}} //namespace boost::metamath

#include <boost/metamath/double/tangent.hpp>

#endif


#ifndef BOOST_MPL_FIND_IF_HPP_INCLUDED
#define BOOST_MPL_FIND_IF_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2002
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/find_if_pred.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/iter_fold_if.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/aux_/common_name_wknd.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost {
namespace mpl {

BOOST_MPL_AUX_COMMON_NAME_WKND(find_if)

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename BOOST_MPL_AUX_NA_PARAM(Predicate)
    >
struct find_if
{
private:

    typedef typename lambda<Predicate>::type pred_;

    typedef typename iter_fold_if<
          Sequence
        , void
        , protect< arg<1> > // ignore
        , protect< aux::find_if_pred<pred_> >
        >::type result_;

public:

    typedef typename result_::second type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,find_if,(Sequence,Predicate))

};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

BOOST_MPL_AUX_NA_ALGORITHM_SPEC(2,find_if)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_FIND_IF_HPP_INCLUDED

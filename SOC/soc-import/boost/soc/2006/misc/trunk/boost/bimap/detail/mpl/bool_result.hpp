// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file detail/mpl/bool_result.hpp
/// \brief Macro utility to construct type_traits

#ifndef BOOST_BIMAP_DETAIL_MPL_BOOL_RESULT_HPP
#define BOOST_BIMAP_DETAIL_MPL_BOOL_RESULT_HPP

#include <boost/mpl/bool.hpp>

/*//////////////////////////////////////////////////////////////////*/
#define BOOST_BIMAP_MPL_BOOL_RESULT(VALUE)                           \
    typedef mpl::bool_<VALUE> type;                                  \
    static const bool value = VALUE;
/*//////////////////////////////////////////////////////////////////*/

#endif // BOOST_BIMAP_DETAIL_MPL_BOOL_RESULT_HPP


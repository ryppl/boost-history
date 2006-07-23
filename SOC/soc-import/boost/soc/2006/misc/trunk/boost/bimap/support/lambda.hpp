// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file support/lambda.hpp
/// \brief Placeholders definition to help in bimap modify function

#ifndef BOOST_BIMAP_SUPPORT_LAMBDA_HPP
#define BOOST_BIMAP_SUPPORT_LAMBDA_HPP

#include <boost/lambda/lambda.hpp>

namespace boost {
namespace bimap {
namespace support {

boost::lambda::placeholder1_type _first ;
boost::lambda::placeholder2_type _second;

boost::lambda::placeholder1_type _left  ;
boost::lambda::placeholder2_type _right ;

boost::lambda::placeholder1_type _key   ;

} // namespace support
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_SUPPORT_LAMBDA_HPP


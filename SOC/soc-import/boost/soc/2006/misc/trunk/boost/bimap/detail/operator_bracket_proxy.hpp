// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file detail/get_value_type.hpp
/// \brief Metafunction to obtain the value_type typedef of a type.

#ifndef BOOST_BIMAP_DETAIL_GET_VALUE_TYPE_HPP
#define BOOST_BIMAP_DETAIL_GET_VALUE_TYPE_HPP

namespace boost {
namespace bimap {
namespace detail {

template< class Type >
struct get_value_type
{
    typedef typename Type::value_type type;
};

} // namespace detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_GET_VALUE_TYPE_HPP

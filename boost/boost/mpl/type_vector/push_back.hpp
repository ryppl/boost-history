//-----------------------------------------------------------------------------
// boost mpl/type_vector/push_back.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_TYPE_VECTOR_PUSH_BACK_HPP
#define BOOST_MPL_TYPE_VECTOR_PUSH_BACK_HPP

#include "boost/mpl/type_vector/insert.hpp"
#include "boost/mpl/push_back.hpp"

namespace boost {
namespace mpl {

template<>
struct push_back_algorithm_traits<mpl::type_vector_sequence_tag>
{
    template<typename Vector, typename T>
    struct algorithm
        : mpl::detail::type_vector_insert_algorithm<
                Vector::size
              , Vector
              , T
              >
    {
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_TYPE_VECTOR_PUSH_BACK_HPP

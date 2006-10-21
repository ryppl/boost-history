// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/detail/value_comparison_adaptor.hpp
/// \brief Value comparison adaptor.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_VALUE_COMPARISON_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_VALUE_COMPARISON_ADAPTOR_HPP

#include <boost/call_traits.hpp>
#include <functional>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Value comparison adaptor
/**

A simple adaptor.
                                                                                    **/

// TODO
// * The value comparison adaptor can be optimized for NewValue equal to ValueCompare::argument_type
// * Use ConceptCheck to ensure the validity of ValueCompare

template < class ValueCompare, class NewValue >
struct value_comparison_adaptor : std::binary_function<NewValue,NewValue,bool>
{
    value_comparison_adaptor( ValueCompare c ) : comp(c) {}

    bool operator()( typename call_traits<NewValue>::param_type x,
                     typename call_traits<NewValue>::param_type y)
    {
        return comp(
            typename ValueCompare::first_argument_type(x),
            typename ValueCompare::first_argument_type(y)
        );
    }

    private:
    ValueCompare comp;
};

} // namespace detail
} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_VALUE_COMPARISON_ADAPTOR_HPP



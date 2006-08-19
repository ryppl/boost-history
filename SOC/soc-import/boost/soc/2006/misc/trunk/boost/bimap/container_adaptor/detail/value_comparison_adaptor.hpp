// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
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



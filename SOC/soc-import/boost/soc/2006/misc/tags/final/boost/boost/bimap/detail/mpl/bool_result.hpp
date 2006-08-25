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


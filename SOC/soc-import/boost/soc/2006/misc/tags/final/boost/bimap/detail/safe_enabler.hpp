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

/// \file detail/safe_enabler.hpp
/// \brief Utility class to manage the set types of a bimap.

#ifndef BOOST_BIMAP_DETAIL_SAFE_ENABLER_HPP
#define BOOST_BIMAP_DETAIL_SAFE_ENABLER_HPP

namespace boost {
namespace bimap {
namespace detail {

/// \brief
/**

This is the class used when working wiht enable_if.
It must be an incomplete type if we wnat the mechanism to work.
It can be used void or void* but is safer this way because there
is no possibility of conversion to this hidden incomplete type.

\note For some reason, I could not make this works. I do not know
what I am doing wrong. Please let me know if you find what is it.
Until then all the enable_if are used with void.

                                                                    **/
class safe_enabler;

} // namesapce detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_SAFE_ENABLER_HPP

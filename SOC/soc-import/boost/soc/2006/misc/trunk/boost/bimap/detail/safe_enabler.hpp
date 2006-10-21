// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
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

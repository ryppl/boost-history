/**
 * \file boost/mirror/utils/name_to_stream/class.hpp
 * Putting names of reflected classes to streams
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_UTILS_NAME_TO_STREAM_CLASS_HPP
#define BOOST_MIRROR_UTILS_NAME_TO_STREAM_CLASS_HPP

// helpers for type name 
#include <boost/mirror/utils/name_to_stream/type.hpp>
// meta classes
#include <boost/mirror/meta_class.hpp>

namespace boost {
namespace mirror {


/** Specialization for meta-classes
 */
template <typename base_type> 
struct name_to_stream_helper<meta_class<base_type> > :
public name_to_stream_helper<meta_type<base_type> > { };


} // namespace mirror
} // namespace boost

#endif //include guard


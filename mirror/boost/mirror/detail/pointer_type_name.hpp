/**
 * \file boost/mirror/detail/pointer_type_name.hpp
 * Helpers for composing a pointer type name
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_POINTER_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_POINTER_TYPE_NAME_HPP

#include <assert.h>
#include <boost/mirror/detail/ptr_ref_type_name.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <class meta_type>
struct static_pointer_type_name : static_ptr_ref_type_name<
	meta_type, BOOST_STR_LIT('*')
>{ };


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


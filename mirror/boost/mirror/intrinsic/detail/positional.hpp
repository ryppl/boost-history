/**
 * \file boost/mirror/intrinsic/detail/positional.hpp
 *
 * Common boilerplate for iterator getters.
 * 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_DETAIL_POSITIONAL_HPP
#define BOOST_MIRROR_INTRINSIC_DETAIL_POSITIONAL_HPP

#include <boost/mirror/meta_attributes.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default position_getter_impl_1
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		class Arg1, 
		template <class, class> class Implementation
	>
	struct position_getter_impl_1 : Implementation<
		MetaObjectSequence,
		Arg1
	>{ };

	/** Declaration of the default position_getter_impl
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		template <class, class> class Implementation
	>
	struct position_getter_impl
	: position_getter_impl_1<MetaObjectSequence, void, Implementation>
	{ };

	/** Declaration of the default explicit_position_getter_impl
	 *  helper template.
	 */
	template <
		class MetaObjectSequence, 
		class Position,
		template <class, class> class Implementation
	>
	struct explicit_position_getter_impl
	: position_getter_impl_1<MetaObjectSequence, Position, Implementation>
	{ };



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


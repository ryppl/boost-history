/**
 * \file boost/mirror/visitors/fallback.hpp
 * Base implementation of a meta-object visitor
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_VISITORS_FALLBACK
#define BOOST_MIRROR_VISITORS_FALLBACK

// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// meta types 
#include <boost/mirror/meta_type.hpp>

namespace boost {
namespace mirror {

struct fallback_visitor 
{
	// visitation of the 
	template <typename base_type, typename meta_type>
	void visit_instance(const meta_type& meta_data, base_type& instance, const bchar* name){ }
};

} // namespace mirror
} // namespace boost

#endif //include guard


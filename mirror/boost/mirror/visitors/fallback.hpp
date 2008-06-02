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
	template <class MetaClass>
	inline void enter_type(MetaClass){ }

	template <class MetaClass>
	inline void leave_type(MetaClass){ }

	template <class MetaClass>
	inline void enter_attributes(void){ }
	template <class MetaClass>
	inline void leave_attributes(void){ }

	template <class MetaClass>
	inline void enter_base_classes(void){ }
	template <class MetaClass>
	inline void leave_base_classes(void){ }

	template <class MetaInheritance>
	inline void enter_base_class(MetaInheritance){ }

	template <class MetaInheritance>
	inline void leave_base_class(MetaInheritance){ }

	template <class MetaAttribute>
	inline void enter_attribute(MetaAttribute){ }

	template <class MetaAttribute>
	inline void leave_attribute(MetaAttribute){ }
};

} // namespace mirror
} // namespace boost

#endif //include guard


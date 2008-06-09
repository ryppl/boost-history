/**
 * \file boost/mirror/visitors/dtd_writer.hpp
 * meta-class/type visitor that writes DTD for the given class
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_VISITORS_DTD_WRITER_HPP
#define BOOST_MIRROR_VISITORS_DTD_WRITER_HPP

// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// meta types 
#include <boost/mirror/meta_class.hpp>
// traits
#include <boost/mirror/traits/reflects_virtual_inheritance.hpp>
#include <boost/mirror/traits/reflects_global_scope.hpp>
//
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace mirror {

namespace visitors {

class dtd_writer
{
public:
	typedef mpl::bool_<false> works_on_instances;

	dtd_writer(void){ }

	// enter a class/type
	template <class MetaClass>
	void enter_type(MetaClass)
	{
	}

	// leave the class/type
	template <class MetaClass>
	void leave_type(MetaClass)
	{
	}

	template <class MetaClass>
	inline void enter_base_classes(MetaClass){ }
	template <class MetaClass>
	inline void leave_base_classes(MetaClass){ }


	// enter a base class
	template <class MetaInheritance>
	void enter_base_class(MetaInheritance)
	{
		using namespace ::std;
		using namespace ::boost;
	}

	// leave base class
	template <class MetaInheritance>
	void leave_base_class(MetaInheritance)
	{
		using namespace ::std;
		using namespace ::boost;
	}


	template <class MetaClass, class MetaAttributes>
	void enter_attributes(MetaClass, MetaAttributes)
	{
	}

	template <class MetaClass, class MetaAttributes>
	void leave_attributes(MetaClass, MetaAttributes)
	{
	}

	template <class MetaAttribute>
	inline void enter_attribute(MetaAttribute)
	{
		using namespace ::std;
		using namespace ::boost;
	}

	template <class MetaAttribute>
	void leave_attribute(MetaAttribute)
	{
		using namespace ::std;
		using namespace ::boost;
	}

private:
	std::map<bstring, bstring> lines;
};

} // namespace visitors
} // namespace mirror
} // namespace boost

#endif //include guard


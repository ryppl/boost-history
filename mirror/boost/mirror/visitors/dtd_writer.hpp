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
//
#include <vector>
#include <map>


namespace boost {
namespace mirror {

namespace visitors {

class dtd_writer
{
public:
	//! this writer works on types
	typedef mpl::bool_<false> works_on_instances;

	//! constructs a new dtd writer
	dtd_writer(const bstring& _docRootName)
	: documentRootInfo(_docRootName)
	{ }

	// enter a class/type
	template <class MetaClass, class Context>
	void enter_type(MetaClass, Context)
	{
	}

	// leave the class/type
	template <class MetaClass, class Context>
	void leave_type(MetaClass, Context)
	{
	}

	template <class MetaClass, class Context>
	inline void enter_base_classes(MetaClass, Context){ }
	template <class MetaClass, class Context>
	inline void leave_base_classes(MetaClass, Context){ }


	// enter a base class
	template <class MetaInheritance, class Context>
	void enter_base_class(MetaInheritance, Context)
	{
		using namespace ::std;
		using namespace ::boost;
	}

	// leave base class
	template <class MetaInheritance, class Context>
	void leave_base_class(MetaInheritance, Context)
	{
		using namespace ::std;
		using namespace ::boost;
	}


	template <class MetaClass, class MetaAttributes, class Context>
	void enter_attributes(MetaClass, MetaAttributes, Context)
	{
	}

	template <class MetaClass, class MetaAttributes, class Context>
	void leave_attributes(MetaClass, MetaAttributes, Context)
	{
	}

	template <class MetaAttribute, class Context>
	inline void enter_attribute(MetaAttribute, Context)
	{
		using namespace ::std;
		using namespace ::boost;
	}

	template <class MetaAttribute, class Context>
	void leave_attribute(MetaAttribute, Context)
	{
		using namespace ::std;
		using namespace ::boost;
	}

private:
	// information about a single element
	struct element_info
	{
		// the name of the element
		bstring name;
		// the type of the element
		bstring type_name;
		//
		// does not need to occur
		bool card_zero;
		// can appear once
		bool card_one;
		// can appear multiple times
		bool card_multi;

		element_info(
			const bstring& _name,
			const bstring& _type_name = bstring(),
			bool _card_zero = false,
			bool _card_one = true,
			bool _card_multi = false
		)
		: name(_name)
		, type_name(_type_name)
		, card_zero(_card_zero)
		, card_one(_card_one)
		, card_multi(_card_multi)
		{ }
	};
	
	// the information about the root element
	element_info documentRootInfo;

	// the type for definition of an elements child nodes
	typedef ::std::vector<element_info> element_nodes;
	//
	// the type that is used to store information about various
	// types of elements
	typedef ::std::map<bstring, element_nodes> type_info_table;
	//
	type_info_table type_infos;
};

} // namespace visitors
} // namespace mirror
} // namespace boost

#endif //include guard


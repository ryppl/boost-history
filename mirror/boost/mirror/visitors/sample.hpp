/**
 * \file boost/mirror/visitors/sample.hpp
 * Sample implementation of a meta-object visitor
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_VISITORS_SAMPLE
#define BOOST_MIRROR_VISITORS_SAMPLE

// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// meta types 
#include <boost/mirror/meta_class.hpp>
// traits
#include <boost/mirror/traits/reflects_virtual_inheritance.hpp>
#include <boost/mirror/traits/reflects_global_scope.hpp>
// the base fallback visitor implementation
#include <boost/mirror/visitors/fallback.hpp>

namespace boost {
namespace mirror {

class sample_visitor : public fallback_visitor 
{
public:
	sample_visitor(void):indent(0){ }

	// enter a class/type
	template <class meta_class>
	void enter_type(meta_class)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<type name='" << 
			meta_class::base_name();
		if(!reflects_global_scope<typename meta_class::scope>::value)
		{
			bcout << 
				"' scope='" <<
				meta_class::scope::full_name();
		}
		bcout << 
			"'>" << 
			endl;
	}

	// leave the class/type
	template <class meta_class>
	void leave_type(meta_class)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</type>" << endl;
	}

	// enter a base class
	template <class meta_inheritance>
	void enter_base_class(meta_inheritance)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<base_class number='" << 
			meta_inheritance::position::value <<
			"' is_virtual='"<<
			(reflects_virtual_inheritance<meta_inheritance>::value ? "true" : "false") << 
			"'>" << 
			endl;
	}

	// leave base class
	template <class meta_class>
	void leave_base_class(meta_class)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</base_class>" << endl;
	}

	template <class meta_attribute>
	void enter_attribute(meta_attribute)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<attribute number='" << 
			meta_attribute::position::value <<
			"' name='" <<
			meta_attribute::base_name() <<
			"'>" << 
			endl;
	}

	template <class meta_attribute>
	void leave_attribute(meta_attribute)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</attribute>" << endl;
	}
private:
	int indent;
	void print_indentation(void)
	{
		for(int i=0;i!=indent;++i)
			bcout << ' ';
	}
};


} // namespace mirror
} // namespace boost

#endif //include guard


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
//
//
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace mirror {

class sample_visitor 
{
public:
	sample_visitor(void):indent(0){ }

	// enter a class/type
	template <class MetaClass>
	void enter_type(MetaClass)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<type name='" << 
			MetaClass::base_name();
		if(!reflects_global_scope<typename MetaClass::scope>::value)
		{
			bcout << 
				"' scope='" <<
				MetaClass::scope::full_name();
		}
		bcout << 
			"'>" << 
			endl;
	}

	// leave the class/type
	template <class MetaClass>
	void leave_type(MetaClass)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</type>" << endl;
	}

	template <class MetaClass>
	inline void enter_base_classes(void){ }
	template <class MetaClass>
	inline void leave_base_classes(void){ }


	// enter a base class
	template <class MetaInheritance>
	void enter_base_class(MetaInheritance)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<base_class number='" << 
			MetaInheritance::position::value <<
			"' is_virtual='"<<
			(reflects_virtual_inheritance<MetaInheritance>::value ? "true" : "false") << 
			"'>" << 
			endl;
	}

	// leave base class
	template <class MetaInheritance>
	void leave_base_class(MetaInheritance)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</base_class>" << endl;
	}


	template <class MetaClass, class MetaAttributes>
	void enter_attributes(void)
	{
		using namespace ::std;
		using namespace ::boost;
		if(!is_fundamental<MetaClass::reflected_type>::value && (mirror::size<MetaAttributes>::value > 0))
		{
			print_indentation();
			++indent;
			bcout << 
				"<attributes>" << 
			endl;
		}
	}

	template <class MetaClass, class MetaAttributes>
	inline void leave_attributes(void)
	{
		using namespace ::std;
		using namespace ::boost;
		if(!is_fundamental<MetaClass::reflected_type>::value && (mirror::size<MetaAttributes>::value > 0))
		{
			--indent;
			print_indentation();
			bcout << "</attributes>" << endl;
		}
	}

	template <class MetaAttribute>
	inline void enter_attribute(MetaAttribute)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		++indent;
		bcout << 
			"<attribute number='" << 
			MetaAttribute::position::value <<
			"' name='" <<
			MetaAttribute::base_name() <<
			"'>" << 
			endl;
	}

	template <class MetaAttribute>
	void leave_attribute(MetaAttribute)
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


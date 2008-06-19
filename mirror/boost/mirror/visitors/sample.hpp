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
// container size
#include <boost/mirror/algorithm/size.hpp>
//
//
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace mirror {

template <bool WorksOnInstances>
class sample_visitor 
{
public:
	typedef mpl::bool_<WorksOnInstances> works_on_instances;

	sample_visitor(void):indent(0){ }

	// enter a class/type
	template <class MetaClass, class Context>
	void enter_type(MetaClass, Context)
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
	template <class MetaClass, class Context>
	void leave_type(MetaClass, Context)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</type>" << endl;
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
	template <class MetaInheritance, class Context>
	void leave_base_class(MetaInheritance, Context)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</base_class>" << endl;
	}


	template <class MetaClass, class MetaAttributes, class Context>
	void enter_attributes(MetaClass, MetaAttributes, Context)
	{
		using namespace ::std;
		using namespace ::boost;
		if(
			!is_fundamental<typename MetaClass::reflected_type>::value && 
			(mirror::size<MetaAttributes>::value > 0)
		)
		{
			print_indentation();
			++indent;
			bcout << 
				"<attributes>" << 
			endl;
		}
	}

	template <class MetaClass, class MetaAttributes, class Context>
	void leave_attributes(MetaClass, MetaAttributes, Context)
	{
		using namespace ::std;
		using namespace ::boost;
		if(
			!is_fundamental<typename MetaClass::reflected_type>::value && 
			(mirror::size<MetaAttributes>::value > 0)
		)
		{
			--indent;
			print_indentation();
			bcout << "</attributes>" << endl;
		}
	}

	template <class MetaAttribute, class Context>
	inline void enter_attribute(MetaAttribute, Context)
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
			"' static='" <<
			(MetaAttribute::traits::is_static::value?"true":"false") <<
			"'>" << 
			endl;
bcout << 
	"---" << meta_path::size<meta_path::self<Context> >::value << "---" << 
	endl << 
	"---" << meta_path::size<meta_path::ancestors<Context> >::value << "---" << 
	endl;
	}

	template <class MetaAttribute, class Context>
	void leave_attribute(MetaAttribute, Context)
	{
		using namespace ::std;
		using namespace ::boost;
		--indent;
		print_indentation();
		bcout << "</attribute>" << endl;
	}

	template <class MetaClass, class Context, typename InstanceType>
	void visit_instance(MetaClass, Context ctx, InstanceType* ptr_to_inst)
	{
		print_value(
			ctx,
			ptr_to_inst, 
			is_fundamental<typename MetaClass::reflected_type>()
		);
	}

private:

	template <typename Type, class Context>
	void print_value(Context, Type* ptr_to_inst, mpl::bool_<false>){ }

	template <typename Type, class Context>
	void print_value(Context, Type* ptr_to_inst, mpl::bool_<true>)
	{
		using namespace ::std;
		using namespace ::boost;
		print_indentation();
		bcout << 
			"<value depth='" <<
			mpl::size<Context>::value << 
			"'>" <<
			*ptr_to_inst << 
			"</value>" << 
			endl;
	}

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


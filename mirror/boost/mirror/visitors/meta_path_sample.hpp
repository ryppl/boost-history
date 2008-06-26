/**
 * \file boost/mirror/visitors/meta_path_sample.hpp
 * Sample implementation of a meta-object visitor
 * showing how traversal contexts and meta-paths work
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_VISITORS_META_PATH_SAMPLE
#define BOOST_MIRROR_VISITORS_META_PATH_SAMPLE

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
// meta-path related stuff
#include <boost/mirror/meta_path/export_paths_and_nodes.hpp>
#include <boost/mirror/meta_path/for_each.hpp>
#include <boost/mirror/meta_path/self.hpp>
#include <boost/mirror/meta_path/parent.hpp>
#include <boost/mirror/meta_path/ancestors.hpp>
#include <boost/mirror/meta_path/ancestors_and_self.hpp>
#include <boost/mirror/meta_path/siblings.hpp>
#include <boost/mirror/meta_path/siblings_and_self.hpp>
#include <boost/mirror/meta_path/size.hpp>

//
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace mirror {

namespace detail {

	struct node_printer
	{
		bool simple;

		node_printer(void):simple(true){ }
		node_printer(const node_printer&):simple(false){ }

		void print_indent(void) const
		{
			if(!simple) bcout << ::std::endl << "\t\t";
		}

		template <
			class ReflectedType, 
			class VariantTag,
			class MetaAttributes,
			class AttribPos
		>
		void operator()(meta_class_attribute<
			ReflectedType,
			VariantTag,
			MetaAttributes,
			AttribPos
		> mca) const
		{
			bcout << "|attribute '" << mca.base_name() << "'|";
			print_indent();
		}
	
		template <
			class ReflectedType, 
			class VariantTag
		>
		void operator()(meta_class_attributes<
			ReflectedType,
			VariantTag
		> mca) const
		{
			bcout << "|attributes|";
			print_indent();
		}
	
		template <
			class ReflectedType, 
			class VariantTag
		>
		void operator()(meta_class_all_attributes<
			ReflectedType,
			VariantTag
		> mca) const
		{
			bcout << "|all_attributes|";
			print_indent();
		}
	
		template <
			class ReflectedType, 
			class VariantTag
		>
		void operator()(meta_base_classes<
			ReflectedType,
			VariantTag
		> mca) const
		{
			bcout << "|base_classes|";
			print_indent();
		}
	
		template <
			class Position,
			class BaseClass,
			typename AccessSpec, 
			typename InheritanceSpec
		>
		void operator()(meta_inheritance<
			Position,
			BaseClass,
			AccessSpec, 
			InheritanceSpec
		> mi) const
		{
			typedef typename meta_inheritance<
				Position,
				BaseClass,
				AccessSpec, 
				InheritanceSpec
			>::meta_base_class mbc;
			bcout << "|base_class '" << mbc::base_name() << "'|";
			print_indent();
		}
	
		template <class Class>
		void operator()(meta_class<Class> mc) const
		{
			bcout << "|type '" << mc.base_name() << "'|";
			print_indent();
		}
	};
	
} // namespace detail 

class meta_path_sample_visitor 
{
public:
	typedef mpl::bool_<false> works_on_instances;

	// enter a class/type
	template <class MetaClass, class Context>
	void enter_type(MetaClass, Context)
	{
		print_node(MetaClass(), Context());
	}

	// leave the class/type
	template <class MetaClass, class Context>
	void leave_type(MetaClass, Context) const { }

	template <class MetaClass, class BaseClasses, class Context>
	inline void enter_base_classes(MetaClass, BaseClasses, Context)
	{
		if(!mpl::empty<BaseClasses>::value)
			print_node(BaseClasses(), Context());
	}
	
	template <class MetaClass, class BaseClasses, class Context>
	inline void leave_base_classes(MetaClass, BaseClasses, Context){ }


	// enter a base class
	template <class MetaInheritance, class Context>
	void enter_base_class(MetaInheritance, Context)
	{
		print_node(MetaInheritance(), Context());
	}

	// leave base class
	template <class MetaInheritance, class Context>
	void leave_base_class(MetaInheritance, Context){ }


	template <class MetaClass, class MetaAttributes, class Context>
	void enter_attributes(MetaClass, MetaAttributes, Context)
	{
		if(!mpl::empty<MetaAttributes>::value)
			print_node(MetaAttributes(), Context());
	}

	template <class MetaClass, class MetaAttributes, class Context>
	void leave_attributes(MetaClass, MetaAttributes, Context){ }

	template <class MetaAttribute, class Context>
	inline void enter_attribute(MetaAttribute, Context)
	{
		print_node(MetaAttribute(), Context());
	}

	template <class MetaAttribute, class Context>
	void leave_attribute(MetaAttribute, Context){ }

	template <class MetaClass, class Context, typename InstanceType>
	void visit_instance(MetaClass, Context ctx, InstanceType* ptr_to_inst)
	{
	}

private:

	template <template <class> class Axis, class MetaObject, class Context>
	void print_node_set(const char* name, MetaObject mo, Context ctx)
	{
		using ::std::endl;
		//
		bcout << "\t" << name << ": "; 
		meta_path::for_each<
			Axis<Context>, 
			mpl::lambda<mpl::second<mpl::_1> > 
		>(detail::node_printer());
		bcout << endl;
	}

	template <class MetaObject, class Context>
	void print_node(MetaObject mo, Context ctx)
	{
		using ::std::endl;
		//
		detail::node_printer p;
		bcout << "node "; 
		p(mo);
		bcout << ":" << endl;
		//
		print_node_set<meta_path::self>("self", mo, ctx);
		print_node_set<meta_path::parent>("parent", mo, ctx);
		print_node_set<meta_path::ancestors>("ancestors", mo, ctx);
		print_node_set<meta_path::ancestors_and_self>("ancestors_and_self", mo, ctx);
		print_node_set<meta_path::siblings>("siblings", mo, ctx);
		print_node_set<meta_path::siblings_and_self>("siblings_and_self", mo, ctx);
	}

};


} // namespace mirror
} // namespace boost

#endif //include guard


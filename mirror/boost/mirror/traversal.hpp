/**
 * \file boost/mirror/traversal.hpp
 * Traversal of meta classes by a visitor
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_TRAVERSAL_HPP
#define BOOST_MIRROR_TRAVERSAL_HPP

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/for_each.hpp>
//
#include <boost/ref.hpp>

namespace boost {
namespace mirror {


template <class MetaClass> class deep_traversal_of;
template <class MetaClass> class flat_traversal_of;

namespace detail {

	template <class MetaClass, class MetaAttributes>
	struct traversal_utils
	{
	protected:
		template <class VisitorType>
		class attribute_traversal
		{
		public:
			attribute_traversal(reference_wrapper<VisitorType> _visitor)
			: visitor(_visitor)
			{
				visitor.enter_attributes<MetaClass, MetaAttributes>();
			}
	
			~attribute_traversal(void)
			{
				visitor.leave_attributes<MetaClass, MetaAttributes>();
			}
	
			template <class MetaAttribute>
			void operator ()(MetaAttribute ma)
			{
				visitor.enter_attribute(ma);
				typedef MetaAttribute meta_attribute;
				typedef typename meta_attribute::type attrib_type;
				deep_traversal_of<
					BOOST_MIRROR_REFLECT_CLASS(attrib_type)
				>::accept(visitor);
				visitor.leave_attribute(ma);
			}
		private:
			VisitorType& visitor;
		};
	
		template <class VisitorType>
		static inline attribute_traversal<VisitorType>
		show_attribs_to(reference_wrapper<VisitorType> visitor)
		{
			return attribute_traversal<VisitorType>(visitor);
		}
	
		template <class VisitorType>
		class base_class_traversal
		{
		public:
			base_class_traversal(reference_wrapper<VisitorType> _visitor)
			: visitor(_visitor)
			{
				visitor.enter_base_classes<MetaClass>();
			}
	
			~base_class_traversal(void)
			{
				visitor.leave_base_classes<MetaClass>();
			}
	
			template <class MetaInheritance>
			void operator ()(MetaInheritance mbc)
			{
				visitor.enter_base_class(mbc);
				typedef MetaInheritance meta_inheritance;
				typedef typename meta_inheritance::meta_base_class
					meta_base_class;
				deep_traversal_of<meta_base_class>::accept(visitor);
				visitor.leave_base_class(mbc);
			}
		private:
			VisitorType& visitor;
		};

		template <class VisitorType>
		static inline base_class_traversal<VisitorType>
		show_bases_to(reference_wrapper<VisitorType> visitor)
		{
			return base_class_traversal<VisitorType>(visitor);
		}
	};


} // namespace detail

template <class MetaClass>
class deep_traversal_of 
: detail::traversal_utils<MetaClass, typename MetaClass::attributes>
{
public:
	template <class VisitorType>
	static void accept(VisitorType visitor)
	{
		do_accept(ref<VisitorType>(visitor));
	}
	template <class VisitorType>
	static void accept(reference_wrapper<VisitorType> visitor)
	{
		do_accept(visitor);
	}
private:
	template <class VisitorType>
	static void do_accept(reference_wrapper<VisitorType> visitor)
	{
		typedef MetaClass meta_class;
		meta_class mc;
		visitor.get().enter_type(mc);
		for_each<typename meta_class::base_classes>(ref(show_bases_to(visitor)));
		for_each<typename meta_class::attributes>(ref(show_attribs_to(visitor)));
		visitor.get().leave_type(mc);
	}
};

template <class MetaClass>
class flat_traversal_of
: detail::traversal_utils<MetaClass, typename MetaClass::all_attributes>
{
public:
	template <class VisitorType>
	static void accept(VisitorType visitor)
	{
		do_accept(ref<VisitorType>(visitor));
	}
	template <class VisitorType>
	static void accept(reference_wrapper<VisitorType> visitor)
	{
		do_accept(visitor);
	}
private:
	template <class VisitorType>
	static void do_accept(reference_wrapper<VisitorType> visitor)
	{
		typedef MetaClass meta_class;
		meta_class mc;
		visitor.get().enter_type(mc);
		for_each<typename meta_class::all_attributes>(ref(show_attribs_to(visitor)));
		visitor.get().leave_type(mc);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard


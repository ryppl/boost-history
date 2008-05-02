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

namespace boost {
namespace mirror {


template <class MetaClass> struct deep_traversal_of;
template <class MetaClass> struct flat_traversal_of;

namespace detail {

	template <class MetaClass>
	struct traversal_utils
	{
	protected:
		template <class VisitorType>
		class attribute_traversal
		{
		public:
			attribute_traversal(VisitorType _visitor)
			: visitor(_visitor){ }
	
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
			VisitorType visitor;
		};
	
		template <class VisitorType>
		static inline attribute_traversal<VisitorType>
		show_attribs_to(VisitorType visitor)
		{
			return attribute_traversal<VisitorType>(visitor);
		}
	
		template <class VisitorType>
		class base_class_traversal
		{
		public:
			base_class_traversal(VisitorType _visitor)
			: visitor(_visitor){ }
	
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
			VisitorType visitor;
		};

		template <class VisitorType>
		static inline base_class_traversal<VisitorType>
		show_bases_to(VisitorType visitor)
		{
			return base_class_traversal<VisitorType>(visitor);
		}
	};


} // namespace detail

template <class MetaClass>
struct deep_traversal_of : detail::traversal_utils<MetaClass>
{
	template <class VisitorType>
	static void accept(VisitorType visitor)
	{
		typedef MetaClass meta_class;
		meta_class mc;
		visitor.enter_type(mc);
		for_each<typename meta_class::base_classes>(show_bases_to(visitor));
		for_each<typename meta_class::attributes>(show_attribs_to(visitor));
		visitor.leave_type(mc);
	}
};

template <class MetaClass>
struct flat_traversal_of : detail::traversal_utils<MetaClass>
{
	template <class VisitorType>
	static void accept(VisitorType visitor)
	{
		typedef MetaClass meta_class;
		meta_class mc;
		visitor.enter_type(mc);
		for_each<typename meta_class::all_attributes>(show_attribs_to(visitor));
		visitor.leave_type(mc);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard


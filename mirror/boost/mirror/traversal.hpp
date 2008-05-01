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
namespace detail {

	template <class meta_class>
	struct traversal_utils
	{
	protected:
		template <class visitor_type>
		class attribute_traversal
		{
		public:
			attribute_traversal(visitor_type _visitor)
			: visitor(_visitor){ }
	
			template <class meta_attribute>
			void operator ()(meta_attribute ma)
			{
				visitor.enter_attribute(ma);
				deep_traversal_of<
					BOOST_MIRROR_REFLECT_CLASS(meta_attribute::type)
				>::accept(visitor);
				visitor.leave_attribute(ma);
			}
		private:
			visitor_type visitor;
		};
	
		template <class visitor_type>
		static inline attribute_traversal<visitor_type>
		show_attribs_to(visitor_type visitor)
		{
			return attribute_traversal<visitor_type>(visitor);
		}
	
		template <class visitor_type>
		class base_class_traversal
		{
		public:
			base_class_traversal(visitor_type _visitor)
			: visitor(_visitor){ }
	
			template <class meta_inheritance>
			void operator ()(meta_inheritance mbc)
			{
				visitor.enter_base_class(mbc);
				deep_traversal_of<
					meta_inheritance::meta_base_class
				>::accept(visitor);
				visitor.leave_base_class(mbc);
			}
		private:
			visitor_type visitor;
		};

		template <class visitor_type>
		static inline base_class_traversal<visitor_type>
		show_bases_to(visitor_type visitor)
		{
			return base_class_traversal<visitor_type>(visitor);
		}
	};


} // namespace detail

template <class meta_class>
struct deep_traversal_of : detail::traversal_utils<meta_class>
{
	template <class visitor_type>
	static void accept(visitor_type visitor)
	{
		meta_class mc;
		visitor.enter_type(mc);
		for_each<meta_class::base_classes>(show_bases_to(visitor));
		for_each<meta_class::attributes>(show_attribs_to(visitor));
		visitor.leave_type(mc);
	}
};

template <class meta_class>
struct flat_traversal_of : detail::traversal_utils<meta_class>
{
	template <class visitor_type>
	static void accept(visitor_type visitor)
	{
		meta_class mc;
		visitor.enter_type(mc);
		for_each<meta_class::all_attributes>(show_attribs_to(visitor));
		visitor.leave_type(mc);
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard


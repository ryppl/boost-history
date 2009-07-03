// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_LIST_HPP
#define BOOST_CLONEABLE_LIST_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/container_base.hpp>
#include <boost/cloneable/base.hpp>
#include <boost/cloneable/instance.hpp>
#include <boost/cloneable/detail/prefix.hpp>

namespace boost 
{
	namespace cloneable
	{
		/// a list of heterogenous objects
		// TODO: derive from sequence<ptr_list>
		// TODO: move to boost/heterogenous/list
		template <class Base, class Alloc>
		struct list
			: detail::sequence_container_base<
				ptr_list<
					abstract_base<Base>
					, allocator
					, typename detail::make_clone_allocator<Alloc>::type >
				, Base
				, Alloc>
		{
			typedef detail::sequence_container_base<
					ptr_list<abstract_base<Base>
						, allocator
						, typename detail::make_clone_allocator<Alloc>::type >
					, Base
					, Alloc> 
				parent_type;
			typedef typename parent_type::base_type base_type;
			typedef typename parent_type::abstract_base_type abstract_base_type;
			typedef typename parent_type::allocator_type allocator_type;
			using parent_type::validate;
			using parent_type::new_instance;
			typedef typename parent_type::value_type value_type;
			typedef typename parent_type::reference reference;
			typedef typename parent_type::const_reference const_reference;
			typedef typename parent_type::iterator iterator;
			typedef typename parent_type::const_iterator const_iterator;

		public:
			list()
			{
			}

			list(allocator_type &a) 
				: parent_type(a)
			{
			}
		};
	
	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_LIST_HPP

//EOF


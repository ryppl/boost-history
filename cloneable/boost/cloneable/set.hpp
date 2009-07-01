// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_SET_HPP
#define BOOST_CLONEABLE_SET_HPP

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/make_clone_allocator.hpp>
#include <boost/cloneable/allocator.hpp>

namespace boost 
{
	namespace cloneable
	{
		/// a vector of heterogenous objects
		// TODO: move to boost/heterogenous/set.hpp, or boost/cloneable/containers/set.hpp
		template <class Base, class Pred, class Alloc>
		struct set
		{
			typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;
			typedef Base base_type;
			typedef Pred predicate_type;
			typedef abstract_base<Base> abstract_base_type;

			typedef ptr_set<abstract_base_type, predicate_type, allocator, allocator_type> implementation;

			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference;
			typedef typename implementation::const_reference const_reference;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;
			typedef set<Base, Pred, Alloc> this_type;

		private:
			implementation impl;

		public:
			set()
			{
			}
			set(allocator_type a) 
				: impl(a)
			{
			}

			/* purposefully elided
			template <class II>
			set(II F, II L, allocator_type a = allocator_type());
			*/


		public:
			typedef std::pair<iterator, bool> emplace_result;
			template <class U>
			emplace_result emplace_insert()
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator()).to_abstract();
				return impl.insert(instance);
			}

			// TODO: use variadic arguments or BOOST_PP to pass ctor args
			template <class U, class A0>
			emplace_result emplace_insert(A0 a0)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0).to_abstract();
				return impl.insert(instance);
			}
			template <class U, class A0, class A1>
			emplace_result emplace_insert(A0 a0, A1 a1)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0, a1).to_abstract();
				return impl.insert(instance);
			}
			template <class U, class A0, class A1, class A2>
			emplace_result emplace_insert(A0 a0, A1 a1, A2 a2)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0, a1, a2).to_abstract();
				return impl.insert(instance);
			}

			template <class Fun>
			Fun for_each(Fun fun)
			{
				BOOST_FOREACH(value_type &value, *this)
				{
					fun(value);
				}
			}

			size_t size() const
			{
				return impl.size();
			}
			bool empty() const
			{
				return impl.empty();
			}

			iterator begin()
			{
				return impl.begin();
			}
			iterator end()
			{
				return impl.end();
			}
			const_iterator begin() const
			{
				return impl.begin();
			}
			const_iterator end() const
			{
				return impl.end();
			}

			template <class U, class A0>
			iterator find(A0 a0) // strong
			{
				abstract_base_type *instance = 0;
				instance = detail::construct<U,base_type>(get_allocator(), a0).to_abstract();
				BOOST_CLONEABLE_SCOPE_EXIT((instance))
				{
					cloneable::release(instance, self->get_allocator());
				}
				BOOST_CLONEABLE_SCOPE_EXIT_END
				return impl.find(*instance);
			}

			typename implementation::allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		};
	
	} // namespace heterogenous

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_SET_HPP

//EOF

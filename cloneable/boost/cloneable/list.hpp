// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_LIST_HPP
#define BOOST_CLONEABLE_LIST_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/make_clone_allocator.hpp>
#include <boost/cloneable/base.hpp>
#include <boost/cloneable/traits.hpp>
#include <boost/cloneable/detail/allocation.hpp>

namespace boost 
{
	namespace cloneable
	{
		/// a list of heterogenous objects
		// TODO: move to boost/heterogenous/list
		template <class Base, class Alloc>
		struct list
		{
			typedef Base base_type;
			typedef abstract_base<Base> abstract_base_type;
			typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;
			typedef ptr_list<abstract_base_type, allocator, allocator_type> implementation;
			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference;
			typedef typename implementation::const_reference const_reference;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;

			// this provides better information from errors than otherwise
			template <class T>
			struct validate
			{
				// can only add cloneable things to a heterogenous container
				BOOST_STATIC_ASSERT(is_cloneable<T>::value);

				typedef is_same<typename traits<T>::abstract_base_type, abstract_base_type> same_type;

				// you must ensure that base-types for objects that you add to a container
				// is the same base used by the container
				BOOST_STATIC_ASSERT(same_type::value);

				typedef typename traits<T>::derived_type type;
			};

		private:
			implementation impl;

		public:
			list()
			{
			}

			list(allocator_type &a) 
				: impl(a)
			{
			}

			//purposefully elided
			//template <class II>
			//list(II F, II L, allocator_type a = allocator_type());
			//list(size_t reserved);

			template <class Ty, class Fun>
			Fun for_each(Fun fun)
			{
				BOOST_FOREACH(base_type &b, *this)
				{
					if (Ty *ptr = dynamic_cast<Ty *>(&b))
					{
						fun(*ptr);
					}
				}
				return fun;
			}

			template <class Ty, class Fun>
			Fun for_each(Fun fun) const
			{
				BOOST_FOREACH(const base_type &base, *this)
				{
					if (Ty *ptr = dynamic_cast<Ty *>(&base))
					{
						fun(*ptr);
					}
				}
				return fun;
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

			value_type &back()
			{
				return impl.back();
			}
			const value_type &back() const
			{
				return impl.back();
			}
			value_type &front()
			{
				return impl.front();
			}
			const value_type &front() const
			{
				return impl.front();
			}

			template <class Other>
			Other &ref_back()
			{
				BOOST_STATIC_ASSERT(is_cloneable<Other>::value);
				Other *ptr = dynamic_cast<Other *>(back());
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}
			template <class Other>
			Other &ref_front()
			{
				Other *ptr = dynamic_cast<Other *>(front());
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}

			// TODO: use variadic arguments or BOOST_PP to pass ctor args
			template <class U>
			void emplace_back()
			{
				typedef typename validate<U>::type value;
				impl.push_back(detail::construct<value,base_type>(get_allocator()).to_abstract());
			}
			template <class U, class A0>
			void emplace_back(A0 a0)
			{
				typedef typename validate<U>::type value;
				impl.push_back(detail::construct<value,base_type>(get_allocator(), a0).to_abstract());
			}
			template <class U, class A0, class A1>
			void emplace_back(A0 a0, A1 a1)
			{
				typedef validate<U>::type value;
				impl.push_back(detail::construct<value,base_type>(get_allocator(), a0,a1).to_abstract());
			}
			template <class U, class A0, class A1, class A2>
			void emplace_back(A0 a0, A1 a1, A2 a2)
			{
				typedef typename validate<U>::type value;
				impl.push_back(detail::construct<value,base_type>(get_allocator(), a0,a1,a2).to_abstract());
			}

			template <class U>
			void emplace_front()
			{
				typedef typename validate<U>::type value;
				impl.push_front(detail::construct<value,base_type>(get_allocator()).to_abstract());
			}
			template <class U, class A0>
			void emplace_front(A0 a0)
			{
				typedef validate<U> valid;
				impl.push_front(detail::construct<U,base_type>(get_allocator(), a0).to_abstract());
			}
			template <class U, class A0, class A1>
			void emplace_front(A0 a0, A1 a1)
			{
				typedef validate<U> valid;
				impl.push_front(detail::construct<U,base_type>(get_allocator(), a0,a1).to_abstract());
			}
			template <class U, class A0, class A1, class A2>
			void emplace_front(A0 a0, A1 a1, A2 a2)
			{
				typedef validate<U> valid;
				impl.push_front(detail::construct<U,base_type>(get_allocator(), a0,a1,a2).to_abstract());
			}

			typename implementation::allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		};
	
	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_LIST_HPP

//EOF


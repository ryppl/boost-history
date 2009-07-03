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
			: detail::container_base<Base, Alloc>
		{
			typedef detail::container_base<Base,Alloc> parent_type;
			typedef typename parent_type::base_type base_type;
			typedef typename parent_type::abstract_base_type abstract_base_type;
			typedef typename parent_type::allocator_type allocator_type;
			using parent_type::validate;
			using parent_type::new_instance;

			typedef ptr_list<abstract_base_type, allocator, allocator_type> implementation;

			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference;
			typedef typename implementation::const_reference const_reference;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;

		private:
			implementation impl;

		public:
			list()
				: impl(get_allocator())
			{
			}

			list(allocator_type &a) 
				: parent_type(a), impl(get_allocator())
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
			Other &back_as()
			{
				BOOST_STATIC_ASSERT(is_cloneable<Other>::value);
				Other *ptr = dynamic_cast<Other *>(back());
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}
			template <class Other>
			Other &front_as()
			{
				BOOST_STATIC_ASSERT(is_cloneable<Other>::value);
				Other *ptr = dynamic_cast<Other *>(front());
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}

			// TODO: use variadic arguments or BOOST_PP to pass ctor args
			template <class U>
			void push_back()
			{
				impl.push_back(new_instance<typename validate<U>::type>().to_abstract());
			}
			template <class U, class A0>
			void push_back(A0 a0)
			{
				impl.push_back(new_instance<typename validate<U>::type>(a0).to_abstract());
			}
			template <class U, class A0, class A1>
			void push_back(A0 a0, A1 a1)
			{
				impl.push_back(new_instance<typename validate<U>::type>(a0,a1).to_abstract());
			}
			template <class U, class A0, class A1, class A2>
			void push_back(A0 a0, A1 a1, A2 a2)
			{
				impl.push_back(new_instance<typename validate<U>::type>(a0,a1,a2).to_abstract());
			}


			template <class U>
			void push_front()
			{
				impl.push_front(new_instance<typename validate<U>::type>().to_abstract());
			}
			template <class U, class A0>
			void push_front(A0 a0)
			{
				impl.push_front(new_instance<typename validate<U>::type>(a0).to_abstract());
			}
			template <class U, class A0, class A1>
			void push_front(A0 a0, A1 a1)
			{
				impl.push_front(new_instance<typename validate<U>::type>(a0,a1).to_abstract());
			}
			template <class U, class A0, class A1, class A2>
			void push_front(A0 a0, A1 a1, A2 a2)
			{
				impl.push_front(new_instance<typename validate<U>::type>(a0,a1,a2).to_abstract());
			}
		};
	
		template <class Base, class Alloc>
		bool operator==(const list<Base,Alloc> &left, const list<Base,Alloc> &right)
		{
			return left.size() == right.size() 
				&& std::equal(left.begin(), left.end(), right.begin(), std::equal_to<abstract_base<Base> >());
		}
	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_LIST_HPP

//EOF


// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_VECTOR_HPP
#define BOOST_HETEROGENOUS_VECTOR_HPP

#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/base.hpp>
#include <boost/heterogenous/make_cloneable_allocator.hpp>

namespace boost 
{
	namespace heterogenous
	{
		/// a vector of heterogenous objects
		template <class Alloc, class Base = common_base >
		struct vector
		{
			typedef typename make_clone_allocator<Alloc>::type allocator_type;
			typedef ptr_vector<Base, allocator, allocator_type> implementation;
			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference_type;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;

		private:
			implementation impl;

		public:

			size_t size() const
			{
				return impl.size();
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
			reference_type at(size_t n)
			{
				return impl.at(n);
			}
			reference_type operator[](size_t n)
			{
				return impl[n];
			}
			template <class Other>
			bool is_type_at(size_t n) const
			{
				return ptr_at<Other>(n) != 0;
			}
			template <class Other>
			Other &ref_at(size_t n)
			{
				Other *ptr = ptr_at<Other>(n);
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}
			template <class Other>
			Other *ptr_at(size_t n)
			{
				return dynamic_cast<Other *>(&at(n));
			}

			template <class U>
			void push_back()
			{
				U *ptr = construct_type<U>();
				base().push_back(ptr);
			}
			template <class U, class A0>
			void push_back(A0 a0)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0);
				impl.push_back(ptr);
			}
			template <class U, class A0, class A1>
			void push_back(A0 a0, A1 a1)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0, a1);
				impl.push_back(ptr);
			}
			template <class U, class A0, class A1, class A2>
			void push_back(A0 a0, A1 a1, A2 a2)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0, a1, a2);
				impl.push_back(ptr);
			}

			allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		private:
			template <class U>
			U *allocate_type()
			{
				typename allocator_type::template rebind<U>::other alloc(get_allocator());
				return alloc.allocate(1);
			}

			template <class U>
			U *construct_type()
			{
				typename allocator_type::template rebind<U>::other alloc(get_allocator());
				U *ptr = alloc.allocate(1);
				alloc.construct(ptr);
				return ptr;
			}

		};
	
	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_VECTOR_HPP

//EOF

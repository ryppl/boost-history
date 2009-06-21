// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_VECTOR_H
#define BOOST_MONOTONIC_VECTOR_H

#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/container.hpp>

namespace boost
{
	namespace monotonic
	{
		/// a vector that uses a monotonic allocator by default
		template <class T>
		struct vector : detail::monotonic_container<vector<T> >
		{
			typedef detail::monotonic_container<std::vector<T, allocator<T> > > Parent;
			typedef detail::Create<detail::is_monotonic<T>::value, T> Create;
			typedef allocator<T> Allocator;
			typedef std::vector<T,Allocator> Vector;
			typedef typename Vector::iterator iterator;
			typedef typename Vector::const_iterator const_iterator;
			typedef typename Vector::size_type size_type;
			typedef typename Vector::value_type value_type;
			typedef typename Vector::reference reference;
			typedef typename Vector::const_reference const_reference;


		private:
			Vector impl;

		public:
			vector() { }
			vector(Allocator const &A) 
				: impl(A) { }
			vector(size_t N, T const &X, Allocator const &A)
				: impl(N,X,A) { }
			template <class II>
			vector(II F, II L, Allocator const &A)
				: impl(F,L,A) { }

			Allocator get_allocator() const
			{
				return impl.get_allocator();
			}
			bool empty() const
			{
				return impl.empty();
			}
			size_type size() const
			{
				return impl.size();
			}
			void resize(size_type size)
			{
				impl.resize(size);//, Creator::Create(GetStorage()));
			}
			void reserve(size_type size)
			{
				impl.reserve(size);
			}
			size_type capacity() const
			{
				return impl.capacity();
			}
			reference at(size_type index)
			{
				return impl.at(index);
			}
			const_reference at(size_type index) const
			{
				return impl.at(index);
			}
			reference operator[](size_type index)
			{
				return impl[index];
			}
			const_reference operator[](size_type index) const
			{
				return impl[index];
			}
			void push_back(value_type const &value)
			{
				impl.push_back(value);
			}
			void pop_back()
			{
				impl.pop_back();
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
			value_type const &front() const
			{
				return impl.front();
			}
			value_type &front()
			{
				return impl.front();
			}
			value_type const &back() const
			{
				return impl.back();
			}
			value_type &back()
			{
				return impl.back();
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_VECTOR_H

//EOF

// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_DEQUE_HPP
#define BOOST_MONOTONIC_DEQUE_HPP

#include <deque>
#include <boost/monotonic/container.hpp>

namespace boost
{
	namespace monotonic
	{
		/// a vector that uses a monotonic allocator in the given region, with given access system
		template <class T, class Region = default_region_tag, class Access = default_access_tag>
		struct deque : detail::container<deque<T,Region,Access> >
		{
			typedef allocator<T,Region,Access> Allocator;
			typedef detail::container<std::vector<T, Allocator > > Parent;
			typedef detail::Create<detail::is_monotonic<T>::value, T> Create;
			typedef std::deque<T,Allocator> Impl;

			typedef BOOST_DEDUCED_TYPENAME Impl::iterator iterator;
			typedef BOOST_DEDUCED_TYPENAME Impl::const_iterator const_iterator;
			typedef BOOST_DEDUCED_TYPENAME Impl::size_type size_type;
			typedef BOOST_DEDUCED_TYPENAME Impl::value_type value_type;
			typedef BOOST_DEDUCED_TYPENAME Impl::reference reference;
			typedef BOOST_DEDUCED_TYPENAME Impl::const_reference const_reference;

		private:
			Impl impl;

		public:
			deque() { }
			deque(Allocator A) 
				: impl(A) { }
			deque(size_t N, T const &X, Allocator A = Allocator())
				: impl(N,X,A) { }
			template <class II>
			deque(II F, II L, Allocator A = Allocator())
				: impl(F,L,A) { }

			Allocator get_allocator() const
			{
				return impl.get_allocator();
			}
			Impl const &get_impl() const
			{
				return impl;
			}
			Impl &get_impl()
			{
				return impl;
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
			void push_front(value_type const &value)
			{
				impl.push_front(value);
			}
			void pop_front()
			{
				impl.pop_front();
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

			void swap(deque &other)
			{
				impl.swap(other.impl);
			}
		};

		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator==(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return A.size() == B.size() && std::equal(A.begin(), A.end(), B.begin());
		}
		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator!=(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return !(A == B);
		}
		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator<(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return std::lexicographical_compare(A.begin(), A.end(), B.begin(), B.end());
		}
		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator>(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return std::lexicographical_compare(B.begin(), B.end(), A.begin(), A.end());
		}
		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator<=(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return !(A > B);
		}
		template <class Ty,class R,class Acc,class Ty2,class R2,class Acc2>
		bool operator>=(deque<Ty,R,Acc> const &A, deque<Ty2,R2,Acc2> const &B)
		{
			return !(A < B);
		}

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_DEQUE_HPP

//EOF

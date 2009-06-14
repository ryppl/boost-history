// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace boost
{
	namespace iter_range_detail
	{
		template <class Iter>
		struct iter_range : std::pair<Iter, Iter>
		{
			typedef std::pair<Iter, Iter> Parent;
			
			iter_range() { }
			iter_range(Iter const &A, Iter const &B) 
				: Parent(A,B) { }

			size_t size() const
			{
				return std::distance(first, second);
			}
			bool empty() const
			{
				return first == second;
			}
			void advance(size_t N)
			{
				std::advance(first, N);
			}
			iter_range& operator++()
			{
				BOOST_ASSERT(*this);
				++first;
				return *this;
			}
			iter_range operator++(int)
			{
				BOOST_ASSERT(*this);
				iter_range tmp(*this);
				++*this;
				return tmp;
			}
			iter_range& operator--()
			{
				BOOST_ASSERT(*this);
				--first;
				return *this;
			}
			iter_range operator--(int)
			{
				BOOST_ASSERT(*this);
				iter_range tmp(*this);
				--*this;
				return tmp;
			}
			operator bool() const
			{
				return first != second;
			}
		};
	}

	template <class C>
	struct iter_range : iter_range_detail::iter_range<typename C::iterator>
	{
		typedef C Container;
		typedef typename Container::iterator iterator;
		typedef iter_range_detail::iter_range<iterator> Parent;
		typedef typename boost::iterator_value<iterator>::type Value;

		iter_range() { }
		iter_range(Container &C) 
			: Parent(C.begin(), C.end()) { }
		iter_range(iterator A) 
			: Parent(A,A) { }
		iter_range(iterator A, iterator B) 
			: Parent(A,B) { }
		typename Value &operator*() const
		{
			return *first;
		}
	};

	template <class C>
	struct const_iter_range : iter_range_detail::iter_range<typename C::const_iterator>
	{
		typedef C Container;
		typedef typename Container::const_iterator const_iterator;
		typedef iter_range_detail::iter_range<const_iterator> Parent;
		typedef typename boost::iterator_value<const_iterator>::type Value;

		const_iter_range() { }
		template <class C2>
		const_iter_range(iter_range<C2> const &R)
			: Parent(R.first, R.second) { }
		const_iter_range(Container const &C) 
			: Parent(C.begin(), C.end()) { }
		const_iter_range(const_iterator A) 
			: Parent(A,A) { }
		const_iter_range(const_iterator A, const_iterator B) 
			: Parent(A,B) { }
		typename const Value &operator*() const
		{
			return *first;
		}
	};
	template <class C>
	const_iter_range<C> make_const_range(C &X)
	{
		return const_iter_range<C>(X);
	}
	template <class C>
	const_iter_range<C> make_iter_range(C const &X)
	{
		return const_iter_range<C>(X);
	}
	template <class C>
	const_iter_range<C> make_iter_range(C &X)
	{
		return iter_range<C>(X);
	}
}

//EOF

// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_MAP_H
#define BOOST_MONOTONIC_MAP_H

#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/container.hpp>

namespace boost
{
	namespace monotonic
	{
		/// A map that uses a monotonic allocator, and respects that allocator
		/// when creating new referent instances
		template <class K, class T, class P = std::less<K> >
		struct map : detail::MonotonicContainer<map<K,T,P> >
		{
			typedef detail::MonotonicContainer<std::map<K,T,P, allocator<K> > > Parent;
			typedef detail::Creator<detail::IsMonotonic<T>::value, T> Creator;

			typedef P Predicate;
			typedef allocator<K> Allocator;
			typedef std::map<K,T,P,Allocator > Map, Implementation;
			typedef typename Map::iterator iterator;
			typedef typename Map::const_iterator const_iterator;
			typedef typename Map::mapped_type mapped_type;
			typedef typename Map::value_type value_type;
			typedef typename Map::key_type key_type;
			typedef typename Map::size_type size_type;

		private:
			Implementation impl;
			Predicate pred;

		public:

			map() { }
			map(Allocator const &A)
				: impl(Predicate(), A) { }
			map(Predicate Pr, Allocator const &A) 
				: impl(Pr, A), pred(Pr) { }
			template <class II>
			map(II F, II L, Allocator const &A, Predicate const &Pr = Predicate())
				: impl(F,L,Pr,A), pred(Pr) { }
			template <class II>
			map(II F, II L, Predicate const &Pr, Allocator const &A)
				: impl(F,L,Pr,A), pred(Pr) { }

			Allocator get_allocator() const
			{
				return impl.get_allocator();
			}
			void clear()
			{
				impl.clear();
			}
			size_type size() const
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

			void insert(const value_type& value)
			{
				impl.insert(value);
			}
			void erase(iterator first)
			{
				impl.erase(first);
			}
			void erase(iterator first, iterator last)
			{
				impl.erase(first, last);
			}
			size_type erase(key_type const &key)
			{
				return impl.erase(key);
			}
			mapped_type& operator[](const key_type& key)
			{
				iterator where = impl.lower_bound(key);
				if (where == impl.end() || pred(key, where->first))
				{
					where = impl.insert(where, value_type(key, Creator::Create(GetStorage())));
				}
				return where->second;
			}
			iterator find(key_type const &key)
			{
				return impl.find(key);
			}
			const_iterator find(key_type const &key) const
			{
				return impl.find(key);
			}
		};

	}
}

#endif // BOOST_MONOTONIC_MAP_H

//EOF

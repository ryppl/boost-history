// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_CONTAINER_HPP
#define BOOST_MONOTONIC_CONTAINER_HPP

#include <vector>
#include <list>
#include <map>
#include <set>
#include <boost/monotonic/allocator.hpp>

namespace boost
{
	namespace monotonic
	{
		namespace detail
		{
			struct monotonic_container_base { virtual ~monotonic_container_base() { } };

			template <class Impl>
			struct monotonic_container;

			template <class T>
			struct is_monotonic : mpl::bool_<is_convertible<T *, monotonic_container_base *>::value> { };
			
			template <class Impl>
			struct is_monotonic<monotonic_container<Impl> > : mpl::true_ { };

			template <class Impl>
			struct monotonic_container : monotonic_container_base
			{
				typedef Impl Derived;

				storage_base &get_storage() const
				{
					Derived const &self = static_cast<Derived const &>(*this);
					storage_base *store = self.get_allocator().get_storage();
					if (store == 0)
						//throw_exception(no_storage());
						throw no_storage();
					return *store;
				}
			};

			// match against the standard containers for allocators

			template <class T, class U>
			struct is_monotonic<std::list<T, allocator<U> > > : mpl::true_ { };

			template <class T, class U>
			struct is_monotonic<std::vector<T, allocator<U> > > : mpl::true_ { };

			template <class K, class T, class Pred, class U>
			struct is_monotonic<std::map<K, T, Pred, allocator<U> > > : mpl::true_ { };

			template <class T, class Pred, class U>
			struct is_monotonic<std::set<T, Pred, allocator<U> > > : mpl::true_ { };

			// match against the standard containers for shared allocators

			template <class T, class U>
			struct is_monotonic<std::list<T, shared_allocator<U> > > : mpl::true_ { };

			template <class T, class U>
			struct is_monotonic<std::vector<T, shared_allocator<U> > > : mpl::true_ { };

			template <class K, class T, class Pred, class U>
			struct is_monotonic<std::map<K, T, Pred, shared_allocator<U> > > : mpl::true_ { };

			template <class T, class Pred, class U>
			struct is_monotonic<std::set<T, Pred, shared_allocator<U> > > : mpl::true_ { };
		}
	}
}

#endif // BOOST_MONOTONIC_CONTAINER_HPP

//EOF

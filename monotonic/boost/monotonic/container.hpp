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
			template <bool, class T>
			struct Creator;

			template <class Impl>
			struct MonotonicContainer;

			template <class>
			struct IsMonotonic : boost::mpl::false_ { };

			template <class Impl>
			struct IsMonotonic<MonotonicContainer<Impl> > : boost::mpl::true_ { };

			template <class Impl>
			struct MonotonicContainer
			{
				typedef Impl Derived;

				virtual ~MonotonicContainer() { }
				storage_base &GetStorage() const
				{
					Derived const &self = dynamic_cast<Derived const &>(*this);
					storage_base *store = self.get_allocator().get_storage();
					if (store == 0)
						//throw_exception(no_storage());
						throw no_storage();
					return *store;
				}
			};

			template <bool is_monotonic_container, class T>
			struct Creator
			{
				static T Create(storage_base &)
				{
					return T();
				}
			};
			template <class T>
			struct Creator<true, T>
			{
				static T Create(storage_base &storage)
				{
					return T(storage);
				}
			};

			// match against the standard containers

			template <class T, class U>
			struct IsMonotonic<std::list<T, allocator<U> > > : boost::mpl::true_ { };

			template <class T, class U>
			struct IsMonotonic<std::vector<T, allocator<U> > > : boost::mpl::true_ { };

			template <class K, class T, class Pred, class U>
			struct IsMonotonic<std::map<K, T, Pred, allocator<U> > > : boost::mpl::true_ { };

			template <class T, class Pred, class U>
			struct IsMonotonic<std::set<T, Pred, allocator<U> > > : boost::mpl::true_ { };
		}
	}
}

#endif // BOOST_MONOTONIC_CONTAINER_HPP

//EOF

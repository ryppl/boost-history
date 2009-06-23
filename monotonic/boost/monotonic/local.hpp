// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_LOCAL_HPP
#define BOOST_MONOTONIC_LOCAL_HPP

#include <boost/monotonic/storage.hpp>

namespace boost
{
	namespace monotonic
	{
		/// RIIA for storage
		template <class Region, class Access>
		struct local
		{
			typedef static_storage<
				Region
				, Access
				, DefaultSizes::InlineSize
				, DefaultSizes::MinHeapIncrement
				, std::allocator<void> > StaticStorage;

			local()
			{
			}
			~local()
			{
				reset();
			}
			static typename StaticStorage::StorageType &get_storage()
			{
				return StaticStorage::get_storage();
			}
			static void reset()
			{
				get_storage().reset();
			}
			static void release()
			{
				get_storage().release();
			}

			template <class Ty>
			Ty &create()
			{
				return get_storage().create<Ty>();
			}
			template <class Ty>
			Ty &create(Ty const &X)
			{
				return get_storage().create<Ty>(X);
			}
			template <class Ty>
			void destroy(Ty &X)
			{
				get_storage().destroy(X);
			}

		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_LOCAL_HPP

//EOF

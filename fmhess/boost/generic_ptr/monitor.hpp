//
//  generic_ptr/monitor.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED
#define BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED

#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/detail/unique_lock.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
	namespace generic_ptr
	{
		template<typename T, typename PointerToMutex>
		class monitor
		{
			typedef monitor this_type;
			//FIXME: take advantage of rvalue refs if available
			template<typename Mutex>
			class temporary_monitor_lock
			{
			public:
				temporary_monitor_lock(T p, Mutex &m): _object_p(p),
					_lock(new detail::unique_lock<Mutex>(m))
				{}
#ifndef BOOST_NO_RVALUE_REFERENCES
				temporary_monitor_lock(temporary_monitor_lock &&other):
					_object_p(std::move(other._object_p),
					_lock(std::move(other._lock))
				{}
#endif
				T operator->() const
				{
					return _object_p;
				}
			private:
				T _object_p;
				shared<detail::unique_lock<Mutex> *> _lock;
			};

		public:
			typedef typename pointer_traits<T>::value_type value_type;
			typedef T pointer;
			typedef typename pointer_traits<T>::reference reference;
			typedef PointerToMutex pointer_to_mutex_type;
			typedef typename pointer_traits<PointerToMutex>::value_type mutex_type;

			template<typename ValueType>
			struct rebind
			{
				typedef monitor<typename generic_ptr::rebind<pointer, ValueType>::other, PointerToMutex > other;
			};

			monitor(): px(), _mutex_p()
			{}
			template<typename U>
			monitor( U p, PointerToMutex mutex_p ): px( p ), _mutex_p(mutex_p)
			{}
			template<typename U>
			monitor(const monitor<U, pointer_to_mutex_type> & other): px(other.px), _mutex_p(other._mutex_p)
			{}
#ifndef BOOST_NO_RVALUE_REFERENCES
			monitor(monitor && other): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p)
			{}
			template<typename U>
			monitor(monitor<U> && other): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p)
			{}
#endif

			void swap(monitor & other)
			{
				boost::swap(px, other.px);
				boost::swap(_mutex_p, other._mutex_p);
			}

			monitor & operator=(const monitor & other)
			{
				monitor(other).swap(*this);
				return *this;
			}

			template<typename U>
			monitor & operator=(const monitor<U, pointer_to_mutex_type> & other)
			{
				monitor(other).swap(*this);
				return *this;
			}
#ifndef BOOST_NO_RVALUE_REFERENCES
			monitor & operator=(monitor && other)
			{
				monitor(std::move(other)).swap(*this);
				return *this;
			}
			template<typename U>
			monitor & operator=(monitor<U, pointer_to_mutex_type> && other)
			{
				monitor(std::move(other)).swap(*this);
				return *this;
			}
#endif
			void reset()
			{
				monitor().swap(*this);
			}
			template<typename U> void reset(U object_p, pointer_to_mutex_type mutex_p)
			{
				monitor(object_p, mutex_p).swap(*this);
			}

			pointer get() const {return px;}
			pointer_to_mutex_type get_mutex() const {return _mutex_p;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

			temporary_monitor_lock<mutex_type> operator->() const
			{
				return temporary_monitor_lock<mutex_type>(px, *_mutex_p);
			}
		private:
			pointer px;
			pointer_to_mutex_type _mutex_p;
		};

		template<typename T, typename PointerToMutex>
		T get_pointer(const monitor<T, PointerToMutex> &mp)
		{
			return mp.get();
		}
	} // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED

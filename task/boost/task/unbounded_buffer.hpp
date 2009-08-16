
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_UNBOUNDED_BUFFER_H
#define BOOST_TASK_UNBOUNDED_BUFFER_H

#include <deque>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <boost/task/semaphore.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
template< typename T >
class unbounded_buffer
{
private:
	class base
	{
	private:
		class guard : private noncopyable
		{
		private:
			semaphore	&	sem_;

		public:
			guard( semaphore & sem)
			: sem_( sem)
			{ sem_.wait(); }

			~guard()
			{ sem_.post(); }
		};

		semaphore			protection_sem_;
		semaphore			consumer_sem_;
		std::deque< T >		queue_;

		base( base &);
		base & operator=( base const&);

		bool empty_() const
		{ return queue_.empty(); }

		bool consumers_activate_() const
		{ return ! empty_(); }

	public:
		base()
		:
		protection_sem_( 1),
		consumer_sem_( 0),
		queue_()
		{}

		void put( T const& t)
		{
			{
				guard lk( protection_sem_);
				queue_.push_back( t);
			}
			consumer_sem_.post();
		}

		bool take( optional< T > & t)
		{
			consumer_sem_.wait();
			{
				guard lk( protection_sem_);
				t = queue_.front();
				queue_.pop_front();
			}
			return true;
		}
	};

	shared_ptr< base >		impl_;

public:
	unbounded_buffer()
	: impl_( new base)
	{}

	void put( T const& t)
	{ impl_->put( t); }

	void take( optional< T > & t)
	{ impl_->take( t); }
};
}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_UNBOUNDED_BUFFER_H

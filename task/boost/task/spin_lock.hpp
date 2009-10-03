
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_SPIN_LOCK_H
#define BOOST_TASK_SPIN_LOCK_H

#include <algorithm>

#include <boost/thread/thread_time.hpp>

#include <boost/task/exceptions.hpp>

namespace boost {
namespace task {

template< typename Mutex >
class spin_lock
{
private:
	typedef bool spin_lock::*unspecified_bool_type;

	Mutex	*	mtx_; 
	bool        locked_;

	spin_lock( spin_lock &);
	spin_lock & operator=( spin_lock &);

public:
	explicit spin_lock( Mutex & mtx)
	: mtx_( & mtx), locked_( false)
	{
		mtx_->lock();
		locked_ = true;
	}

	spin_lock( Mutex & mtx, system_time const& abs_time)
	: mtx_( & mtx), locked_( mtx_->timed_lock( abs_time) )
	{}

	template< typename TimeDuration >
	spin_lock( Mutex & mtx, TimeDuration const& rel_time)
	: mtx_( & mtx), locked_( mtx_->timed_lock( rel_time) )
	{}

	~spin_lock()
	{
		try
		{ if ( locked_ && mtx_) mtx_->unlock(); }
		catch (...) {}
	}

	void lock()
	{
		if ( ! mtx_ || locked_)
		   throw lock_error();
		mtx_->lock();
		locked_ = true;
	}

	bool try_lock()
	{
		if ( ! mtx_ || locked_)
		   throw lock_error();
		locked_ = mtx_->try_lock();
		return locked_;
	}

	bool timed_lock( system_time const& abs_time)
	{
	   if ( ! mtx_ || locked_)
	      throw lock_error();
	   locked_ = mtx_->timed_lock( abs_time);
	   return locked_;
	}

	template< typename TimeDuration >
	bool timed_lock( TimeDuration const& rel_time)
	{ return timed_lock( get_system_time() + rel_time); }

	void unlock()
	{
		if ( ! mtx_ || ! locked_)
		   throw lock_error();
		mtx_->unlock();
		locked_ = false;
	}

	bool owns() const
	{ return locked_ && mtx_; }

	operator unspecified_bool_type() const
	{ return locked_ ? & locked_ : 0; }

	bool operator!() const
	{ return ! locked_; }

	Mutex * mutex() const
	{ return mtx_; }

	void swap( spin_lock & other)
	{
		std::swap( mtx_, other.mtx_);
		std::swap( locked_, other.locked_);
	}
};

}}

#endif // BOOST_TASK_SPIN_LOCK_H

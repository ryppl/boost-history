
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::scoped_lock

#ifndef BOOST_FIBERS_UNIQUE_LOCK_H
#define BOOST_FIBERS_UNIQUE_LOCK_H

#include <algorithm>

#include <boost/thread/locks.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/fiber/exceptions.hpp>

namespace boost {
namespace fibers {

template< typename Mutex >
class unique_lock
{
private:
	typedef unique_lock< Mutex >		lock_t;
	typedef bool unique_lock::*unspecified_bool_type;

	Mutex	*	mtx_; 
	bool        locked_;

	unique_lock( unique_lock &);
	unique_lock & operator=( unique_lock &);

public:
	unique_lock() :
		mtx_( 0),
	   	locked_( false)
	{}

	explicit unique_lock( Mutex & mtx) :
		mtx_( & mtx),
		locked_( false)
	{
		mtx_->lock();
		locked_ = true;
	}

	unique_lock( Mutex & mtx, adopt_lock_t) :
		mtx_( & mtx),
		locked_( true)
	{}

	unique_lock( Mutex & mtx, defer_lock_t) :
		mtx_( & mtx),
		locked_( false)
	{}

	unique_lock( Mutex & mtx, try_to_lock_t) :
		mtx_( & mtx),
		locked_( mtx_->try_lock() )
	{}

	unique_lock( Mutex & mtx, system_time const& abs_time) :
		mtx_( & mtx),
		locked_( mtx_->timed_lock( abs_time) )
	{}

	template< typename TimeDuration >
	unique_lock( Mutex & mtx, TimeDuration const& rel_time) :
		mtx_( & mtx),
		locked_( mtx_->timed_lock( rel_time) )
	{}

	~unique_lock()
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

	bool owns_lock() const
	{ return locked_ && mtx_; }

	operator unspecified_bool_type() const
	{ return locked_ ? & lock_t::locked_ : 0; }

	bool operator!() const
	{ return ! locked_; }

	Mutex * mutex() const
	{ return mtx_; }

	Mutex * release()
	{
		Mutex * mtx = mtx_;
		mtx_ = 0;
		locked_ = false;
		return mtx;
	}

	void swap( unique_lock & other)
	{
		std::swap( mtx_, other.mtx_);
		std::swap( locked_, other.locked_);
	}
};

}}

#endif // BOOST_FIBERS_UNIQUE_LOCK_H

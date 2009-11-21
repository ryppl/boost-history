
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::scoped_lock

#ifndef BOOST_FIBERS_LOCK_H
#define BOOST_FIBERS_LOCK_H

#include <algorithm>

#include <boost/config.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/fiber/detail/move.hpp>

#include <boost/fiber/exceptions.hpp>

namespace boost {
namespace fibers {

template< typename Mutex >
class lock_guard
{
private:
	Mutex	&	mtx_;
	
	explicit lock_guard( lock_guard &);
	lock_guard & operator=( lock_guard &);

public:
	explicit lock_guard( Mutex & mtx) :
	    mtx_( mtx)
	{ mtx_.lock(); }
	
	lock_guard( Mutex & mtx, adopt_lock_t) :
	    mtx_( mtx)
	{}
	
	~lock_guard()
	{ mtx_.unlock(); }
};

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

#ifdef BOOST_HAS_RVALUE_REFS
        unique_lock( unique_lock && other) :
		mtx_( other.mtx_),
		locked_( other.locked_)
        {
            other.locked_ = false;
            other.mtx_ = 0;
        }

        unique_lock< Mutex > && move()
        { return static_cast< unique_lock< Mutex > && >( * this); }

        unique_lock & operator=( unique_lock< Mutex > && other)
        {
            unique_lock tmp( other);
            swap( tmp);
            return * this;
        }

        void swap( unique_lock && other)
        {
            std::swap( mtx_, other.mtx_);
            std::swap( locked_, other.locked_);
        }
#else
        unique_lock( boost::detail::fiber_move_t< unique_lock< Mutex > > other) :
		mtx_( other->mtx_),
		locked_( other->locked_)
        {
            other->locked_ = false;
            other->mtx_ = 0;
        }

        operator boost::detail::fiber_move_t< unique_lock< Mutex > >()
        { return move(); }

        boost::detail::fiber_move_t< unique_lock< Mutex > > move()
        { return boost::detail::fiber_move_t< unique_lock< Mutex > >( * this); }

        unique_lock & operator=( boost::detail::fiber_move_t< unique_lock< Mutex > > other)
        {
            unique_lock tmp( other);
            swap( tmp);
            return * this;
        }

        void swap( boost::detail::fiber_move_t< unique_lock< Mutex > > other)
        {
            std::swap( mtx_, other->mtx_);
            std::swap( locked_, other->locked_);
        }
#endif

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

#ifdef BOOST_HAS_RVALUE_REFS
    template< typename Mutex >
    void swap( unique_lock< Mutex > && lhs, unique_lock< Mutex > && rhs)
    { lhs.swap( rhs); }
#else
    template< typename Mutex >
    void swap( unique_lock< Mutex > & lhs, unique_lock< Mutex > & rhs)
    { lhs.swap(rhs); }
#endif

#ifdef BOOST_HAS_RVALUE_REFS
    template< typename Mutex >
    inline unique_lock< Mutex > && move( unique_lock< Mutex > && ul)
    { return ul; }
#endif

}}

#endif // BOOST_FIBERS_LOCK_H

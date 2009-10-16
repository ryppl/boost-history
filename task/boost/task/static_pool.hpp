
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_STATIC_POOL_H
#define BOOST_TASK_STATIC_POOL_H

#include <cstddef>

#include <boost/config.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/detail/bind_processor.hpp>
#include <boost/task/detail/pool_base.hpp>

#include <boost/task/detail/worker_group.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/local_rr_ums.hpp>
#include <boost/task/poolsize.hpp>
#include <boost/task/scanns.hpp>
#include <boost/task/stacksize.hpp>
#include <boost/task/task.hpp>
#include <boost/task/watermark.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {

template<
	typename Queue,
	typename UMS = local_rr_ums
>
class static_pool
{
public:
	typedef Queue	queue_type;
	typedef UMS		ums_type;

private:
	typedef detail::pool_base< queue_type, ums_type >	base_type;

	template< typename T, typename X, typename Z >
	friend class detail::worker_object;

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	struct tag_bind_to_processors {};
# endif
	
	shared_ptr< base_type >		pool_;

	static_pool( static_pool &);
	static_pool & operator=( static_pool &);

public:
	static_pool() :
		pool_()
	{}
	
	explicit static_pool(
			poolsize const& psize,
			posix_time::time_duration const& asleep = posix_time::microseconds( 10),
			scanns const& max_scns = scanns( 20),
			stacksize const& stack_size = stacksize( 64000) ) :
		pool_( new base_type( psize, asleep, max_scns, stack_size) )
	{}

	explicit static_pool(
			poolsize const& psize,
			high_watermark const& hwm,
			low_watermark const& lwm,
			posix_time::time_duration const& asleep = posix_time::microseconds( 100),
			scanns const& max_scns = scanns( 20),
			stacksize const& stack_size = stacksize( 64000) ) :
		pool_( new base_type( psize, hwm, lwm, asleep, max_scns, stack_size) )
	{}

# if defined(BOOST_HAS_PROCESSOR_BINDINGS)
	explicit static_pool(
			tag_bind_to_processors,
			posix_time::time_duration const& asleep = posix_time::microseconds( 10),
			scanns const& max_scns = scanns( 20),
			stacksize const& stack_size = stacksize( 64000) ) :
		pool_( new base_type( asleep, max_scns, stack_size) )
	{}

	explicit static_pool(
			tag_bind_to_processors,
			high_watermark const& hwm,
			low_watermark const& lwm,
			posix_time::time_duration const& asleep = posix_time::microseconds( 100),
			scanns const& max_scns = scanns( 20),
			stacksize const& stack_size = stacksize( 64000) ) :
		pool_( new base_type( hwm, lwm, asleep, max_scns, stack_size) )
	{}

	static tag_bind_to_processors bind_to_processors()
	{ return tag_bind_to_processors(); }
# endif

# if defined(BOOST_HAS_RVALUE_REFS)
	static_pool( static_pool && other) :
		pool_()
	{ pool_.swap( other.pool_); }

	static_pool & operator=( static_pool && other)
	{
	    static_pool tmp( static_cast< static_pool && >( other) );
	    swap( tmp);
	    return * this;
	}

	static_pool && move()
	{ return static_cast< static_pool && >( * this); }
# else
	static_pool( boost::detail::thread_move_t< static_pool > other) :
		pool_()
	{ pool_.swap( other->pool_); }

	static_pool & operator=( boost::detail::thread_move_t< static_pool > other)
	{
		static_pool tmp( other);
		swap( tmp);
		return * this;
	}

	operator boost::detail::thread_move_t< static_pool >()
	{ return move(); }

	boost::detail::thread_move_t< static_pool > move()
	{
		boost::detail::thread_move_t< static_pool > t( * this);
		return t;
	}
# endif

	void interrupt_all_worker()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->interrupt_all_worker();
	}

	void shutdown()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->shutdown();
	}

	const void shutdown_now()
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->shutdown_now();
	}

	std::size_t size()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->size();
	}

	bool closed()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->closed();
	}

	std::size_t upper_bound()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->upper_bound();
	}

	void upper_bound( high_watermark const& hwm)
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->upper_bound( hwm);
	}

	std::size_t lower_bound()
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->lower_bound();
	}

	void lower_bound( low_watermark const lwm)
	{
		if ( ! pool_)
			throw pool_moved();
		pool_->lower_bound( lwm);
	}

	template< typename R >
	handle< R > submit( task< R > t)
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->submit( boost::move( t) );
	}

	template< typename R, typename Attr >
	handle< R > submit( task< R > t, Attr const& attr)
	{
		if ( ! pool_)
			throw pool_moved();
		return pool_->submit( boost::move( t), attr);
	}

	typedef typename shared_ptr< base_type >::unspecified_bool_type	unspecified_bool_type;

	operator unspecified_bool_type() const // throw()
	{ return pool_; }

	bool operator!() const // throw()
	{ return ! pool_; }

	void swap( static_pool & other) // throw()
	{ pool_.swap( other.pool_); }
};

}

template< typename Queue, typename UMS >
void swap( task::static_pool< Queue, UMS > & l, task::static_pool< Queue, UMS > & r)
{ return l.swap( r); }

# if defined(BOOST_HAS_RVALUE_REFS)
template< typename Queue, typename UMS >
task::static_pool< Queue, UMS > && move( task::static_pool< Queue, UMS > && t)
{ return t; }
# else
template< typename Queue, typename UMS >
task::static_pool< Queue, UMS >  move( boost::detail::thread_move_t< task::static_pool< Queue, UMS > > t)
{ return task::static_pool< Queue, UMS >( t); }
# endif

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_STATIC_POOL_H


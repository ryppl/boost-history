
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_WORKER_H
#define BOOST_TASK_DETAIL_WORKER_H

#include <cstddef>
#include <utility>

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/random.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/detail/config.hpp>
#include <boost/task/detail/fiber.hpp>
#include <boost/task/detail/guard.hpp>
#include <boost/task/detail/wsq.hpp>
#include <boost/task/poolsize.hpp>
#include <boost/task/scanns.hpp>
#include <boost/task/stacksize.hpp>
#include <boost/task/semaphore.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost { namespace task {
namespace detail
{

struct worker_base
{
	virtual ~worker_base() {}

	virtual const thread::id get_id() const = 0;

	virtual void join() const = 0;

	virtual void interrupt() const = 0;

	virtual void put( callable const&) = 0;

	virtual bool try_steal( callable &) = 0;

	virtual void signal_shutdown() = 0;

	virtual void signal_shutdown_now() = 0;

	virtual void run() = 0;

	virtual void reschedule_until( function< bool() > const&) = 0;

	virtual void block() = 0;
};

template<
	typename Pool,
	typename UMS,
	typename Worker
>
class worker_object : public worker_base,
					  private noncopyable
{
private:
	class random_idx
	{
	private:
		rand48 rng_;
		uniform_int<> six_;
		variate_generator< rand48 &, uniform_int<> > die_;

	public:
		random_idx( std::size_t size)
		:
		rng_(),
		six_( 0, size - 1),
		die_( rng_, six_)
		{}

		std::size_t operator()()
		{ return die_(); }
	};

	typedef shared_ptr< thread >	thread_t;

	Pool					&	pool_;
	UMS						&	ums_;
	thread_t					thrd_;
	fiber::sptr_t				fib_;
	wsq							wsq_;
	semaphore					shtdwn_sem_;
	semaphore					shtdwn_now_sem_;
	bool						shtdwn_;
	posix_time::time_duration	asleep_;
	scanns						max_scns_;
	std::size_t					scns_;
	std::size_t					stack_size_;
	random_idx					rnd_idx_;

	void execute_( callable & ca)
	{
		BOOST_ASSERT( ! ca.empty() );
		{
			context_guard lk( ca, thrd_);
			ca();
		}
		ca.clear();
		BOOST_ASSERT( ca.empty() );
	}

	bool take_global_callable_(
			callable & ca,
			posix_time::time_duration const& asleep)
	{ return pool_.queue_.take( ca, asleep); }

	bool try_take_global_callable_( callable & ca)
	{ return pool_.queue_.try_take( ca); }

	bool try_take_local_callable_( callable & ca)
	{ return wsq_.try_take( ca); }
	
	bool try_steal_other_callable_( callable & ca)
	{
		std::size_t idx( rnd_idx_() );
		for ( std::size_t j( 0); j < pool_.wg_.size(); ++j)
		{
			// TODO: not thread-safe -> segfault
			Worker other( pool_.wg_[idx]);
			if ( this_thread::get_id() == other.get_id() ) continue;
			if ( ++idx >= pool_.wg_.size() ) idx = 0;
			if ( other.try_steal( ca) )
				return true;
		}
		return false;
	}

	bool try_blocked_fibers_()
	{
		if ( ums_.has_blocked() )
		{
			fiber::sptr_t this_fib = fib_;
			BOOST_ASSERT( this_fib);
			ums_.put_runnable( this_fib);
			fiber::sptr_t blocked_fib;
			ums_.try_take_blocked( blocked_fib);
			BOOST_ASSERT( blocked_fib);
			fib_ = blocked_fib;
			BOOST_ASSERT( this_fib->running() );
			this_fib->switch_to( blocked_fib);
			BOOST_ASSERT( this_fib->running() );
			fib_ = this_fib;
			return true;
		}
		return false;
	}

	void process_( bool all)
	{
		try_blocked_fibers_();
		callable ca;
		if ( all ? try_take_local_callable_( ca) || 
					try_steal_other_callable_( ca) ||
					try_take_global_callable_( ca)
				 : try_take_local_callable_( ca) )
		{
			execute_( ca);
			scns_ = 0;
		}
		else
		{
			guard grd( pool_.idle_worker_);
			++scns_;
			if ( scns_ >= max_scns_)
			{
				if ( pool_.size_() > pool_.idle_worker_)
				{
					if ( take_global_callable_( ca, asleep_) )
						execute_( ca);
				}
				else if ( ! ums_.has_blocked() )
				{
					if ( take_global_callable_( ca) )
						execute_( ca);
				}
				else
					this_thread::yield();
				scns_ = 0;
			}
			else
				this_thread::yield();
		}
	}

	void run_()
	{	
		while ( ! shutdown_() )
		{
			try_blocked_fibers_();
			callable ca;
			if ( try_take_local_callable_( ca) || 
				 try_steal_other_callable_( ca) ||
				 try_take_global_callable_( ca) )
			{
				execute_( ca);
				scns_ = 0;
			}
			else
			{
				guard grd( pool_.idle_worker_);
				++scns_;
				if ( scns_ >= max_scns_)
				{
					if ( pool_.size_() > pool_.idle_worker_)
					{
						if ( take_global_callable_( ca, asleep_) )
							execute_( ca);
					}
					else if ( ! ums_.has_blocked() )
					{
						try
						{ this_thread::sleep( asleep_); }
						catch ( thread_interrupted const&)
						{ return; }
					}
					scns_ = 0;
				}
				else
					this_thread::yield();
			}
		}
	}

	bool shutdown_()
	{
		if ( shutdown__() && pool_.queue_.empty() && ! ums_.has_blocked() )
			return true;
		else if ( shutdown_now__() )
			return true;
		return false;
	}

	bool shutdown__()
	{
		if ( ! shtdwn_)
			shtdwn_ = shtdwn_sem_.try_wait();
		return shtdwn_;
	}
	
	bool shutdown_now__()
	{ return shtdwn_now_sem_.try_wait(); }

public:
	worker_object(
		Pool & pool,
		UMS & ums,
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size,
		function< void() > const& fn)
	:
	pool_( pool),
	ums_( ums),
	thrd_( new thread( fn) ),
	fib_(),
	wsq_(),
	shtdwn_sem_( 0),
	shtdwn_now_sem_( 0),
	shtdwn_( false),
	asleep_( asleep),
	max_scns_( max_scns),
	scns_( 0),
	stack_size_( stack_size),
	rnd_idx_( psize)
	{ BOOST_ASSERT( ! fn.empty() ); }

	const thread::id get_id() const
	{ return thrd_->get_id(); }

	void join() const
	{ thrd_->join(); }

	void
	interrupt() const
	{ thrd_->interrupt(); }

	void signal_shutdown()
	{ shtdwn_sem_.post(); }
	
	void signal_shutdown_now()
	{ shtdwn_now_sem_.post(); }

	void put( callable const& ca)
	{
		BOOST_ASSERT( ! ca.empty() );
		wsq_.put( ca);
	}

	bool try_steal( callable & ca)
	{ return wsq_.try_steal( ca); }

	void run()
	{
		BOOST_ASSERT( get_id() == this_thread::get_id() );

		fiber::convert_thread_to_fiber();

		ums_.attach();

		fib_ = fiber::create(
			bind( & worker_object::run_, this),
			stack_size_);
		fib_->run();
		BOOST_ASSERT( fib_->exited() );
	}

	void reschedule_until( function< bool() > const& pred)
	{
		while ( ! pred() )
			block();
	}

	void block()
	{
		fiber::sptr_t this_fib = fib_;
		ums_.put_blocked( this_fib);
		fiber::sptr_t runnable_fib;
		if ( ums_.has_runnable() )
			ums_.try_take_runnable( runnable_fib);
		else
			runnable_fib = fiber::create(
				bind( & worker_object::run_, this),
				stack_size_);
		BOOST_ASSERT( runnable_fib);
		fib_ = runnable_fib;
		BOOST_ASSERT( this_fib->running() );
		this_fib->switch_to( runnable_fib);
		BOOST_ASSERT( this_fib->running() );
		fib_ = this_fib;
	}
};

class BOOST_TASK_DECL worker
{
private:
	static thread_specific_ptr< worker >	tss_;

	shared_ptr< worker_base >	impl_;

public:
	template< typename Pool, typename UMS >
	worker(
		Pool & pool,
		UMS & ums,
		poolsize const& psize,
		posix_time::time_duration const& asleep,
		scanns const& max_scns,
		stacksize const& stack_size,
		function< void() > const& fn)
	:
	impl_(
		new worker_object< Pool, UMS, worker >(
			pool,
			ums,
			psize,
			asleep,
			max_scns,
			stack_size,
			fn) )
	{}

	const thread::id get_id() const;

	void join() const;
	void interrupt() const;
	void signal_shutdown();
	void signal_shutdown_now();

	void put( callable const&);
	bool try_steal( callable &);

	void run();
	void reschedule_until( function< bool() > const&);
	void block();

	static worker * tss_get();
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_WORKER_H


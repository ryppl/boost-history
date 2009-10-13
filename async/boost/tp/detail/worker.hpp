//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_WORKER_H
#define BOOST_TP_DETAIL_WORKER_H

#include <cstddef>
#include <utility>

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/detail/wsq.hpp>

namespace boost {
namespace tp {
namespace detail
{
class worker
{
private:
	static thread_specific_ptr< worker >	tss_;

	class impl : private noncopyable
	{
	private:
		typedef std::pair< callable, interrupter >	item;
		shared_ptr< thread >						thrd_;
		wsq< item > 								wsq_;
		interprocess::interprocess_semaphore		shtdwn_sem_;
		interprocess::interprocess_semaphore		shtdwn_now_sem_;
		bool										shtdwn_;
		std::size_t									scns_;

	public:
		impl( function< void() > const&);

		virtual ~impl() {}

		const shared_ptr< thread > thrd() const;

		const thread::id get_id() const;

		void join() const;

		void interrupt() const;

		void put( callable const&, interrupter const&);

		bool try_take( callable &, interrupter &);

		bool try_steal( callable &, interrupter &);

		bool empty();

		void signal_shutdown();

		void signal_shutdown_now();

		bool shutdown();

		bool shutdown_now();

		std::size_t scanns() const;

		void increment_scanns();

		void reset_scanns();

		virtual void reschedule_until( function< bool() > const&) = 0;
	};

	template< typename Pool >
	class impl_pool : public impl
	{
	private:
		Pool	&	p_;

	public:
		impl_pool( Pool & p, function< void() > const& fn)
		: impl( fn), p_( p)
		{}

		void reschedule_until( function< bool() > const& pred)
		{ p_.reschedule_until( pred); }

		Pool & get_pool() const
		{ return p_; }
	};

	shared_ptr< impl >	impl_;

public:
	template< typename Pool >
	worker(
		Pool & pool,
		function< void() > const& fn)
	: impl_( new impl_pool< Pool >( pool, fn) )
	{}

	const shared_ptr< thread > thrd() const;

	const thread::id get_id() const;

	void join() const;

	void interrupt() const;

	void put( callable const&, interrupter const&);

	bool try_take( callable &, interrupter &);

	bool try_steal( callable &, interrupter &);

	bool empty() const;

	void signal_shutdown();

	void signal_shutdown_now();

	bool shutdown();

	bool shutdown_now();

	std::size_t scanns() const;

	void increment_scanns();

	void reset_scanns();

	void reschedule_until( function< bool() > const&);

	template< typename Pool >
	Pool & get_thread_pool() const
	{
		impl_pool< Pool > * p( dynamic_cast< impl_pool< Pool > * >( impl_.get() ) );
		BOOST_ASSERT( p);
		return p->get_pool();
	}

	static worker * tss_get();

	static void tss_reset( worker * w);
};

} } }

#endif // BOOST_TP_DETAIL_WORKER_H


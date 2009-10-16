
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DISTRIB_RR_UMS_H
#define BOOST_TASK_DISTRIB_RR_UMS_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {

class distrib_rr_ums
{
private:
	class twolock_fifo
	{
		struct node
		{
			typedef shared_ptr< node >	sptr_t;

			detail::fiber::sptr_t	fib;
			sptr_t					next;
		};

		node::sptr_t		head_;
		mutex				head_mtx_;
		node::sptr_t		tail_;
		mutex				tail_mtx_;

		bool empty_()
		{ return head_ == get_tail_(); }

		node::sptr_t get_tail_()
		{
			lock_guard< mutex > lk( tail_mtx_);	
			node::sptr_t tmp = tail_;
			return tmp;
		}

		node::sptr_t pop_head_()
		{
			node::sptr_t old_head = head_;
			head_ = old_head->next;
			return old_head;
		}

	public:
		twolock_fifo() :
			head_( new node),
			head_mtx_(),
			tail_( head_),
			tail_mtx_()
		{}

		bool empty()
		{
			unique_lock< mutex > lk( head_mtx_);
			return empty_();
		}

		void put( detail::fiber::sptr_t const& fib)
		{
			node::sptr_t new_node( new node);
			{
				unique_lock< mutex > lk( tail_mtx_);
				tail_->fib = fib;
				tail_->next = new_node;
				tail_ = new_node;
			}
		}

		bool try_take( detail::fiber::sptr_t & fib)
		{
			unique_lock< mutex > lk( head_mtx_);
			if ( empty_() )
				return false;
			fib.swap( head_->fib);
			pop_head_();
			return fib;
		}
	};	

	twolock_fifo	runnable_;
	twolock_fifo	blocked_;

public:
	distrib_rr_ums() :
		runnable_(), blocked_()
	{}

	void attach()
	{}

	bool has_runnable()
	{ return ! runnable_.empty(); }

	bool has_blocked()
	{ return ! blocked_.empty(); }

	void put_runnable( detail::fiber::sptr_t const& fib)
	{ runnable_.put( fib); }

	void put_blocked( detail::fiber::sptr_t const& fib)
	{ blocked_.put( fib); }

	bool try_take_runnable( detail::fiber::sptr_t & fib)
	{ return runnable_.try_take( fib); }

	bool try_take_blocked( detail::fiber::sptr_t & fib)
	{ return blocked_.try_take( fib); }
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DISTRIB_RR_UMS_H


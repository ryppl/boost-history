
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_UNBOUNDED_FIFO_H
#define BOOST_FIBERS_UNBOUNDED_FIFO_H

#include <cstddef>

#include <boost/config.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/condition.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

template< typename T >
class unbounded_fifo
{
public:
	typedef optional< T >	value_type;

private:
	class impl : private noncopyable
	{
	private:
		struct node
		{
			typedef intrusive_ptr< node >	ptr;

			std::size_t		use_count;
			value_type		va;
			ptr				next;

			node() :
				use_count( 0),
				va(),
				next()
			{}

			inline friend void intrusive_ptr_add_ref( node * p)
			{ ++p->use_count; }
			
			inline friend void intrusive_ptr_release( node * p)
			{ if ( --p->use_count == 0) delete p; }
		};

		enum state
		{
			ACTIVE = 0,
			DEACTIVE
		};

		state				state_;
		typename node::ptr	head_;
		mutable mutex		head_mtx_;
		typename node::ptr	tail_;
		mutable mutex		tail_mtx_;
		condition			not_empty_cond_;
		std::size_t			use_count_;	

		bool active_() const
		{ return ACTIVE == state_; }

		void deactivate_()
		{ state_ = DEACTIVE; }

		bool empty_() const
		{ return head_ == get_tail_(); }

		typename node::ptr get_tail_() const
		{
			mutex::scoped_lock lk( tail_mtx_);	
			typename node::ptr tmp = tail_;
			return tmp;
		}

		typename node::ptr pop_head_()
		{
			typename node::ptr old_head = head_;
			head_ = old_head->next;
			return old_head;
		}

	public:
		template< typename Strategy >
		impl( scheduler< Strategy > & sched) :
			state_( ACTIVE),
			head_( new node() ),
			head_mtx_( sched),
			tail_( head_),
			tail_mtx_( sched),
			not_empty_cond_( sched),
			use_count_( 0)
		{}

		void deactivate()
		{ deactivate_(); }

		bool empty() const
		{
			mutex::scoped_lock lk( head_mtx_);
			return empty_();
		}

		void put( T const& t)
		{
			typename node::ptr new_node( new node() );
			{
				mutex::scoped_lock lk( tail_mtx_);

				tail_->va = t;
				tail_->next = new_node;
				tail_ = new_node;
			}
			not_empty_cond_.notify_one();
		}

		bool take( value_type & va)
		{
			mutex::scoped_lock lk( head_mtx_);
			bool empty = empty_();
			if ( ! active_() && empty)
				return false;
			if ( empty)
			{
				try
				{
					while ( active_() && empty_() )
						not_empty_cond_.wait( lk);
				}
				catch ( fiber_interrupted const&)
				{ return false; }
			}
			if ( ! active_() && empty_() )
				return false;
			swap( va, head_->va);
			pop_head_();
			return va;
		}

		bool try_take( value_type & va)
		{
			mutex::scoped_lock lk( head_mtx_);
			if ( empty_() )
				return false;
			swap( va, head_->va);
			pop_head_();
			return va;
		}

		friend void intrusive_ptr_add_ref( impl * p)
		{ ++( p->use_count_); }

		friend void intrusive_ptr_release( impl * p)
		{ if ( --( p->use_count_) == 1) delete p; }
	};

	intrusive_ptr< impl >	impl_;

public:
	template< typename Strategy >
	unbounded_fifo( scheduler< Strategy > & sched) :
		impl_( new impl( sched) )
	{}

	void deactivate()
	{ impl_->deactivate(); }

	bool empty() const
	{ return impl_->empty(); }

	void put( T const& t)
	{ impl_->put( t); }

	bool take( value_type & va)
	{ return impl_->take( va); }

	bool try_take( value_type & va)
	{ return impl_->try_take( va); }
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_UNBOUNDED_FIFO_H

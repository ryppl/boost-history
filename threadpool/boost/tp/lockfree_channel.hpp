#ifndef BOOST_TP_LOCKFREE_CHANNEL_H
#define BOOST_TP_LOCKFREE_CHANNEL_H

#include <vector>

#include <boost/assert.hpp>
#include <boost/optional.hpp>

#include <boost/tp/detail/atomic.hpp>
#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/exceptions.hpp>

namespace boost {
namespace tp
{

class lockfree_channel
{
public:
	class item
	{
	private:
		detail::callable	ca_;
		detail::interrupter	intr_;

	public:
		item(
			detail::callable const& ca,
			detail::interrupter const& intr)
		: ca_( ca), intr_( intr)
		{ BOOST_ASSERT( ! ca_.empty() ); }

		const detail::callable ca() const
		{ return ca_; }

		const detail::interrupter intr() const
		{ return intr_; }
	};

private:
	enum channel_state
	{
		channel_active  = 0,
		channel_deactive = 1,
		channel_deactive_now = 2
	};

	struct node_t;

	struct pointer_t
	{
		node_t		*	ptr;
		unsigned int	tag;

		pointer_t()
		: ptr( 0), tag( 0)
		{}

		pointer_t( node_t * ptr_)
		: ptr( ptr_), tag( 0)
		{}

		pointer_t( node_t * ptr_, unsigned int tag_)
		: ptr( ptr_), tag( tag_)
		{}

		pointer_t( pointer_t const& rhs)
		: ptr( 0), tag( 0)
		{
			detail::atomic_write_ptr( & ptr, rhs.ptr);
			detail::atomic_write_32( & tag, rhs.tag);
		}
		
		bool operator==( pointer_t const& rhs)
		{ return ptr == rhs.ptr && tag == rhs.tag; }
		
		bool operator!=( pointer_t const& rhs)
		{ return ptr != rhs.ptr || tag != rhs.tag; }
	};

	struct node_t
	{
		boost::optional< item >	itm;
		pointer_t				next;
		pointer_t				prev;

		node_t()
		: itm( boost::none), next(), prev()
		{}

		node_t( item const& itm_)
		: itm( itm_), next(), prev()
		{}
	};

	channel_state	state_;
	unsigned int	size_;
	pointer_t		tail_;
	pointer_t		head_;

	bool compare_exchange_( pointer_t & dest, pointer_t & cmp, pointer_t & value)
	{
		if ( detail::atomic_compare_exchange_ptr( & dest.ptr, cmp.ptr, value.ptr) )
		{
			detail::atomic_write_32( & dest.tag, value.tag);
			return true;
		}

		return false;
	}

	void fix_list_( pointer_t const& tail, pointer_t const& head)
	{
		pointer_t cur( tail), next, prev;
		while ( head_ == head && cur != head)
		{
			next = cur.ptr->next;
			if ( next.tag != cur.tag) return;
			prev = next.ptr->prev;
			pointer_t ncur( cur.ptr, cur.tag - 1);
			if ( prev != ncur)
				next.ptr->prev = ncur;
			cur = pointer_t( next.ptr, cur.tag - 1);
		}
	}

	void increment_size_()
	{ detail::atomic_inc_32( & size_); }

	void decrement_size_()
	{ detail::atomic_dec_32( & size_); }

public:
	lockfree_channel()
	: state_( channel_active), size_( 0), tail_(), head_()
	{ tail_.ptr = head_.ptr = new node_t; }

	~lockfree_channel()
	{ clear(); }

	void put( item const& itm)
	{
		pointer_t tail;
		node_t * node( new node_t( itm) );
		while ( active() )
		for(;;)
		{
			if ( ! active() )
				throw task_rejected("channel is not active");
			
			tail = pointer_t( tail_);
			node->next = pointer_t( tail.ptr, tail.tag + 1);
			pointer_t ntail( node, tail.tag + 1);
			if ( compare_exchange_( tail_, tail, ntail) )
			{
				tail.ptr->prev = pointer_t( node, tail.tag);
				increment_size_();
				break;
			}
		}
	}

	bool take(
		detail::callable & ca,
		detail::interrupter & intr)
	{
		pointer_t head, tail, first;
		node_t * dummy( 0);
		while ( ! deactive_now() && ! ( deactive() && empty() ) )
		{
			head = head_;
			tail = tail_;
			first = head.ptr->prev;
			boost::optional< item > val( head.ptr->itm);
			if ( head_ == head)
			{
				if ( val)
				{
					if ( tail != head)
					{
						if ( first.tag != head.tag)
						{
							fix_list_( tail, head);
							continue;
						}
					}
					else
					{
						dummy = new node_t;
						dummy->next = pointer_t( tail.ptr, tail.tag + 1);
						pointer_t ntail( dummy, tail.tag + 1);
						if ( compare_exchange_( tail_, tail, ntail) )
							head.ptr->prev = pointer_t( dummy, tail.tag);
						else
						{
							delete dummy;
							dummy = 0;
						}
						continue;
					}
					pointer_t nhead( first.ptr, head.tag + 1);
					if ( compare_exchange_( head_, head, nhead) )
					{
						ca = val->ca();
						intr = val->intr();
						delete head.ptr;
						head.ptr = 0;
						decrement_size_();
						return true;
					}
				}
				else
				{
					if ( tail.ptr == head.ptr)
						return false;
					else
					{
						if ( first.tag != head.tag)
						{
							fix_list_( tail, head);
							continue;
						}
						pointer_t nhead( first.ptr, head.tag + 1);
						compare_exchange_( head_, head, nhead);
					}
				}
			}
		}
		return false;
	}

	template< typename Duration >
	bool take(
		detail::callable & ca,
		detail::interrupter & intr,
		Duration const& rel_time)
	{ return take( ca, intr); }

	bool try_take(
		detail::callable & ca,
		detail::interrupter & intr)
	{ return take( ca, intr); }

	bool active() const
	{ return state_ == channel_active; }

	bool deactive() const
	{ return state_ == channel_deactive; }

	bool deactive_now() const
	{ return state_ == channel_deactive_now; }

	void activate()
	{ detail::atomic_write_32( ( unsigned int *) & state_, channel_active); }

	void deactivate()
	{
		if ( active() )
			detail::atomic_write_32(  ( unsigned int *) & state_, channel_deactive);

		BOOST_ASSERT( deactive() );
	}

	void deactivate_now()
	{
		if ( active() )
			detail::atomic_write_32(  ( unsigned int *) & state_, channel_deactive_now);

		BOOST_ASSERT( deactive_now() );
	}

	const std::vector< detail::callable > drain()
	{
		BOOST_ASSERT( deactive_now() );
		std::vector< detail::callable > unprocessed;
		unprocessed.reserve( size() );
		pointer_t head( head_);
		while ( head.ptr)
		{
			if ( head.ptr->itm)
				unprocessed.push_back( head.ptr->itm->ca() );
			head = head.ptr->prev;
		}
		clear();
		BOOST_ASSERT( empty() );
		return unprocessed;
	}

	void clear()
	{
		while ( head_.ptr)
		{
			pointer_t tmp( head_);
			head_ = tmp.ptr->prev;
			if ( tmp.ptr->itm)
				decrement_size_();
			delete tmp.ptr;
			tmp.ptr = 0;
		}
	}

	bool empty()
	{ return size_ <= 0; }

	bool full()
	{ return false; }

	std::size_t size()
	{ return size_; }
};

}}

#endif // BOOST_TP_LOCKFREE_CHANNEL_H

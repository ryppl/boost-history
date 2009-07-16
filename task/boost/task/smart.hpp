
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_SMART_H
#define BOOST_TASK_SMART_H

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/detail/meta.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
template<
	typename Attr,
	typename Ord,
	typename Enq,
	typename Deq
>
struct smart
{
	typedef detail::has_attribute	attribute_tag_type;
	typedef Attr					attribute_type;

	class impl
	{
	private:
		typedef Attr		attribute;
		typedef Deq			dequeue_op;
		typedef Enq			enqueue_op;
		typedef Ord			ordering;

	public:
		struct item
		{
			callable	ca;
			attribute	attr;
	
			item()
			: ca(), attr()
			{}

			item(
				callable const& ca_,
				attribute const& attr_)
			: ca( ca_), attr( attr_)
			{ BOOST_ASSERT( ! ca.empty() ); }

			void swap( item & other)
			{
				ca.swap( other.ca);
				std::swap( attr, other.attr);
			}
		};
	
	private:
		typedef multi_index::multi_index_container<
			item,
			multi_index::indexed_by<
				multi_index::ordered_non_unique<
					multi_index::member<
						item,
						attribute,
						& item::attr
					>,
					ordering
				>
			>
		>														list;
		typedef typename list::template nth_index< 0 >::type	index;
	
		list		lst_;
		index	&	idx_;
		enqueue_op	enq_op_;
		dequeue_op	deq_op_;
	
	public:
		typedef typename index::iterator		iterator;
		typedef typename index::const_iterator	const_iterator;
	
		impl(
			enqueue_op const& enq_op = enqueue_op(),
			dequeue_op const& deq_op = dequeue_op() )
		:
		lst_(),
		idx_( lst_.get< 0 >() ),
		enq_op_( enq_op),
		deq_op_( deq_op)
		{}
	
		void push( item const& itm)
		{ enq_op_( idx_, itm); }
	
		void pop( callable & ca)
		{
			item itm;
			deq_op_( idx_, itm);
			ca.swap( itm.ca);
		}
	
		std::size_t size() const
		{ return lst_.size(); }
	
		bool empty() const
		{ return lst_.empty(); }
	
		void clear()
		{ lst_.clear(); }
	
		const iterator begin()
		{ return lst_.begin(); }
	
		const const_iterator begin() const
		{ return lst_.begin(); }
	
		const iterator end()
		{ return lst_.end(); }
	
		const const_iterator end() const
		{ return lst_.end(); }
	};
};

struct replace_oldest
{
	template<
		typename Index,
		typename Item
	>
	void operator()( Index & idx, Item const& itm)
	{
		typedef typename Index::iterator iterator;
		iterator i( idx.find( itm.attr) );
		if ( i == idx.end() )
			idx.insert( itm);
		else
			idx.replace( i, itm);
	}
};

struct take_oldest
{
	template< typename Item >
	class swapper
	{
	private:
		Item	&	itm_;

	public:
		swapper( Item & itm)
		: itm_( itm)
		{}

		void operator()( Item & itm)
		{ itm_.swap( itm); }
	};

	template<
		typename Index,
		typename Item
	>
	void operator()( Index & idx, Item & itm)
	{
		typedef typename Index::iterator	iterator;
		iterator i( idx.begin() );
		BOOST_ASSERT( i != idx.end() );
		idx.modify( i, swapper< Item >( itm) );
		idx.erase( i);
	}
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_SMART_H


//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_PRIORITY_H
#define BOOST_TASK_PRIORITY_H

#include <cstddef>
#include <utility>

#include <boost/assert.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/task/detail/pool_callable.hpp>
#include <boost/task/detail/meta.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
template<
	typename Attr,
	typename Ord = std::less< Attr >
>
struct priority
{
	typedef detail::has_attribute	attribute_tag_type;
	typedef Attr					attribute_type;

	class impl
	{
	private:
		typedef Attr		attribute;
		typedef Ord			ordering;

	public:
		class item
		{
		private:
			detail::pool_callable	ca_;
			attribute			attr_;
	
		public:
			item(
				detail::pool_callable const& ca,
				attribute const& attr)
			: ca_( ca), attr_( attr)
			{ BOOST_ASSERT( ! ca_.empty() ); }
	
			const detail::pool_callable ca() const
			{ return ca_; }
	
			const attribute attr() const
			{ return attr_; }
		};
	
	private:
		typedef multi_index::multi_index_container<
			item,
			multi_index::indexed_by<
				multi_index::ordered_non_unique<
					multi_index::const_mem_fun<
						item,
						const attribute,
						& item::attr
					>,
					ordering
				>
			>
		>														list;
		typedef typename list::template nth_index< 0 >::type	index;
	
		list		lst_;
		index	&	idx_;
	
	public:
		typedef typename list::iterator			iterator;
		typedef typename list::const_iterator	const_iterator;
	
		impl()
		:
		lst_(),
		idx_( lst_.get< 0 >() )
		{}
	
		void push( item const& itm)
		{ idx_.insert( itm); }
	
		const detail::pool_callable pop()
		{
			iterator i( lst_.begin() );
			BOOST_ASSERT( i != lst_.end() );
			item itm( * i);
			lst_.erase( i);
			return itm.ca();
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
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_PRIORITY_H

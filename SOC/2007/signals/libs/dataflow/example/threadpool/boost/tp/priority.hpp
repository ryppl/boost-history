//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_PRIORITY_H
#define BOOST_TP_PRIORITY_H

#include <cstddef>
#include <utility>

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/future/future.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/tp/detail/info.hpp>
#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp
{
template<
	typename Attr,
	typename Ord = std::less< Attr >
>
struct priority
{
	typedef Attr					attribute_type;
	typedef detail::has_priority	priority_tag_type;

	template< typename Callable >
	class impl
	{
	private:
		typedef Attr		attribute;
		typedef Callable	callable;
		typedef Ord			ordering;

	public:
		class item
		{
		private:
			callable			ca_;
			attribute			attr_;
			detail::interrupter	intr_;
			promise< void >	prom_;
	
		public:
			item(
				callable const& ca,
				attribute const& attr,
				detail::interrupter const& intr)
			: ca_( ca), attr_( attr), intr_( intr), prom_()
			{}
	
			const callable ca() const
			{ return ca_; }
	
			const attribute attr() const
			{ return attr_; }

			const detail::interrupter intr() const
			{ return intr_; }

			promise< void > & prom()
			{ return prom_; }
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
	
		iterator push( item const& itm)
		{
			std::pair< typename index::iterator, bool > p( idx_.insert( itm) );
			BOOST_ASSERT( p.second);
			return p.first;
		}
	
		const item pop()
		{
			iterator i( lst_.begin() );
			BOOST_ASSERT( i != lst_.end() );
			item itm( * i);
			lst_.erase( i);
			itm.prom().set();
			return itm;
		}
	
		bool erase(
			iterator & i,
			future< void > & f)
		{
			if ( f.ready() ) return false;
			item itm( * i);
			itm.prom().set();
			idx_.erase( i);
			BOOST_ASSERT( f.ready() );
			return true;
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

#endif // BOOST_TP_PRIORITY_H

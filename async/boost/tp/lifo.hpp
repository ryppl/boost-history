//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_LIFO_H
#define BOOST_TP_LIFO_H

#include <cstddef>
#include <list>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/tp/detail/info.hpp>
#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp
{
struct lifo
{
	typedef detail::has_no_priority	priority_tag_type;

	template< typename Callable >
	class impl
	{
	private:
		typedef Callable	callable;

	public:
		class item
		{
		private:
			callable			ca_;
			detail::interrupter	intr_;
	
		public:
			item(
				callable const& ca,
				detail::interrupter const& intr)
			: ca_( ca), intr_( intr)
			{ BOOST_ASSERT( ! ca_.empty() ); }

			const callable ca() const
			{ return ca_; }

			const detail::interrupter intr() const
			{ return intr_; }
		};

		typedef typename std::list< item >::iterator		iterator;
		typedef typename std::list< item >::const_iterator	const_iterator;
	
	private:
		std::list< item >	lst_;
	
	public:
		void push( item const& itm)
		{ lst_.push_front( itm); }
	
		const item pop()
		{
			item itm( lst_.front() );
			lst_.pop_front();
			return itm;
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

#endif // BOOST_TP_LIFO_H

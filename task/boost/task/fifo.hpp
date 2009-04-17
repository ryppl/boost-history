//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_FIFO_H
#define BOOST_TASK_FIFO_H

#include <cstddef>
#include <list>

#include <boost/task/detail/callable.hpp>
#include <boost/task/detail/info.hpp>

namespace boost { namespace task
{
struct fifo
{
	typedef detail::has_no_priority	priority_tag_type;

	class impl
	{
	public:
		typedef detail::callable					item;
		typedef std::list< item >::iterator			iterator;
		typedef std::list< item >::const_iterator	const_iterator;
	
	private:
		std::list< item >	lst_;
	
	public:
		void push( item const& itm)
		{ lst_.push_back( itm); }
	
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

#endif // BOOST_TASK_FIFO_H

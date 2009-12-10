
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_BARRIER_H
#define BOOST_FIBERS_BARRIER_H

#include <stdexcept>

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/condition.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/scheduler.hpp>

namespace boost {
namespace fibers {

class barrier : private noncopyable
{
private:
	uint32_t			initial_;
	uint32_t			current_;
	bool				cycle_;
	mutex				mtx_;
	condition			cond_;

public:
	template< typename Strategy >
	barrier( scheduler< Strategy > & sched, uint32_t initial) :
		initial_( initial),
		current_( initial_),
		cycle_( true),
		mtx_( sched),
		cond_( sched)
	{ if ( initial == 0) throw std::invalid_argument("invalid barrier count"); }

	bool wait();
};

}}

#endif // BOOST_FIBERS_BARRIER_H

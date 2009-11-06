
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/scheduler.hpp>

#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <boost/fiber/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

scheduler::data_ptr_t scheduler::data;

fiber::id
scheduler::get_id()
{
	detail::scheduler_data * dt( data.get() );
	BOOST_ASSERT( dt);
	return dt->f_id;
}

void
scheduler::yield()
{
	detail::scheduler_data * dt( data.get() );
	BOOST_ASSERT( dt);
	dt->run_queue.push( dt->f_id);
	dt->fibers[dt->f_id].switch_to( dt->master);
}

void
scheduler::exit()
{
	detail::scheduler_data * dt( data.get() );
	BOOST_ASSERT( dt);
	dt->zombie_queue.push( dt->f_id);
	dt->fibers[dt->f_id].switch_to( dt->master);
}

scheduler::scheduler()
{}

detail::scheduler_data *
scheduler::access_data()
{
	if ( ! data.get() )
	{
		detail::fiber::convert_thread_to_fiber();
		data.reset( new detail::scheduler_data() );
	}
	return data.get();
}

void
scheduler::add( std::auto_ptr< detail::fiber > f)
{
	detail::scheduler_data * dt = access_data();

	detail::fiber::id id( f->get_id() );
	std::pair< ptr_map< detail::fiber::id, detail::fiber >::iterator, bool > result(
		dt->fibers.insert( id, f) );
	if ( ! result.second) throw scheduler_error("inserting fiber failed");
	dt->run_queue.push( result.first->first);
}

bool
scheduler::run()
{
	detail::scheduler_data * dt = access_data();

	if ( ! dt->run_queue.empty() )
	{
		dt->f_id = dt->run_queue.front();
		dt->master.switch_to( dt->fibers[dt->f_id]);
		dt->run_queue.pop();
		return true;
	}
	if ( ! dt->zombie_queue.empty() )
	{
		dt->fibers.erase( dt->zombie_queue.front() );
		dt->zombie_queue.pop();
		return true;
	}
	return false;
}

bool
scheduler::empty()
{ return access_data()->fibers.empty(); }

std::size_t
scheduler::size()
{ return access_data()->fibers.size(); }

}}

#include <boost/config/abi_suffix.hpp>

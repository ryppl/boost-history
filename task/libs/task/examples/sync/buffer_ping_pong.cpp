
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

void send_ping(
		int n,
		tsk::unbounded_buffer< std::string > & buf_ping,
		tsk::unbounded_buffer< std::string > & buf_pong)
{
	for ( int i = 0; i < n; ++i)
	{
		printf("task 1: buf_ping.put(%d-th ping)\n", i);
		buf_ping.put(
				boost::lexical_cast< std::string >( i) + "-th ping");
		boost::optional< std::string > msg;
	   	buf_pong.take( msg);
		BOOST_ASSERT( msg);
		printf("task 1: buf_pong.take(%s)\n", msg->c_str() );
	}
	printf("task 1: end\n");
}

void send_pong(
		int n,
		tsk::unbounded_buffer< std::string > & buf_ping,
		tsk::unbounded_buffer< std::string > & buf_pong)
{
	for ( int i = 0; i < n; ++i)
	{
		printf("task 2: buf_pong.put(%d-th pong)\n", i);
		buf_pong.put(
				boost::lexical_cast< std::string >( i) + "-th pong");
		boost::optional< std::string > msg;
	   	buf_ping.take( msg);
		BOOST_ASSERT( msg);
		printf("task 2: buf_ping.take(%s)\n", msg->c_str() );
	}
	printf("task 2: end\n");
}

void thread_only()
{
	int n = 5;	
	tsk::unbounded_buffer< std::string > buf_ping, buf_pong;
	tsk::handle< void > h1 = tsk::async(
		tsk::make_task(
			& send_ping,
			n,
			buf_ping,
			buf_pong),
		tsk::new_thread() );
	tsk::handle< void > h2 = tsk::async(
		tsk::make_task(
			& send_pong,
			n,
			buf_ping,
			buf_pong),
		tsk::new_thread() );

	tsk::waitfor_all( h1, h2);
}

void pool_only()
{
	tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );

	int n = 5;	
	tsk::unbounded_buffer< std::string > buf_ping, buf_pong;
	tsk::async(
		tsk::make_task(
			& send_ping,
			n,
			buf_ping,
			buf_pong),
		pool);
	tsk::async(
		tsk::make_task(
			& send_pong,
			n,
			buf_ping,
			buf_pong),
		pool);
}

void pool_and_thread()
{
	tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );

	int n = 5;	
	tsk::unbounded_buffer< std::string > buf_ping, buf_pong;
	tsk::async(
		tsk::make_task(
			& send_ping,
			n,
			buf_ping,
			buf_pong),
		pool);
	tsk::handle< void > h = tsk::async(
		tsk::make_task(
			& send_pong,
			n,
			buf_ping,
			buf_pong),
		tsk::new_thread() );

	h.wait();
}

int main( int argc, char *argv[])
{
	try
	{
		pool_only();
		thread_only();
		pool_and_thread();

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}


//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_EXAMPLES_BUFFER_H
#define BOOST_TASK_EXAMPLES_BUFFER_H

#include <cstdlib>
#include <string>

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

inline
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

inline
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

#endif // BOOST_TASK_EXAMPLES_BUFFER_H

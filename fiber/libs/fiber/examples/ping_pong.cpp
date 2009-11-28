#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/assert.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/system/system_error.hpp>

#include <boost/fiber.hpp>

typedef boost::fibers::unbounded_fifo< std::string >	fifo_t;
typedef boost::intrusive_ptr< fifo_t >					fifo_ptr_t;
inline
void ping(
		fifo_ptr_t & recv_buf,
		fifo_ptr_t & send_buf)
{
	boost::optional< std::string > value;

	send_buf->put("ping");
	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();

	send_buf->put("ping");
	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();

	send_buf->put("ping");
	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();

	send_buf->deactivate();
}

inline
void pong(
		fifo_ptr_t & recv_buf,
		fifo_ptr_t & send_buf)
{
	boost::optional< std::string > value;

	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();
	send_buf->put("pong");

	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();
	send_buf->put("pong");

	BOOST_ASSERT( recv_buf->take( value) );
	std::cout << * value << std::endl;
	value.reset();
	send_buf->put("pong");

	send_buf->deactivate();
}

void f( boost::fibers::scheduler<> & sched)
{
	fifo_ptr_t buf1( new fifo_t() );
	fifo_ptr_t buf2( new fifo_t() );
	
	sched.make_fiber( & ping, buf1, buf2);
	sched.make_fiber( & pong, buf2, buf1);
}

int main()
{
	try
	{
		boost::fibers::scheduler<> sched;

		sched.make_fiber( & f, boost::ref( sched) );

		std::cout << "start" << std::endl;

		for (;;)
		{
			while ( sched.run() );
			if ( sched.empty() ) break;
		}

		std::cout << "finish" << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( boost::system::system_error const& e)
	{ std::cerr << "system_error: " << e.code().value() << std::endl; }
	catch ( boost::fibers::scheduler_error const& e)
	{ std::cerr << "scheduler_error: " << e.what() << std::endl; }
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch (...)
	{ std::cerr << "unhandled exception" << std::endl; }
	return EXIT_FAILURE;
}

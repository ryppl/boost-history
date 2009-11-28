#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>

#include <boost/fiber.hpp>

void f( std::string const& str, int n)
{
	for ( int i = 0; i < n; ++i)
	{
		std::ostringstream os1;
		std::ostringstream os2;
		os1	<< boost::this_thread::get_id();
		os2 << boost::this_fiber::get_id();
		fprintf( stderr, "(thread: %s, fiber: %s) %d: %s\n", os1.str().c_str(), os2.str().c_str(), i, str.c_str() );
		boost::this_fiber::yield();
	}
}

void run_thread(
		boost::barrier & b,
		boost::fibers::scheduler<> & sched,
		std::string const& msg, int n)
{
		std::ostringstream os;
		os << boost::this_thread::get_id();
		fprintf( stderr, "start (thread: %s)\n", os.str().c_str() );
		sched.make_fiber( & f, msg, n);

		b.wait();
		for (;;)
		{
			while ( sched.run() );
			if ( sched.empty() ) break;
		}

		fprintf( stderr, "finish (thread: %s)\n", os.str().c_str() );
}

int main()
{
	try
	{
		boost::fibers::scheduler<> sched;

		std::cout << "start" << std::endl;

		boost::barrier b( 2);
		boost::thread th1(
				run_thread,
				boost::ref( b),
				boost::ref( sched), "abc", 3);
		boost::thread th2(
				run_thread,
				boost::ref( b),
				boost::ref( sched), "xyz", 4);

		th1.join();
		th2.join();

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

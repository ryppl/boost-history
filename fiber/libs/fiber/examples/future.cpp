#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>

#include <boost/fiber.hpp>

inline
std::string helloworld_fn()
{ return "Hello World"; }

class callable
{
private:
	struct impl
	{
		virtual ~impl() {}

		virtual void exec() = 0;
	};

	template< typename T >
	class timpl : public impl
	{
	private:
		boost::packaged_task< T >	pt_;

	public:
		timpl( boost::packaged_task< T > & pt) :
			pt_( boost::move( pt) )
		{}

		void exec()
		{ pt_(); }
	};

	boost::shared_ptr< impl >	impl_;

public:
	template< typename T >
	callable( boost::packaged_task< T > & pt) :
		impl_( new timpl< T >( pt) )
	{}

	void operator()()
	{ impl_->exec(); }
};

int main()
{
	try
	{
		boost::fibers::scheduler<> sched;

		boost::packaged_task< std::string > pt( helloworld_fn);
		boost::unique_future< std::string > fu = pt.get_future();
		callable ca( pt);
		sched.make_fiber( ca);

		for (;;)
		{
			while ( sched.run() );
			if ( sched.empty() ) break;
		}

		std::cout << fu.get() << std::endl;

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

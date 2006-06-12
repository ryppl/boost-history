#include <iostream>
#include <boost/test/minimal.hpp>

int test_main(int, char*[])
{
	BOOST_CHECK(1==0);
	return 0;
}


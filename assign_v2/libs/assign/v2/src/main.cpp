
// Example
//#include <iostream>
//#include <libs/assign/v2/example/utility.h>
//#include <libs/assign/v2/example/ref.h>
//#include <libs/assign/v2/example/put.h>
//#include <libs/assign/v2/test/detail.h>

// Test
//#include <libs/assign/v2/test/put.h>
//#include <libs/assign/v2/test/ref.h>
//#include <libs/assign/v2/test/utility.h>

// Speed
#include <fstream>
#include <libs/assign/v2/speed/put_ref.h>

int main (int argc, char * const argv[])
{

    // Do not put libs/assign/v2/test/unit_testing in this project
	{
    	//using namespace test_assign_v2;
		//xxx_detail::test();
		//xxx_utility::test();
		//xxx_put::test();
		//xxx_ref::test();
    }
    {
/*
		using namespace example_assign_v2;

		xxx_ref::run(std::cout);
		xxx_put::run(std::cout);
		xxx_utility::run(std::cout);
*/
    }
	{
    	using namespace speed_assign_v2;
        std::ofstream ofs("results");
		xxx_put_ref::run(ofs);
    }
    return 0;

}

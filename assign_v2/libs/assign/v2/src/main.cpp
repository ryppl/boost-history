#include <string>
#include <iostream>
#include <boost/config.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/format.hpp>
#include <boost/version.hpp>

// Test
#include <libs/assign/v2/test/chain.h>
#include <libs/assign/v2/test/conversion.h>
#include <libs/assign/v2/test/support.h>
#include <libs/assign/v2/test/deque.h>
#include <libs/assign/v2/test/interpreter.h>
#include <libs/assign/v2/test/option.h>
#include <libs/assign/v2/test/put.h>
#include <libs/assign/v2/test/ref.h>

// Tutorial
#include <libs/assign/v2/tutorial.h>

// Speed -- CPP0x required
#include <fstream>
#include <libs/assign/v2/speed/test.h>

int main (int argc, char * const argv[])
{

    // Do not put libs/assign/v2/test/unit_testing in this project
    {
        boost::format f( "boost version %1%.%2% %3% " );
        f
            % ( BOOST_VERSION / 100000 )
                % ( (BOOST_VERSION / 100) % 1000)
                    % ( BOOST_VERSION % 100 );
        std::cout << f.str() << std::endl;
    }
    {
        std::string str = "RVALUE_REFERENCE : %1%\n";
        str += "VARIADIC_TEMPLATES : %2%\n";
        str += "0X_HDR_TUPLE : %3%\n";
        str += "BOOST_ASSIGN_V2_CPP0X : %4%\n";

        boost::format cpp0x(str);

        #ifdef BOOST_NO_RVALUE_REFERENCES
        cpp0x % "No";
        #else
        cpp0x % "Yes";
        #endif
        #ifdef BOOST_NO_VARIADIC_TEMPLATES
        cpp0x % "No";
        #else
        cpp0x % "Yes";
        #endif
        #ifdef BOOST_NO_0X_HDR_TUPLE
        cpp0x % "No";
        #else
        cpp0x % "Yes";
        #endif
        cpp0x % (BOOST_ASSIGN_V2_ENABLE_CPP0X ? "YES" : "NO");

        std::cout << cpp0x.str() << std::endl;
    }
    {
        using namespace test_assign_v2;
        xxx_chain::test();
        xxx_conversion::test();
        xxx_support::test();
        xxx_deque::test();
        xxx_interpreter::test();
        xxx_option::test();
        xxx_put::test();
        xxx_ref::test();
    }
    {
        //tutorial_assign_v2::run();
    }
    {/*
        using namespace speed_assign_v2;
        std::ofstream ofs( "assign_v2_speed_test" );
        speed_assign_v2::test(ofs);
    */}

    return 0;

}

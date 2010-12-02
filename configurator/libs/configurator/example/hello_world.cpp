// Example of simplest usage of Configurator library.
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#include <boost/configurator/configurator.hpp>

#include <iostream>

int main( int argc, char* argv[] ) {	
    try {
        boost::cf::configurator conf; 
        conf.add_option( "Host" );

        conf.load( "/some/path/to/hello_world_conf.txt" ); // See hello_world_conf.txt in 'example'.

        std::string value_of_host = conf.get_value( "Host" );
        std::cout << "Value of host: " << value_of_host << std::endl;
    } catch ( const std::exception& exc ) {
		std::cerr << exc.what() << std::endl;
	} 
	
	return 0;
}

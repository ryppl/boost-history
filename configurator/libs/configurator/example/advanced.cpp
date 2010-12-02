// Example of advanced usage of Configurator library.
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)
//
// IMPORTANT! Example must compile with flags WITH_SINGLETON and WITH_SEMANTIC_CHECK, example:
// # g++ -DWITH_SINGLETON -DWITH_SEMANTIC_CHECK -o test main.cpp -lboost_system -lboost_filesystem -lboost_regex -lboost_thread 

#include <boost/configurator/configurator.hpp>

#include <iostream>

using namespace boost;

void register_options( cf::single_configurator& configurator ) {
    configurator.add_option( "DbName" );
    configurator.add_option( "DbHost" );
    configurator.add_option( "DbPort" );
    configurator.add_option( "DbUser" );
    configurator.add_option( "DbPassword" );

    configurator.in_section( "Server" );
    configurator.add_option_here( "Host" )
                .default_value( "127.0.0.1" )
                .check_semantic( cf::ip )
                ;
    configurator.add_option_here( "Port" )
                .default_value( 80 )
                ;
    configurator.add_option_here( "Administrator" )
                .check_semantic( cf::email );
    configurator.add_option_here( "StorePath" )
                .check_semantic( cf::path );
    configurator.add_option_here( "ReconnectPeriod" )
                .necessary();
    
    configurator.in( "Server" ).in( "Internal" );
    configurator.add_option_here( "Logfile" )
                .check_semantic( cf::optional_path );
    configurator.add_option_here( "MaxLogFileSize" )
                .check_semantic( cf::size )
                .necessary();
    
    configurator.in( "Plugins" ).add_option_here( "plugins" )
                                .allow_multi_values();
}

void view_options_values( cf::single_configurator& configurator ) {
    std::string name;
    std::string host;
    unsigned int port = 0;
    std::string user;
    std::string password;
    configurator.get_value( "DbName", name )
                .get_value( "DbHost", host )
                .get_value( "DbPort", port )
                .get_value( "DbUser", user )
                .get_value( "DbPassword", password )
                ;
    std::cout << "db name: "     << name << std::endl;
    std::cout << "db host: "     << host << std::endl;
    std::cout << "db port: "     << port << std::endl;
    std::cout << "db user: "     << user << std::endl;
    std::cout << "db password: " << password << std::endl;

    configurator.from_section( "Server" );
    std::string serv_host   = configurator.get_value_from_here( "Host" );
    unsigned int serv_port  = configurator.get_value_from_here< unsigned int >( "Port" );
    std::string admin_email = configurator.get_value_from_here( "Administrator" );
    std::string store_path  = configurator.get_value_from_here( "StorePath" );
    size_t period           = configurator.get_value_from_here< size_t >( "ReconnectPeriod" );

    std::cout << "From Server: " << std::endl;
    std::cout << "  serv_host: "    << serv_host << std::endl;
    std::cout << "  serv_port: "    << serv_port << std::endl;
    std::cout << "  admin_email: "  << admin_email << std::endl;
    std::cout << "  store_path: "   << store_path << std::endl;
    std::cout << "  period: "       << period << std::endl;
    
    std::string logfile;
    unsigned long int max_log_file_size = 0;
    configurator.from( "Server" ).from( "Internal" ).get_from_here( "Logfile", logfile )
                                                    .get_from_here( "MaxLogFileSize", max_log_file_size )
                                                    ;
    std::cout << "From Server > Internal: " << std::endl;
    std::cout << "  logfile path: " << logfile << std::endl;
    std::cout << "  max_log_file_size (in bytes): " << max_log_file_size << std::endl;

    std::string plugins_list = configurator.from( "Plugins" )
                                           .get_from_here( "plugins" )
                                           ;
    std::cout << "From Plugins, plugins_list: " << plugins_list << std::endl;
}

int main( int argc, char* argv[] ) {
    try {
        cf::single_configurator& configurator = cf::single_configurator::inst();
        configurator.set_name_value_separator( ':' );
        configurator.set_case_sensitivity_for_names();

        register_options( configurator );
        
        configurator.parse( "/some/path/to/advanced_conf.txt" ); // See advanced_conf.txt in 'example'.
        
        view_options_values( configurator );
    } catch ( const std::exception& exc ) {
		std::cerr << exc.what() << std::endl;
	}

    return 0;
}

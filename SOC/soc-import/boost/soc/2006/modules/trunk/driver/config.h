#ifndef INCLUDE_CONF_H
#define INCLUDE_CONF_H

#include <string>
// #include <iostream>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpp_context.hpp>
#include <boost/wave/language_support.hpp>

struct config {
	static const std::map<std::string, std::string>& macros ();
	static const std::vector<std::string>&  system_include_paths ();
};

template<class Context>
void configure_context (Context& c) {
	typedef std::vector<std::string>::const_iterator  vec_iter_t;
	typedef std::map<std::string, std::string>::const_iterator map_iter_t;
	
	using namespace boost;
	using namespace boost::wave;
	
	c.set_language (enable_long_long(c.get_language()));
	c.set_language (enable_variadics(c.get_language()));
	c.set_language (language_support (support_option_convert_trigraphs 
	                                  | c.get_language()));
	c.set_language (language_support (support_option_no_character_validation 
	                                  | c.get_language()));
	
	// add in all the system include paths.
	for (vec_iter_t it = config::system_include_paths().begin();
	     it != config::system_include_paths().end();
	     ++it) {
	     c.add_sysinclude_path(it->c_str());
	}
	
	// and now the macros.
	for (map_iter_t it = config::macros().begin();
	     it != config::macros().end();
	     ++it) {
// 	    try {
			if (it->second.length ())
				c.add_macro_definition( str(boost::format("%s=%s") 
										 % it->first % it->second), 
										 true);
			else
				c.add_macro_definition( it->first, true );
// 		} catch (...) {
// 			std::cout << "Failed adding macro definition " 
// 			          << it->first << "=" << it->second << std::endl;
// 		}
	}
}


#endif

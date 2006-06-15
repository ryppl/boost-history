#ifndef INCLUDE_CONF_H
#define INCLUDE_CONF_H

#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>

struct config {
	static const std::map<std::string, std::string>& macros ();
	static const std::vector<std::string>&  system_include_paths ();
};

template<class Context>
void configure_context (Context& c) {
	typedef std::vector<std::string>::const_iterator  vec_iter_t;
	typedef std::map<std::string, std::string>::const_iterator map_iter_t;
	
	using namespace boost;
	
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
	 	if (it->second.length ())
	 		c.add_macro_definition( str(boost::format("%s=%s") 
	 		                         % it->first % it->second), 
	 		                         true);
	 	else
	 		c.add_macro_definition( it->first, true );
	}
}


#endif

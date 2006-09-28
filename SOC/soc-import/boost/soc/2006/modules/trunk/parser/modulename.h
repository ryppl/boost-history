/** @file modulename.h
	\brief ModuleName handles assembly, comparison, etc of module names.
	
	
*/

#ifndef INCLUDE_MODULENAME_H
#define INCLUDE_MODULENAME_H

#include <vector>
#include <string>
#include "lexpolicies.h"

class ModuleName {
	std::vector<std::string> m_name_segments;
	std::string m_partition;
	mutable std::string m_canonical;
	mutable std::string m_identifier;
	void invalidate () const;
public:
	ModuleName (std::string to_parse);
	ModuleName ();
	
	void add_segment (const std::string& s) {
		invalidate (); 
		m_name_segments.push_back(s); 
	}
	
	void add_segment (const token_t& tok) {
		invalidate ();
		add_segment (std::string(tok.get_value().begin(), tok.get_value().end()));
	}
	
	void set_partition (const std::string& s) { 
		invalidate ();
		m_partition = s; 
	}
	
	void set_partition (const token_t& tok) {
		invalidate ();
		set_partition (std::string(tok.get_value().begin(), tok.get_value().end()));
	}
	
	int depth () const { return m_name_segments.size (); }
	std::string element(int i) const{ return m_name_segments[i]; }
	std::string partition () const { return m_partition; }
	
	bool same_module (const ModuleName& other);
	/// this gives the format Foo_Bar_Baz__blah for Foo::Bar::Baz["blah"]
	const std::string as_identifier () const;
	/// this gives the format 'Foo::Bar::Baz["blah"]'.
	const std::string canonical () const;
};

#endif

#define BOOST_SPIRIT_DEBUG

#include "map.h"

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/actor.hpp>
#include <boost/wave.hpp>
#include <boost/wave/util/pattern_parser.hpp>


using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace boost::wave;
using namespace boost::filesystem;

/// A single mapfile.
class Map {
	
	// this'll get extended to vector<path> in phase II, when we
	// do module partitions.
	typedef map<string, list<path> >  vecmap_t;
	
	vecmap_t m_map;
	const path m_path;
	boost::filesystem::fstream m_file;
	bool m_save;
	string m_curmodule;
	
public:
	Map (const path & mapfile, bool create = false);
	~Map ();
	void add (const string & module_name, const path & filename) {
		m_map[module_name].push_back(filename);
	}
	
	void set_modname (const string& module) {
		m_curmodule = module;
	}
	
	void add_filename (const string& p) {
		m_map[m_curmodule].push_back (path(p));
	}
	
	list<path> lookup (const string& module_name) { 
		return m_map[module_name];
	}
};

#pragma mark -

//
// Some actions operating on Map
//

struct setmodule_action {
	void act (Map& ref, const string value) const {
		ref.set_modname(value);
	}
	template<class Iter>
	void act (Map& ref, const Iter& f, const Iter& e) const {
		ref.set_modname(string(f,e));
	}
};

inline ref_value_actor<Map, setmodule_action>
setmodule (Map& ref) {
	return ref_value_actor<Map, setmodule_action>(ref);
}

struct addfile_action {
	void act (Map& ref, const string value) const {
		ref.add_filename (value);
	}

	template<class Iter>
	void act (Map& ref, const Iter& f, const Iter& e) const {
		ref.add_filename (string(f,e));
	}
};

inline ref_value_actor<Map, addfile_action>
addfile (Map& ref) {
	return ref_value_actor<Map, addfile_action>(ref);
}

#pragma mark -
//
// MapManager Implementation.
// 

MapManager::
MapManager (const path& origin) {
	// kill the file extension of origin's name, and pop in .map
	string filename = origin.leaf ();
	string::size_type last_p = filename.find_last_of('.');
	if (last_p > 0)
		filename.resize(last_p);
	filename.append (".map");
	m_localmap.reset(new Map (origin.branch_path () / filename, true));
}

void
MapManager::
add (const path& path) {
	// scan path for .map files, add them in.
	cout << "[mm] scanning " << path << endl;
	directory_iterator end, it(path);
	for (; it != end; ++it) {
		if (ends_with(it->leaf(),".map") > 0) {
			try {
				cout << "  found " << *it << endl;
				m_maps.push_back(shared_ptr<Map>(new Map(*it)));
			} catch (std::exception & e) {
				cout << "failed to load mapfile " << *it << ":" 
				   << e.what () << endl;
			}
		}
	}
}

list<path>
MapManager::
lookup (const string& module_name) {
	list<path> p;
	typedef std::list<boost::shared_ptr<Map> > map_t;
	if ( (p = m_localmap->lookup (module_name)).size ())
		return p;
	for (map_t::reverse_iterator it = m_maps.rbegin ();
	     it != m_maps.rend ();
	     ++it) {
		if ( (p= (*it)->lookup(module_name)).size ())
			return p;
	}
	return p;
}

void
MapManager::
put (const string& module_name,
     const path& filename) {
	m_localmap->add (module_name, filename);
}

#pragma mark -
//
// Map implementation
//

// a grammar for the mapfile.
struct mapfile_grammar : public grammar<mapfile_grammar> {
	mutable Map & m;	

	mapfile_grammar (Map &map) : m(map) {}
	
	
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  module, module_name, name_element, partition_name,
		                quoted_string, module_decl, filename, mapfile;

		definition (const mapfile_grammar & s) {
			quoted_string 
			  =  lexeme_d
			     [ 
			        ch_p('"')
			        >> (~ch_p('"'))
			        >> ch_p('"') 
			     ]
			  ;
			  
			module = str_p("module");
			
			name_element 
			  = alpha_p 
			    >> *(alnum_p | ch_p('_'))
			  ;
			  
			partition_name 
			  = ch_p('[') 
			    >> quoted_string
			    >> ']'
			  ;
			  
			module_name 
			  = name_element 
			    >> *(str_p("::") >> name_element)
//			    >> !partition_name
			  ;
			            
			filename = *(alnum_p | ch_p('_') | '.' | '-');
			
			module_decl 
			  = str_p("module")
			    >> module_name [setmodule(s.m)]
			    >> ":" 
			    >> !filename [addfile(s.m)]
			    >> *(blank_p >> filename [addfile(s.m)] )
			    >> ch_p(';')
			  ;
			  
			mapfile 
			  = *module_decl >> end_p
			  ;
			
			BOOST_SPIRIT_DEBUG_RULE(mapfile);
			BOOST_SPIRIT_DEBUG_RULE(module_decl);
			BOOST_SPIRIT_DEBUG_RULE(filename);
			BOOST_SPIRIT_DEBUG_RULE(module_name);
			BOOST_SPIRIT_DEBUG_RULE(name_element);
		}
		rule<ScannerT> const& start () { return mapfile; }
	};
};

Map::
Map (const path & mapfile, bool create)	
  :m_path (mapfile),  m_file (mapfile) {
	
	if (!exists(mapfile) && !create) {
		cout << "[map] file doesn't exist: "<< mapfile << endl;
		return;
		// I should probably throw something here. Eh.
	}
	
	if (create)
		m_save = true;
	
	m_file.unsetf(ios::skipws);
	string instring ((istreambuf_iterator<char>(m_file.rdbuf ())),
	                 (istreambuf_iterator<char>()));
	
	mapfile_grammar g(*this);
	if (parse(instring.begin (), instring.end (), g, space_p).full) {
		cout << "[map] done parsing " << mapfile << endl;
		// if it's really done, let's see what's inside.
		for (vecmap_t::iterator it = m_map.begin ();
		     it != m_map.end ();
		     ++it) {
			cout << " " << it->first << ": ";
			for (list<path>::iterator i = it->second.begin ();
			     i != it->second.end ();
			     ++i) {
				cout << *i << " ";    
			}
			cout << endl;
		}
	} else {
		cout << "! [map] failed parsing " << mapfile << endl;
	}

}

Map::
~Map () {
	if (m_save) {
		// we rewrite the local map every time.
		// eh, not good, but enough for now
		// reopen the file in truncate mode.
		m_file.close ();
		m_file.open (m_path, ios::trunc | ios::out);
		for (vecmap_t::iterator it = m_map.begin ();
		     it != m_map.end ();
		     ++it) {
			string buffer ("module ");
			buffer.append (it->first);
			for (list<path>::iterator pi = it->second.begin ();
			     pi != it->second.end ();
			     ++pi) {
				buffer.append (pi->native_file_string ());
				buffer.append (" ");
			}
			buffer.append (";\n");
			m_file << buffer;
		}
		m_file.close ();
	}
}

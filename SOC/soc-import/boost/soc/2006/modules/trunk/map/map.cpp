#include "map.h"

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/actor.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace boost::wave;
using namespace boost::filesystem;

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
	directory_iterator end, it(path);
	boost::regex dotmap("^.*\\.map$");
	for (; it != end; ++it) {
//		if (it->leaf().find_last_of(".map") > 0) {
		if (boost:regex_match (it->leaf(), reg)) {
			try {
				m_maps.push_back(shared_ptr<Map>(new Map(*it)));
			} catch (std::exception & e) {
				cout << "failed to load mapfile " << *it << ":" 
				   << e.what () << endl;
			}
		}
	}
}

// a grammar for the mapfile.
struct mapfile_grammar : public grammar<mapfile_grammar> {
	vector<char>  m_tmp_str;
	
	map<string, vector<string> > m_assoc;
	map<string, vector<string> >::value_type  element, blank;
	
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  module, module_name, name_element, partition_name,
		                quoted_string, module_decl, filename, mapfile;

		definition (mapfile_grammar & s) {
			quoted_string 
			  =  lexeme_d[ ch_p('"')[clear_a(s.m_tmp_str)] 
			  >> (~ch_p('"'))[push_back_a(s.m_tmp_str)] 
			  >> ch_p('"') ];
			  
			module = str_p("module");
			
			name_element = alpha_p >> *(alnum_p | ch_p('_'));
			
			partition_name 
			  = ch_p("[") 
			  >> quoted_string[assign_a(s.element.first, s.m_tmp_str)]
			  >> "]";
			  
			module_name = name_element >> *(str_p("::") >> name_element)
			            >> !partition_name;
			            
			filename = lexeme_d[ *(~ch_p(' ')) ];
			module_decl 
			  = module[assign_a(s.element,blank)]
			  >> module_name[assign_a(s.element.first, s.m_tmp_str)] 
			  >> ":" >> *filename[push_back_a(s.element.second)] 
			  >> ch_p(';')[insert_at_a(s.m_assoc, s.element)];
			  
			mapfile = *module_decl;
		}
		rule<ScannerT> const& start () { return mapfile; }
	};
};


// (create) is currently ignored.
Map::
Map (const path & mapfile, bool create = false)	: m_file (mapfile) {
	// setup a wave context
	// run the grammar on it the skip_parser from the wave example should
	// be fine here.
}

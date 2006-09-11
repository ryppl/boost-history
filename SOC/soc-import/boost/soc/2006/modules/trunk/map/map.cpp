#include "setup.h"
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
#include <map>
#include <set>

using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace boost::wave;
using namespace boost::filesystem;

/// A single mapfile.
class Map {
    
    typedef map<string, set<path> >  vecmap_t;
    
    vecmap_t m_map;
    const path m_path;
    boost::filesystem::fstream m_file;
    bool m_save;
    string m_curmodule;
    
public:
    Map (const path & mapfile, bool create = false);
    ~Map ();
    
    string name() {
        return m_path.native_file_string ();
    }
    
    void add (const string & module_name, const path & filename) {
        m_map[module_name].insert(filename);
    }
    
    void set_modname (const string& module) {
        m_curmodule = module;
    }
    
    void add_filename (const string& p) {
        m_map[m_curmodule].insert (path(p));
    }
    
    set<path> lookup (const string& module_name) { 
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
    directory_iterator end, it(path);
    for (; it != end; ++it) {
        if (ends_with(it->leaf(),".map")) {
            try {
//              cerr << "===== Loading " << *it << " =====" << endl;
                m_maps.push_back(shared_ptr<Map>(new Map(*it)));
            } catch (std::exception & e) {
                cout << "failed to load mapfile " << *it << ":" 
                   << e.what () << endl;
            }
        }
    }
}

set<path>
MapManager::
lookup (const string& module_name) {
    set<path> p;
    typedef std::list<boost::shared_ptr<Map> > map_t;
//  cout << "[MM::lookup] " << module_name <<": ";
    if ( (p = m_localmap->lookup (module_name)).size ()) {
//      cout << "locally found " << p.size () << endl;
        return p;
    }
//  cout << endl;
    for (map_t::reverse_iterator it = m_maps.rbegin ();
         it != m_maps.rend ();
         ++it) {
//      cout <<"  trying " << (*it)->name() << ": ";
        if ( (p= (*it)->lookup(module_name)).size ()) {
//          cout << "found " << p.size () << endl;
            return p;
        } else {
//          cout << "none found" << endl;
        }
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
                    >> *(~ch_p('"'))
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
              = ( name_element % str_p("::") )
                >> !partition_name
              ;
                        
            filename 
              = lexeme_d [+(alnum_p | ch_p('_') | ch_p('.')) ]
              ;
            
            module_decl 
              = str_p("module")
                >> module_name [setmodule(s.m)]
                >> ch_p(':')
                >> *filename [addfile(s.m)]
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
  :m_path (mapfile),  m_file (mapfile, ios_base::in), m_save(create) {
  
//  cout << "Map::Map(" << mapfile <<", " << (create? "true)":"false)") <<  endl;
    
    if (!exists(mapfile) && !create) {
        cout << "[map] file doesn't exist: "<< mapfile << endl;
        return;
        // I should probably throw something here. Eh.
    }
    
    m_file.unsetf(ios::skipws);
    string instring ((istreambuf_iterator<char>(m_file.rdbuf ())),
                     (istreambuf_iterator<char>()));
    
    mapfile_grammar g(*this);
    if (parse(instring.begin (), instring.end (), g, space_p).full) {
//      cout << "[map] done parsing " << mapfile << endl;
        // if it's really done, let's see what's inside.
//      for (vecmap_t::iterator it = m_map.begin ();
//           it != m_map.end ();
//           ++it) {
//          cout << " " << it->first << ": ";
//          for (set<path>::iterator i = it->second.begin ();
//               i != it->second.end ();
//               ++i) {
//              cout << *i << " ";    
//          }
//          cout << endl;
//      }
    } else {
        cout << "! [map] failed parsing " << mapfile << endl;
    }

}

Map::
~Map () {
    if (m_save) {
//      cerr << "[Map::saving " << m_path << "]";
        // we rewrite the local map every time.
        // eh, not good, but enough for now
        // reopen the file in truncate mode.
        m_file.close ();
        m_file.open (m_path, ios::trunc | ios::out);
        for (vecmap_t::iterator it = m_map.begin ();
             it != m_map.end ();
             ++it) {
            if (it->second.size ()) {
                string buffer ("module ");
                buffer.append (it->first);
                buffer.append (": ");
                for (set<path>::iterator pi = it->second.begin ();
                     pi != it->second.end ();
                     ++pi) {
                    buffer.append (pi->native_file_string ());
                    buffer.append (" ");
                }
                buffer.append (";\n");
                m_file << buffer;
            }
        }
        m_file.close ();
    }
}

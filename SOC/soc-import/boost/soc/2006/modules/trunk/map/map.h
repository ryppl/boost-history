/// A single map file.
#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem;
using namespace std;

/// A single mapfile.
class Map {
	
	// this'll get extended to vector<path> in phase II, when we
	// do module partitions.
	typedef map<string, list<path> >  vecmap_t;
	
	vecmap_t m_map;
	const path m_path;
	boost::filesystem::fstream m_file;
	
public:
	Map (const path & mapfile, bool create = false);
	~Map ();
	
	void add (const string & module_name, const path & filename);
	list<path> lookup (const string& module_name) { 
			return m_map[module_name];
		}
};


/// Keeps track of all available mapfiles and their
/// relative mappings.
class MapManager {
	/// Stored in order of decreasing precedence.
	std::list<boost::shared_ptr<Map> >  m_maps;
	
	std::auto_ptr<Map> m_localmap; 
public:
	/// origin's the file we're compiling now.
	MapManager (const path& origin);
	
	// adds the directory, and scans for any mapfiles within.
	void  add (const path& path);
	
	path lookup (const string& module_name);
	Map * localMap () { return m_localmap.get (); }
};

#endif

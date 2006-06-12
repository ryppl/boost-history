/// A single map file.
#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

/// A single mapfile.
class Map {
	using namespace boost::filesystem;
	
	// this'll get extended to vector<path> in phase II, when we
	// do module partitions.
	typedef map<string, path >  vecmap_t;
	
	vecmap_t m_map;
	const path mapfile;
	
public:
	Map (const path & mapfile, bool create = false);
	~Map ();
	
	// 
	void add (const string & module_name, const path & filename);
	path lookup (const string& module_name);

};

#endif

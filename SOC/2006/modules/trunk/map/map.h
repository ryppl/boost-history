/// A single map file.
#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <list>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

class Map;

/// Keeps track of all available mapfiles and their
/// relative mappings.
class MapManager {
    /// Stored in order of decreasing precedence.
    std::list<boost::shared_ptr<Map> >  m_maps;
    
    boost::shared_ptr<Map> m_localmap; 
public:
    /// origin's the file we're compiling now.
    MapManager (const boost::filesystem::path& origin);
    
    /// adds the directory, and scans for any mapfiles within.
    void add (const boost::filesystem::path& path);
    
    std::set<boost::filesystem::path> lookup (const std::string& module_name);
    void put (const std::string& module_name, 
              const boost::filesystem::path& filename);
};

#endif

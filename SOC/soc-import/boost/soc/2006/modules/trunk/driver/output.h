/** @file output.h
    \brief OutputDelegate, for managing a single output stream
    
    The OutputDelegate manages the final emission of text to an output stream.
*/

#ifndef INCLUDE_OUTPUT_H
#define INCLUDE_OUTPUT_H

#include "../parser/lexpolicies.h"

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stack>
#include <boost/shared_ptr.hpp>

#include "../map/map.h"

class OutputDelegate {
    std::ostream&  m_stream;
    MapManager   * m_mmgr;
    bool           m_emitted;
    std::string    m_filename;

    std::vector<std::string>  m_includes;
    std::vector<std::string>  m_text;
    void check ();
    
public:
    OutputDelegate (const std::string& filename, std::ostream& s, MapManager *m);
    ~OutputDelegate ();
    
    void text (const std::string& s);
    void include (const std::string& module_name);
    
    void emit ();
};


#endif

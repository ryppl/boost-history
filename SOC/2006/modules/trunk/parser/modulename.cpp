#include "modulename.h"
#include <boost/format.hpp>
using namespace boost;
using namespace std;

// class ModuleName {
//     std::vector<std::string> m_name_segments;
//     std::string m_partition;
// public:
//     ModuleName (std::string to_parse);
//     ModuleName ();
//     
//     void add_segment (const std::string& s) { m_name_segments.push_back(s); }
//     void set_parititon (const std::string& s) { m_partition = s; }
//     
//     bool same_module (const ModuleName& other);
//     std::string normalized ();
// };

ModuleName::
ModuleName () {}

void
ModuleName::
invalidate () const {
    m_identifier.clear ();
    m_canonical.clear ();
}

bool
ModuleName::
same_module (const ModuleName& other) {
    // it's the same if the name segments are all true.
    if (other.m_name_segments.size () != m_name_segments.size ())
        return false;
        
    for (size_t i=0; i<m_name_segments.size (); i++)
        if (m_name_segments[i] != other.m_name_segments[i])
            return false;
    
    return true;
}

const std::string
ModuleName::
as_identifier () const {
    // simple method: a single underscore between module name segments,
    // two underscores between that and the partition name, which
    // has underscores for every non-alphanumeric char.
    if (m_identifier.length ()) {
        return m_identifier;
    }
    
    std::string result;
    for (size_t i =0; i<m_name_segments.size (); i++) {
        if (i)
            result.append ("_");
        result.append (m_name_segments[i]);
    }
    
    if (m_partition.length ()) {
        std::string n;
        for (string::const_iterator it = m_partition.begin ();
             it != m_partition.end ();
             ++it) {
            if (isalnum(*it)) 
                n.append(1,*it);
            else
                n.append(1,'_');
        }
        result.append ("__");
        result.append (n);
       }
       
    m_identifier = result;
    return result;
}

const std::string 
ModuleName::
canonical () const {
    if (m_canonical.length ())
        return m_canonical;
        
    std::string result;
    for (size_t i =0; i<m_name_segments.size (); i++) {
        if (i)
            result.append ("::");
        result.append (m_name_segments[i]);
    }
    
    if (m_partition.length ()){
        std::string p = m_partition;
        result.append ((format ("[%s]") % p).str ());
       }
    
    m_canonical = result;
    return result;
}

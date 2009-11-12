#ifndef BOOST_CGI_FORM_PART_HPP_INCLUDED__
#define BOOST_CGI_FORM_PART_HPP_INCLUDED__

#include <map>
#include <vector>
#include <string>
#include <boost/range.hpp>
#include "boost/cgi/config.hpp"

BOOST_CGI_NAMESPACE_BEGIN
 namespace common {

   struct form_part
   {
     typedef std::string       string_type;
     //typedef std::vector<char> buffer_type;
     typedef string_type buffer_type;
     typedef buffer_type::iterator iter_t;
     
     typedef boost::iterator_range<
       buffer_type::const_iterator
     > range_type;

     typedef std::pair<iter_t, iter_t> pair_t;
     
     typedef std::map<
       string_type, pair_t
     > meta_data_map_type;
   
     form_part()
     {
     }

     bool operator==(form_part& other) {
        return this->name == other.name;
     }
   
     meta_data_map_type meta_data_;

     /// The boundary marker that's needed.
     // If this is empty, it means the corresponding data has been read.
     string_type boundary_marker_;

     //range_type buffer_;
     pair_t buffer_;

     string_type content_type; // must exist
     string_type content_disposition; // must exist
     string_type name; // must exist
     string_type filename;
     // Where the actual uploaded file is stored.
     string_type path;
     
     // Using a simple map while everything is changing. This will not copy the
     // values when it is properly implemented (it'll hold a pair of iterators 
     // to the data).
     //std::map<string_type, string_type> meta_data_;
     
     // Boolean to show if the form part has been completely read/parsed
     //bool finished_;
   };

 } // namespace common
BOOST_CGI_NAMESPACE_END

#endif // BOOST_CGI_FORM_PART_HPP_INCLUDED__


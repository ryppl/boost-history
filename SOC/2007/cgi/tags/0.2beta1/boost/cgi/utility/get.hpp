
#ifndef BOOST_CGI_GET_HPP_INCLUDED_
#define BOOST_CGI_GET_HPP_INCLUDED_

#include <map>
///////////////////////////////////////////////////////////
#include "boost/cgi/common/name.hpp"
#include "boost/cgi/utility/has_key.hpp"

namespace cgi {
 namespace common {

   /// Get a value from the map, or return the passed value as a default.
   /**
    * Examples:
    */
    template<typename MapT>
    std::string
      get_value(MapT& data, common::name const& name, std::string const& default_value)
    {
      return has_key(data, name) ? data["name"] : default_value;
    }

 } // namespace common
} // namespace cgi

#endif // BOOST_CGI_GET_HPP_INCLUDED_


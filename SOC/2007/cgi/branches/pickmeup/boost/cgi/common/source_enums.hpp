
#ifndef BOOST_CGI_COMMON_SOURCE_ENUMS_HPP_INCLUDED__
#define BOOST_CGI_COMMON_SOURCE_ENUMS_HPP_INCLUDED__

#include <boost/fusion/include/at.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/common/map.hpp"

namespace cgi {
 namespace common {

   /// Types of data sources and related enums.
   /**
    * Each data source is given a unique type. This is so the accessor
    * operators on a `basic_request<>` are compile-time aliases to the
    * internal data maps.
    *
    * If you want to add a new data type to a request you need to:
    *   > Update this file
    *   > Update map.hpp with a new map type
    *   > Update `data_map_type` in the `request_base` class
    *   > Use the `BOOST_CGI_DETAIL_MAP_ACCESS` macro in `basic_request<>`,
    *     next to the other uses of it.
    */
   enum env_data_type     { env,        env_data       };
   enum get_data_type     { get,        get_data       };
   enum post_data_type    { post,       post_data      };
   enum cookie_data_type  { cookies,    cookie_data    };
   enum form_data_type    { form,       form_data      };
   enum session_data_type { session,    session_data   };

   template<typename Impl>
     env_map&
       env_vars(Impl& impl)     { return boost::fusion::at_c<0>(impl); }
   template<typename Impl>
     get_map&
       get_vars(Impl& impl)     { return boost::fusion::at_c<1>(impl); }
   template<typename Impl>
     post_map&
       post_vars(Impl& impl)    { return boost::fusion::at_c<2>(impl); }
   template<typename Impl>
     cookie_map&
       cookie_vars(Impl& impl)  { return boost::fusion::at_c<3>(impl); }
   template<typename Impl>
     session_map&
       session_vars(Impl& impl) { return boost::fusion::at_c<4>(impl); }

/// This is used in `class basic_request<>` to add the request data accessors operators.
#define BOOST_CGI_DETAIL_MAP_ACCESS(x) \
   x##_map& operator[](common::##x##_data_type const&) \
   { \
     return x##_vars(this->implementation.vars_); \
   }

 } // namespace common
} // namespace cgi

#endif // BOOST_CGI_COMMON_SOURCE_ENUMS_HPP_INCLUDED__


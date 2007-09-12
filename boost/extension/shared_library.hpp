/*
 * Boost.Extension / shared_library:
 *         functions for shared_library loading
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_SHARED_LIBRARY_HPP
#define BOOST_EXTENSION_SHARED_LIBRARY_HPP
#include <string>
#include <boost/extension/impl/library_impl.hpp>
#include <boost/extension/common.hpp>
namespace boost { namespace extensions {

#define BOOST_EXTENSION_SHARED_LIBRARY_GET_FUNCTION(Z, N, _) \
template <class ReturnValue BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param) > \
ReturnValue (*get(const char * name))(BOOST_PP_ENUM_PARAMS(N, Param)) { \
  return reinterpret_cast<ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
      (get_function(handle_, name)); \
}


class shared_library
{
public:
  bool is_open(){return handle_ != 0;}
  bool open() {
    return (handle_ = load_shared_library(location_.c_str())) != 0;
  }
  bool close() {
    return close_shared_library(handle_);
  }  
  shared_library(const char * location, bool auto_close = false)
  :location_(location), handle_(0), auto_close_(auto_close) {
  }

BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_EXTENSION_SHARED_LIBRARY_GET_FUNCTION, _)

protected:
  std::string location_;
  library_handle handle_;
  bool auto_close_;
};
}}
#endif  // BOOST_EXTENSION_SHARED_LIBRARY_HPP





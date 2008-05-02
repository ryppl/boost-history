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
#include <boost/preprocessor/iteration/iterate.hpp>

namespace boost { namespace extensions {

class shared_library
{
public:
  bool is_open() { return handle_ != 0; }
  bool open() {
    return (handle_ = impl::load_shared_library(location_.c_str())) != 0;
  }
  bool close() {
    return impl::close_shared_library(handle_);
  }
  shared_library(const std::string& location, bool auto_close = false)
    : location_(location), handle_(0), auto_close_(auto_close) {
  }
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/extension/impl/shared_library.hpp>
#include BOOST_PP_ITERATE()
protected:
  std::string location_;
  impl::library_handle handle_;
  bool auto_close_;
};
}}
#endif  // BOOST_EXTENSION_SHARED_LIBRARY_HPP

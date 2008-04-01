//             -- basic_sync_io_object.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_SYNC_IO_OBJECT_HPP_INCLUDED__
#define CGI_BASIC_SYNC_IO_OBJECT_HPP_INCLUDED__

#include <boost/noncopyable.hpp>

namespace cgi {
 namespace common {

  /// basic_io_object alternative when an io_service isn't used
  template<typename Service>
  class basic_sync_io_object
    : private boost::noncopyable
  {
  public:
    typedef Service                                  service_type;
    typedef typename Service::implementation_type    implementation_type;

  protected:
    explicit basic_sync_io_object()
    {
      service.construct(implementation);
    }

    ~basic_sync_io_object()
    {
      service.destroy(implementation);
    }

    service_type service;
    implementation_type implementation;
  };

 } // namespace common
} // namespace cgi

#endif // CGI_BASIC_SYNC_IO_OBJECT_HPP_INCLUDED__

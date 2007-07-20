//                  -- cgi_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_SERVICE_HPP_INCLUDED__
#define CGI_CGI_SERVICE_HPP_INCLUDED__

#include "../tags.hpp"
#include "../connections/stdio.hpp"
#include "../connections/async_stdio.hpp"
#include "../gateway_impl/acgi_gateway_impl.hpp"
#include "../gateway_service/acgi_gateway_service.hpp"
#include "../basic_protocol_service.hpp"

namespace cgi {

  /// typedef for typical usage
  /**
   * Works with both cgi_request and acgi_request.
   */
  typedef basic_protocol_service<tags::acgi> cgi_service;

  /// A service 'owned' by a single user-supplied io_service
  typedef basic_protocol_service<tags::acgi, 0> cgi_sub_service;

  /// A service with a pool of io_services underneath
  template<int IoServiceCount, typename PoolingPolicy = tags::round_robin>
  struct cgi_service_pool
    : public basic_protocol_service<tags::acgi, IoServiceCount, PoolingPolicy>
  {
    cgi_service_pool(int concurrency_hint = 0)
      : basic_protocol_service<tags::acgi, IoServiceCount, PoolingPolicy>
          (concurrency_hint)
    {
    }
  };

} // namespace cgi

#endif // CGI_CGI_SERVICE_HPP_INCLUDED__

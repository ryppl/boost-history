//               -- acgi_service_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ACGI_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_ACGI_SERVICE_IMPL_HPP_INCLUDED__

#include <boost/asio/io_service.hpp>
#include "../tags.hpp"
#include "../io_service.hpp"
#include "../map.hpp"
#include "../request_impl/acgi_request_impl.hpp"
#include "cgi_service_impl_base.hpp"
#include "../detail/service_base.hpp"
#include "../io_service.hpp"
#include "../detail/extract_params.hpp"

namespace cgi {

  //template<typename ProtocolService>
  class acgi_service_impl
  //: public boost::asio::io_service::service
    : public cgi_service_impl_base<acgi_request_impl>
    , public detail::service_base<acgi_service_impl>
  {
  public:
    typedef acgi_service_impl           type;
    typedef acgi_request_impl           impl_type;
    typedef cgi::map                    map_type;
    typedef tags::acgi                  protocol_type;
    typedef cgi_service                 protocol_service_type;

    /// The unique service identifier
    //    static boost::asio::io_service::id id;

    acgi_service_impl(cgi::io_service& ios)
      : detail::service_base<acgi_service_impl>(ios)
    {
    }

    acgi_service_impl(protocol_service_type& protocol_service)
    //: boost::asio::io_service::service(protocol_service.io_service())
      : detail::service_base<acgi_service_impl>(protocol_service.io_service())
    //  , cgi_service_impl_base<acgi_request_impl>()
    {
    }

    void shutdown_service()
    {
    }

    void construct(implementation_type& impl)
    {
    }

    void destroy(implementation_type& impl)
    {
    }
  };

} // namespace cgi

#endif // CGI_ASYNC_CGI_SERVICE_IMPL_HPP_INCLUDED__

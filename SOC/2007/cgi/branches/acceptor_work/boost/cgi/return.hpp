//                     -- return.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_RETURN_HPP_INCLUDED__
#define CGI_RETURN_HPP_INCLUDED__

#include "boost/cgi/response.hpp"
#include "boost/cgi/basic_request.hpp"

namespace cgi {
 namespace common {

   template<typename Response, typename Request>
   int return_helper(Response& resp, Request& req, int program_status)
   {
     boost::system::error_code ec;
     resp.send(req, ec);
     if (ec) return ec;

     req.close(resp.status(), program_status);

     return program_status;
   }

   template<typename R1, typename R2>
   void send_helper(R1& resp, R2& req)
   {
     resp.send(req.client());
   }

   void close_helper(response& resp, boost::fcgi::request& req, int prog_status)
   {
     req.close(resp.status(), prog_status);
   }

#define return_(RESP, REQ, STATUS) \
    send_helper(RESP, REQ);            \
    ::cgi::common::close_helper(RESP, REQ, STATUS); \
    return STATUS;

//return return_helper(resp, req, status)

 } // namespace common
} // namespace cgi

#endif // CGI_RETURN_HPP_INCLUDED__

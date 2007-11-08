//                   -- basic-1.cpp --
//
//            Copyright (c) Darren Garvey 2007
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////

#include "boost/cgi/scgi.hpp"

namespace cgi {
 namespace scgi {
 
   class server1
   {
   public:
    //using cgi::scgi;

     typedef service protocol_service_type;

     server1()
       : service_()
       //, acceptor_()
     {
     }

     void run()
     {
       //acceptor_.async_accept(
       service_.run();
     }

     void stop()
     {
       service_.stop();
     }
   private:
     protocol_service_type service_;
     //acceptor acceptor_;
   };

 } // namespace scgi
} // namespace cgi

int main()
{
  cgi::scgi::server1 server;

  server.run();

  return 0;
}

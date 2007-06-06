/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <iostream>

// interface for the parameters of each plugin
class network_parameters
{
 public:
  virtual std::string hostname(void) = 0;
  virtual std::string port(void) = 0;
  virtual void set_http_mode(void)= 0;

  virtual ~network_parameters(void) {};
};


// MSN implementation
class MSN_network_parameters : public network_parameters
{
 public:
  virtual std::string hostname(void) { return "msn.messenger.com"; }
  virtual std::string port(void) { return "1863"; }

  virtual void set_http_mode(void) { std::cout << "http mode set" << std::endl; }

  virtual ~MSN_network_parameters() {}
};



// Jabber implementation
class Jabber_network_parameters : public network_parameters
{
 public:
  virtual std::string hostname(void) { return "jabber.org"; }
  virtual std::string port(void) { return "7063"; }

  virtual void set_http_mode(void) { std::cout << "http mode not supported" << std::endl; }

  virtual ~Jabber_network_parameters() {}
};

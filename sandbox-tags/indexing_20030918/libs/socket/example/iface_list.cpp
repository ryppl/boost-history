// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma warning (disable: 4786 4305)
  // 4786 truncated debug symbolic name
  // 4305 truncation from const double to float
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif

#include "boost/socket/interface.hpp"
#include "boost/socket/any_address.hpp"
#include "boost/socket/ip4/address.hpp"
#include <iostream>

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

using namespace boost::socket;

int main(int, char**)
{
  interface_info_list iface;
  for (interface_info_list::iterator i=iface.begin(); i!=iface.end(); ++i)
  {
    std::cout << "Address   : " << ip4::address(i->address()).to_string() << std::endl;
    std::cout << "Netmask   : " << ip4::address(i->netmask()).to_string() << std::endl;
    std::cout << "Broadcast : " << ip4::address(i->broadcast()).to_string() << std::endl;
  }
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

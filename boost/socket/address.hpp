// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef ADDRESS_HPP
#define ADDRESS_HPP 1


namespace boost
{
  namespace socket
  {
    /* NB: Address familiy is strongly linked to protocol family, but
       is independent.
    */

    //! concept check for addresses
    template <typename Address>
    struct AddressConcept
    {
      void constraints()
      {
        //short family() const
        address.family();
//         //! set the address based on a string
//         // void addr(const char* addr)
//         address.hostname("");

//         //! return the (host ordered) port number
//         //unsigned short port() const
//         address.port();
//         //! set the (host ordered) port number
//         // void port(unsigned short port)
//         address.port(1);

        // should return something that can be passed to other functions
        // sockaddr& socket_address()
        address.socket_address();

        bool x = address < address;
        address=address;
        bool y = address==address;
        bool z = address!=address;
        boost::ignore_unused_variable_warning(x);
        boost::ignore_unused_variable_warning(y);
        boost::ignore_unused_variable_warning(z);

      }

    private:
      Address address;
    };




  }
}

#endif

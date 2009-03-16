//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONVERT_TO__HPP
#define BOOST_CONVERT_TO__HPP

namespace boost {

  namespace partial_specialization_workaround {
    template < typename To, typename From >
    struct convert_to {
      inline static To apply(const From& val)
      {
        return To(val);
      }
      inline static void apply(const From& from, To& to)
      {
        to = from;
      }
    };
  }

  template < typename To, typename From >
  To convert_to(const From& val) {
    return partial_specialization_workaround::convert_to<To,From>::apply(val);
  }

  template < typename To, typename From >
  void convert_to(const From& from, To& to) {
    partial_specialization_workaround::convert_to<To,From>::apply(from, to);
  }


}

#endif


//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/enums for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/enums/scoped.hpp>

namespace Ex {
  BOOST_ENUM_CLASS_START(EC_Cons, int) {
    E0 = 3,
    E1,
    E2
  } BOOST_ENUM_CLASS_CONS_END(EC_Cons, int)
}
BOOST_ENUMS_SPECIALIZATIONS(Ex::EC_Cons, int)

#if !defined(BOOST_NO_ENUM_UNRESTRICTED_UNION)
#error "force error as not applicable as unrestricted union available"
#else
// The ENUM can NOT be used as member of a union as it is the case of the underlying enum.
union U {
  Ex::EC_Cons e;
  int i;
};
#endif


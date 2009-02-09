//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __BOOST_SYNCHRO_DETAIL_NON_ALIAS__HPP
#define __BOOST_SYNCHRO_DETAIL_NON_ALIAS__HPP
/**
* avoids aliasing by using a technique:
* disable address taking.
*/
#define BOOST_SYNCHRO_NON_ALIAS(T)   \
    T* operator&();     \


#endif

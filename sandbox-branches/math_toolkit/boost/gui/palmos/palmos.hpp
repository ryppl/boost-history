// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_PALMOS_PALMOS_HPP
#define BOOST_GUI_PALMOS_PALMOS_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <PalmOS.h>

namespace boost { namespace gui { namespace palmos
{
   // platform-dependant types

   typedef char          char_type; // [todo]: validate
   typedef const char *  string;    // [todo]: validate
}}}

#endif

/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__APPLICATION_HPP
#define BOOST__GUIGL__APPLICATION_HPP


#include <boost/signals.hpp>

namespace boost { namespace guigl {

class application {
public:
    static void run();
    static boost::signal<void()> &on_idle();
};

}}

#endif // BOOST__GUIGL__APPLICATION_HPP

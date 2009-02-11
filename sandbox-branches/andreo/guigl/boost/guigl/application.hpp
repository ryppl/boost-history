/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__APPLICATION_HPP
#define BOOST__GUIGL__APPLICATION_HPP


#include <boost/guigl/export_symbols.hpp>
#include <boost/signals.hpp>

namespace boost { namespace guigl {

class application {
public:
    BOOST_EXPORT_SYMBOLS static void run();
    BOOST_EXPORT_SYMBOLS static boost::signal<void()> &on_idle();
    BOOST_EXPORT_SYMBOLS static void timeout(const boost::function<void()> &, unsigned milliseconds);
};

}}

#endif // BOOST__GUIGL__APPLICATION_HPP

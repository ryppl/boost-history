// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_LOOP_HPP
#define BOOST_GUI_EVENT_LOOP_HPP

#include <boost/gui/platform.hpp>
#include <boost/gui/event.hpp>
#include <boost/gui/event_filter.hpp>

namespace boost { namespace gui
{

class event_loop: public event
{
   private:
      typedef event_filter * filter_chain;
      filter_chain           filters;
   public:
      void add_filter( event_filter * );
      bool preprocess();
      int  process();
   public:
      inline event_loop();
};

event_loop::event_loop(): event( 0, 0 ), filters( 0 )
{
}

}}

#endif

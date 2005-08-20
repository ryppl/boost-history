// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_FILTER_HPP
#define BOOST_GUI_EVENT_FILTER_HPP

namespace boost { namespace gui
{

class event;

class event_filter
{
   public:
      event_filter * next;
   public:
      virtual bool filter( event & ) = 0;
      inline   event_filter();
      virtual ~event_filter();
};

event_filter::event_filter(): next( 0 )
{
}

}}

#endif

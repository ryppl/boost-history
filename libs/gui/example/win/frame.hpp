// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GUIX_FRAME_HPP
#define GUIX_FRAME_HPP

#include <boost/gui/win/window_type.hpp>
#include <boost/gui/component.hpp>
#include <boost/gui/area.hpp>

namespace gui = boost::gui;

namespace guix
{

/** Manage an SDI frame with a child window (pane) that fills this window's
  * client area.
  */

class frame: public gui::component
{
   private:
      static gui::win::window_type wt;
      gui::component * pane;
   public:
      inline void set_pane( gui::component * p )
      {
         pane = p;
         if( pane ) pane -> parent = this;
      }
   public:
      virtual void EvDestroy(    gui::event * );
      virtual void EvPosChanged( gui::event * );
   public:
      frame( gui::string );
};

}

#endif

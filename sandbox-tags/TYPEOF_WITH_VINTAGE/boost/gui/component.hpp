// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_COMPONENT_HPP
#define BOOST_GUI_COMPONENT_HPP

#include <boost/gui/platform.hpp>
#include <boost/gui/event_handler.hpp>
#include <boost/gui/area.hpp>

#include <boost/property/scalar_property.hpp>
#include <boost/property/readonly_object_property.hpp>
#include <boost/property/object_property.hpp>

namespace boost { namespace gui
{

extern area default_window_area;

class component: public event_handler
{
   private:
      platf::component_handle   wh;
      platf::component_callback callback;
   public:
      inline void initialize( platf::component_handle );
      inline operator platf::component_handle() const;
   public:
      component * get_parent() const;
      void        set_parent( component * const & );
      boost::scalar_property< boost::object_property
      <
         component *, component, &component::get_parent, &component::set_parent
      > > parent;

      area get_client_area() const;
      boost::scalar_property< boost::readonly_object_property
      <
         gui::area, component, &component::get_client_area
      > > client_area;
   public:
      void resize(  const area & );
      void resize(  const size & );
      void move_to( const position & );
      void hook_events();
      void default_event( event * );
   public:
      component
      (
         string wcname, string name, component * p = 0,
         frame_style fs = frame_style(), area wa = default_window_area,
         platf::component_callback cb = 0
      );
      virtual ~component();
};

void component::initialize( platf::component_handle h )
{
   if( wh == 0 ) wh = h;
}

component::operator platf::component_handle() const
{
   return wh;
}

}}

#endif

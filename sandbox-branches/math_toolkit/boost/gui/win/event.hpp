// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_EVENT_HPP
#define BOOST_GUI_WIN_EVENT_HPP

#include <boost/gui/win/windows.hpp>
#include <iostream>
#include <string>

namespace boost { namespace gui
{

class component;

namespace win
{

std::string get_message_string( UINT message );

class event: public MSG
{
   public:
      typedef LRESULT return_type;
      typedef UINT    id_type;
      typedef HWND    handle_type;
   public: // native interoperability
      template< typename T > inline void param1( T p );
      template< typename T > inline T    param1() const;
      template< typename T > inline void param2( T p );
      template< typename T > inline T    param2() const;
   public: // generic functionality
      inline unsigned int operator()() const;
      inline HWND get_component_handle() const;
      inline void set_component_handle( HWND );
      inline gui::component * get_component() const;
   public:
      inline bool preprocess();
      inline bool get();
   public:
      inline event( id_type id, handle_type wh );
};

inline std::ostream & operator<<( std::ostream & os, const event & ev );

unsigned int event::operator()() const
{
   return message;
}

template< typename T >
void event::param1( T p )
{
   wParam = ( WPARAM )p;
}

template< typename T >
T event::param1() const
{
   return ( T )wParam;
}

template< typename T >
void event::param2( T p )
{
   lParam = ( LPARAM )p;
}

template< typename T >
T event::param2() const
{
   return ( T )lParam;
}

HWND event::get_component_handle() const
{
   return hwnd;
}

void event::set_component_handle( HWND hw )
{
   hwnd = hw;
}

gui::component * event::get_component() const
{
   return reinterpret_cast< gui::component * >
   (
      ::GetProp( hwnd, GUIPROP_COMPONENT )
   );
}

bool event::preprocess()
{
   return false;
}

bool event::get()
{
   return ::GetMessage( this, 0, 0, 0 ) > 0;
}

event::event( id_type id, handle_type wh )
{
   message = id;
   hwnd    = wh;
}

std::ostream & operator<<( std::ostream & os, const event & ev )
{
   return
   (
      os << '[' << ev.hwnd << '|' << get_message_string( ev.message ) << ']'
         << '(' << ev.wParam << ", " << ev.lParam << ')'
   );
}

}}}

#endif

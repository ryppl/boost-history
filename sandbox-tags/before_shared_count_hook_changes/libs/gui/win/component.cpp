// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/component.hpp>
#include <stdexcept>

#if defined(BOOST_MSVC)
#  pragma warning( push )
#  pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#endif

boost::gui::area boost::gui::default_window_area
(
   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
);

//std::string get_message_string( UINT message );

LRESULT CALLBACK EventHandler
(
   HWND wh, UINT message, WPARAM wParam, LPARAM lParam
)
{
   boost::gui::event ev( message, wh );
   ev.param1( wParam );
   ev.param2( lParam );
   ev.system_event = true;

   boost::gui::component * pwnd = ev.get_component();
   if( ev() == WM_NCCREATE )
   {
      CREATESTRUCT * cs = ev.param2< CREATESTRUCT * >();
      pwnd =  static_cast< boost::gui::component * >( cs -> lpCreateParams );
      pwnd -> initialize( wh );
      ::SetProp
      (
         wh, GUIPROP_COMPONENT, reinterpret_cast< HANDLE >( pwnd )
      );
   }

   if( pwnd == 0 ) return ::DefWindowProc( wh, message, wParam, lParam );

   pwnd -> onevent( &ev );
   return ev;
}

boost::gui::component * boost::gui::component::get_parent() const
{
   return reinterpret_cast< gui::component * >
   (
      ::GetProp( ::GetParent( *this ), GUIPROP_COMPONENT )
   );
}

void boost::gui::component::set_parent( component * const & w )
{
   if( this == w )
      throw( std::invalid_argument( "cannot set self as parent" ));

   ::SetParent( *this, *w );
   ::SetWindowLong
   (
      *this, GWL_STYLE,
      ( ::GetWindowLong( *this, GWL_STYLE ) | WS_CHILD | WS_VISIBLE )
         & ~0x400000 // 0x400000 == WS_CAPTION & ~WS_BORDER
   );
}

boost::gui::area boost::gui::component::get_client_area() const
{
   area client;
   ::GetClientRect( *this, &client );
   return client;
}

void boost::gui::component::resize( const area & mv )
{
   ::SetWindowPos
   (
      *this, 0, mv.xpos, mv.ypos, mv.width, mv.height, SWP_NOZORDER | SWP_NOOWNERZORDER
   );
}

void boost::gui::component::resize( const size & mv )
{
   ::SetWindowPos
   (
      *this, 0, 0, 0, mv.dx, mv.dy, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE
   );
}

void boost::gui::component::move_to( const position & pos )
{
   ::SetWindowPos
   (
      *this, 0, pos.xpos, pos.ypos, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE
   );
}

void boost::gui::component::hook_events()
{
   if( ::GetProp( *this, GUIPROP_COMPONENT ) == this )
      return; // we are already hooked into the event mechanism

   callback = reinterpret_cast< platf::component_callback >
   (
      ::SetWindowLongPtr
      (
         *this, GWLP_WNDPROC,
         reinterpret_cast< LPARAM >( EventHandler )
      )
   );
   ::SetProp
   (
      *this, GUIPROP_COMPONENT, reinterpret_cast< HANDLE >( this )
   );
}

void boost::gui::component::default_event( boost::gui::event * ev )
{
   if( ev -> system_event )
   {
      *ev = ::CallWindowProc
      (
         callback, ev -> get_component_handle(), ( *ev )(),
         ev -> param1< WPARAM >(), ev -> param2< LPARAM >()
      );
      ev -> handled = true;
   }
}

boost::gui::component::component
(
   string wcname, string name, component * p, frame_style fs, area wa,
   platf::component_callback cb
):
   wh( 0 ), parent( this ), client_area( this )
{
   if( p != 0 ) fs.style |= WS_CHILD;
   callback = cb == 0 ? ::DefWindowProc : cb;

   wh = ::CreateWindowEx
   (
      fs.styleEx, wcname, name, fs.style,
      wa.xpos, wa.ypos, wa.width, wa.height,
      ( p == 0 ) ? HWND( 0 ) : HWND( *p ), 0, ::GetModuleHandle( 0 ), this
   );
}

boost::gui::component::~component()
{
}

#if defined(BOOST_MSVC)
#  pragma warning( pop )
#endif

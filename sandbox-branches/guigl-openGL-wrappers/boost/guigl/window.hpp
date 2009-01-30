/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WINDOW_HPP
#define BOOST__GUIGL__WINDOW_HPP


#include <boost/guigl/export_symbols.hpp>
#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/parameters.hpp>
#include <boost/guigl/widget/window.hpp>
#include <boost/noncopyable.hpp>

#include <memory>
#include <string>


namespace boost { namespace guigl {

/// A window placed on the user's desktop.
class window : public guigl::widget::window, boost::noncopyable
{
    typedef argument_pack<tag::label, tag::size, tag::position, tag::background>::type
        argument_pack_type;

public:
    /// Constructor taking named parameters.
    /** \param args A pack of named parameters (accepts _label, _size, _position, _background).
    */
    BOOST_EXPORT_SYMBOLS window(const argument_pack_type &args);
    BOOST_EXPORT_SYMBOLS ~window();

    static void BOOST_EXPORT_SYMBOLS redraw(const view::positioned<> &v);
    
    class impl;
private:
    std::auto_ptr<impl> m_impl;
    
};

}}

#endif // BOOST__GUIGL__WINDOW_HPP

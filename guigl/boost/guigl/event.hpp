/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__EVENT_HPP
#define BOOST__GUIGL__EVENT_HPP


#include <boost/guigl/types.hpp>
#include <boost/variant.hpp>


namespace boost { namespace guigl {

namespace button {

enum enum_type
{
    left, middle, right
};

}

namespace direction {

enum enum_type
{
    up, down
};

}

namespace region
{

enum enum_type
{
    entry, exit
};

}

struct movement_event
{
    position_type position;
};

struct entry_exit_event
{
    entry_exit_event()
    {}
    entry_exit_event(region::enum_type value)
        : region(value)
    {}
    region::enum_type region;
};

struct button_event
{
    position_type position;
    button::enum_type button;
    direction::enum_type direction;
};

typedef boost::variant<button_event, movement_event, entry_exit_event> event_type;

}}

#endif // BOOST__GUIGL__ACCESS_HPP

// Pinhole category_iterator.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_TYPES
#define BOOST_PINHOLE_TYPES

#include <set>
#include <string>
#include <list>
#include <map>

namespace boost { namespace pinhole
{
    class property_group;

    namespace detail
    {
        struct property_info_base;
        struct signal_info_base;
        struct action_info;
    } 

    typedef std::set<std::string> category_collection;
    typedef std::list<property_group*> children_collection;
    typedef std::map<std::string, detail::property_info_base*> property_collection;
    typedef std::map<std::string, detail::signal_info_base*> signal_collection;
    typedef std::map<std::string, detail::action_info*> action_collection;
}}

#endif // include guard

// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_CONSTANT_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_CONSTANT_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // The constant property map is nearly identical to the reference
    // property map except that a) it can be constructed over an rvalue,
    // and b) the returned value cannot be changed.

    template <typename Key, typename Value>
    struct constant_property_map
    {
        typedef Key key_type;
        typedef Value value_type;
        typedef const Value& reference;
        typedef lvalue_property_map_tag category;

        inline constant_property_map(const value_type& v)
            : value(v)
        { }

        const value_type& value;
    };

    template <typename Key, typename Value>
    inline const Value&
    get(const constant_property_map<Key, Value>& pm, const Key&)
    {
        return pm.value;
    }

    template <typename Key, typename Value>
    inline void
    put(constant_property_map<Key, Value>& pm, const Key&, const Value&)
    { }


    // A generator function for creating const reference property maps.
    template <typename Key, typename Value>
    inline constant_property_map<Key, Value>
    make_constant_property_map(const Value& v)
    {
        typedef constant_property_map<Key, Value> PropertyMap;
        return PropertyMap(v);
    }
} }

#endif

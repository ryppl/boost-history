// (C) Copyright Jeremy Siek 1999-2001.
// (C) Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_IDENTITY_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_IDENTITY_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // The identity property map return has a key and value type of size_t
    // and get() returns the same value that its given as an argument.

    // NOTE: This class pretends to model the mutable lvalue property map. I
    // don't think that "identity" is really a mutable concept, but I also don't
    // think the identity of a non-cont reference is a const reference.
    // However, the semantics are a little strange. Consider:
    // pm[k] = v;       // sets k == v?
    // It's legal, but it doesn't make a lot of sense. It may be worth removing
    // these later if it turns out we don't need them.

    template <typename Value>
    struct identity_property_map
    {
        typedef Value key_type;
        typedef Value value_type;
        typedef const Value& reference;
        typedef readable_property_map_tag category;

        inline value_type& operator[](key_type& v) const
        { return v; }

        inline const value_type& operator[](const key_type& v) const
        { return v; }
    };

    template <typename Value>
    inline const Value&
    get(const identity_property_map<Value>& pm, const Value& v)
    { return v; }

    template <typename Value>
    inline Value&
    get(const identity_property_map<Value>& pm, Value& v)
    { return v; }

} }

#endif

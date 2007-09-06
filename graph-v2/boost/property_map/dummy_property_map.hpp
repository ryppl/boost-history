// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_DUMMY_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_DUMMY_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // The old description says, "when you have to supply a property map
    // but don't need it". The only possible case that this covers is providing
    // a map that can be written to, but doesn't actually do anything with the
    // writes.

    // The dummy property map basically consumes put()s. Although it's
    // fully an lvalue property map (and supports get()s as well), not
    // algorithm should rely on the returned values from either accessor.

    // NOTE: This no longer models neither the ReadablePropertyMap concept nor
    // nor the LvaluePropertyMap concept.

    template <typename Key, typename Value>
    class dummy_property_map
        : public put_get_helper< Value&, dummy_property_map<Key,Value> >
    {
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef Value& reference;
        typedef read_write_property_map_tag category;
    };

    // This function is provided for completeness, but just retuns the
    // default value of the value type.
    template <typename Key, typename Value>
    void get(const dummy_property_map<Key, Value>& pm, const Key&)
    { return Value(); }

    template <typename Key, typename Value>
    void put(dummy_property_map<Key, Value>& pm, const Key&, const Value&)
    { }

} }

#endif

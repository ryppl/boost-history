// (C) Copyright Jeremy Siek 1999-2001.
// (C) Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_REFERENCE_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_REFERENCE_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // The reference property map simply returns the same every time
    // it is accessed. Note that put()ing the value will change the value
    // that is returned for subsequent gets.

    // NOTE: This class provides lvalue access via the operator[], but doesn't
    // really need too.

    template <typename Key, typename Value>
    class reference_property_map
    : public put_get_helper< Value&, reference_property_map<Key, Value> >
    {
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef Value& reference;
        typedef lvalue_property_map_tag category;

        inline reference_property_map(value_type& v)
            : value(v)
        { }

        inline reference operator[](const key_type &) const
        { return value; }

    private:
        value_type& value;
    };

    // A generator function for creating reference property maps.
    template <typename Key, typename Value>
    inline reference_property_map<Key, Value>
    make_reference_property_map(Value& v)
    {
        typedef reference_property_map<Key, Value> PropertyMap;
        return PropertyMap(v);
    }

} }

#endif

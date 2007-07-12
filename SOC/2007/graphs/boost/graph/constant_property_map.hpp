// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CONSTANT_PROPERTY_HPP
#define BOOST_GRAPH_CONSTANT_PROPERTY_HPP

#include <boost/property_map.hpp>

namespace boost
{
    // A constant property is one, that regardless of the
    // edge or vertex given, will always return a constant
    // value.

    // The key is pretty much anything it doesn't matter. The
    // value has to be default and copy constructible.

    template <typename Key, typename Type>
    struct constant_property_map
        : public boost::put_get_helper<
                const Type&,
                constant_property_map<Key, Type> >
    {
        typedef Key key_type;
        typedef Type value_type;
        typedef const Type& reference;
        typedef boost::readable_property_map_tag category;

        constant_property_map()
            : m_value()
        { }

        constant_property_map(const value_type &value)
            : m_value(value)
        { }

        constant_property_map(const constant_property_map& copy)
            : m_value(copy.m_value)
        { }

        inline reference operator[](const key_type& v) const
        { return m_value; }

        Type m_value;
    };

    template <typename Key, typename Type>
    inline constant_property_map<Key, Type>
    make_constant_property(const Type& value)
    {
        return constant_property_map<Key, Type>(value);
    }
}

#endif

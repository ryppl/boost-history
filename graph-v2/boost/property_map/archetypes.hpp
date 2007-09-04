// (C) Copyright Jeremy Siek 1999-2001.
// (C) Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_ARCHETYPES_HPP
#define BOOST_PROPERTY_MAP_ARCHETYPES_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/concept_check.hpp>

namespace boost { namespace property_map {

    // ReadablePropertyMap
    // Supplies only the requisite get() function.
    template <typename KeyArchetype, typename ValueArchetype>
    struct readable_property_map_archetype
    {
        typedef KeyArchetype key_type;
        typedef ValueArchetype value_type;
        typedef convertible_to_archetype<ValueArchetype> reference;
        typedef readable_property_map_tag category;
    };

    template <typename K, typename V>
    const typename readable_property_map_archetype<K,V>::reference&
    get(const readable_property_map_archetype<K,V>&,
        const typename readable_property_map_archetype<K,V>::key_type&)
    {
        typedef typename readable_property_map_archetype<K,V>::reference Ref;
        return static_object<Ref>::get();
    }

    // WritablePropertyMap
    // Supplies only the requisite put() function.
    template <typename KeyArchetype, typename ValueArchetype>
    struct writable_property_map_archetype
    {
        typedef KeyArchetype key_type;
        typedef ValueArchetype value_type;
        typedef void reference;
        typedef writable_property_map_tag category;
    };

    template <typename K, typename V>
    void put(const writable_property_map_archetype<K,V>&,
             const typename writable_property_map_archetype<K,V>::key_type&,
             const typename writable_property_map_archetype<K,V>::value_type&)
    { }


    // ReadWritePropertyMap
    // This pulls in both the put() and get() functions via inheritance.
    template <typename KeyArchetype, typename ValueArchetype>
    struct read_write_property_map_archetype
        : public readable_property_map_archetype<KeyArchetype, ValueArchetype>
        , public writable_property_map_archetype<KeyArchetype, ValueArchetype>
    {
        typedef KeyArchetype key_type;
        typedef ValueArchetype value_type;
        typedef convertible_to_archetype<ValueArchetype> reference;
        typedef read_write_property_map_tag category;
    };

    // LvaluePropertyMap
    // Index operator (operator []) returns a const reference to the value type
    // and the get() function is inherited from the readable type.
    template <typename KeyArchetype, typename ValueArchetype>
    struct lvalue_property_map_archetype
        : public readable_property_map_archetype<KeyArchetype, ValueArchetype>
    {
        typedef KeyArchetype key_type;
        typedef ValueArchetype value_type;
        typedef const ValueArchetype& reference;
        typedef lvalue_property_map_tag category;

        reference operator[](const key_type&) const
        { return static_object<value_type>::get(); }
    };

    // MutableLvaluePropertyMap
    // Index operator (operator []) returns a non-const reference to the value
    // type. This also inherits the get() and put() from readable and writable
    // concepts.
    template <typename KeyArchetype, typename ValueArchetype>
    struct mutable_lvalue_property_map_archetype
        : public readable_property_map_archetype<KeyArchetype, ValueArchetype>
        , public writable_property_map_archetype<KeyArchetype, ValueArchetype>
    {
        typedef KeyArchetype key_type;
        typedef ValueArchetype value_type;
        typedef ValueArchetype& reference;
        typedef lvalue_property_map_tag category;

        reference operator[](const key_type&) const
        { return static_object<value_type>::get(); }
    };

} }

#endif

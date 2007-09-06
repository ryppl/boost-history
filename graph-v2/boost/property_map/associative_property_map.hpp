// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_ASSOCIATIVE_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_ASSOCIATIVE_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/concepts.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // Associative property maps provide lookup capabilities for unique
    // pair associative containers. There are actually two types here...
    // A non-const variety and a const variety. The associated generator
    // function will deduce the correct type to instantiate.

    // Note that the non-const version may actually insert key/value pairs
    // into to the underlying container if get()ing a key that isn't already
    // in the map. The const version has undefined behavior if the key is not
    // found (since it will try to access through end()).

    template <typename Container>
    class associative_property_map
        : public put_get_helper<
                typename Container::value_type::second_type&,
                associative_property_map<Container>
            >
    {
        typedef Container C;
    public:
        typedef typename Container::key_type key_type;
        typedef typename Container::value_type::second_type value_type;
        typedef value_type& reference;
        typedef lvalue_property_map_tag category;

        associative_property_map()
            : m_container(0)
        { }

        associative_property_map(Container& c)
            : m_container(&c)
        { }

        reference operator[](const key_type& k) const
        { return (*m_container)[k]; }

    private:
        Container* m_container;
    };

    template <class Container>
    inline associative_property_map<Container>
    make_associative_property_map(Container& c)
    {
        return associative_property_map<Container>(c);
    }



    template <typename Container>
    class const_associative_property_map
        : public put_get_helper<
                const typename Container::value_type::second_type&,
                const_associative_property_map<Container>
            >
    {
        typedef Container C;
    public:
        typedef typename Container::key_type key_type;
        typedef typename Container::value_type::second_type value_type;
        typedef const value_type& reference;
        typedef lvalue_property_map_tag category;

        const_associative_property_map()
            : m_container(0)
        { }

        const_associative_property_map(const C& c)
            : m_container(&c)
        { }

        reference operator[](const key_type& k) const
        { return m_container->find(k)->second; }

    private:
        Container const* m_container;
    };

    template <class Container>
    const_associative_property_map<Container>
    make_associative_property_map(const Container& c)
    {
        return const_associative_property_map<Container>(c);
    }

} }

#endif

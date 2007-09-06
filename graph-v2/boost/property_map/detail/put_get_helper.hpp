// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_DETAIL_PUT_GET_HELPER_HPP
#define BOOST_PROPERTY_MAP_DETAIL_PUT_GET_HELPER_HPP

namespace boost { namespace property_map {
    // This helper class will automatically generate the put() and get()
    // functions for a property map when instantiated with class that models
    // the LvaluePropertyMap concept. Most of the property maps provided by
    // this library use this class. Note that this class is actually empty,
    // and is really only used to instantiate the put() and get() functions
    // for the given LvaluePropertyMap.
    //
    // The Reference parameter should be the same as the return type of the
    // operator[] on the LvaluePropertyMap and, in most cases should actually
    // be a reference type (either const or not).
    template <class Reference, class LvaluePropertyMap>
    struct put_get_helper { };

    template <class PropertyMap, class Reference, class K>
    inline Reference
    get(const put_get_helper<Reference, PropertyMap>& pg, const K& k)
    {
        const PropertyMap& pm = static_cast<const PropertyMap&>(pg);
        return pm[k];
    }

    template <class PropertyMap, class Reference, class K, class V>
    inline void
    put(const put_get_helper<Reference, PropertyMap>& pg, const K& k, const V& v)
    {
        const PropertyMap& pm = static_cast<const PropertyMap&>(pg);
        pm[k] = v;
    }
} }

#endif

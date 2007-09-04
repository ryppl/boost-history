// (C) Copyright Jeremy Siek 1999-2001.
// (C) Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_CONCEPTS_HPP
#define BOOST_PROPERTY_MAP_CONCEPTS_HPP

#include <boost/property_map/property_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/concept_check.hpp>

#include <boost/concept/detail/concept_def.hpp>
namespace boost { namespace property_map {
    namespace concepts {
        BOOST_concept(ReadablePropertyMap, (PropertyMap)(Key))
        {
            typedef typename property_traits<PropertyMap>::key_type key_type;
            typedef typename property_traits<PropertyMap>::reference reference;
            typedef typename property_traits<PropertyMap>::category Category;
            typedef typename property_traits<PropertyMap>::value_type Value;
            typedef readable_property_map_tag ReadableTag;

            BOOST_CONCEPT_USAGE(ReadablePropertyMap)
            {
                function_requires< ConvertibleConcept<Category, ReadableTag> >();
                v = get(pmap, k);
            }

            PropertyMap pmap;
            Key k;
            Value v;
        };

        BOOST_concept(WritablePropertyMap, (PropertyMap)(Key))
        {
            typedef typename property_traits<PropertyMap>::key_type key_type;
            typedef typename property_traits<PropertyMap>::category Category;
            typedef typename property_traits<PropertyMap>::value_type Value;
            typedef writable_property_map_tag WritableTag;

            BOOST_CONCEPT_USAGE(WritablePropertyMap)
            {
                function_requires< ConvertibleConcept<Category, WritableTag> >();
                put(pmap, k, v);
            }

            PropertyMap pmap;
            Key k;
            Value v;
        };

        BOOST_concept(ReadWritePropertyMap, (PropertyMap)(Key))
        {
            typedef typename property_traits<PropertyMap>::category Category;
            typedef read_write_property_map_tag ReadWriteTag;

            BOOST_CONCEPT_USAGE(ReadWritePropertyMap)
            {
                function_requires< ConvertibleConcept<Category, ReadWriteTag> >();
                function_requires< ReadablePropertyMapConcept<PropertyMap, Key> >();
                function_requires< WritablePropertyMapConcept<PropertyMap, Key> >();
            }
        };

        BOOST_concept(LvaluePropertyMap, (PropertyMap)(Key))
        {
            typedef typename property_traits<PropertyMap>::value_type value_type;
            typedef typename property_traits<PropertyMap>::reference reference;
            typedef typename property_traits<PropertyMap>::category Category;
            typedef lvalue_property_map_tag LvalueTag;

            BOOST_CONCEPT_USAGE(LvaluePropertyMap)
            {
                function_requires< ConvertibleConcept<Category, LvalueTag> >();
                function_requires< ReadablePropertyMapConcept<PropertyMap, Key> >();

                BOOST_MPL_ASSERT((boost::mpl::or_<
                                    boost::is_same<const value_type&, reference>,
                                    boost::is_same<value_type&, reference> >));

                reference ref = pmap[k];
                ignore_unused_variable_warning(ref);
            }

            PropertyMap pmap;
            Key k;
        };

        BOOST_concept(MutableLvaluePropertyMap, (PropertyMap)(Key))
        {
            typedef typename property_traits<PropertyMap>::value_type value_type;
            typedef typename property_traits<PropertyMap>::reference reference;
            typedef typename property_traits<PropertyMap>::category Category;
            typedef lvalue_property_map_tag LvalueTag;

            BOOST_CONCEPT_USAGE(MutableLvaluePropertyMap)
            {
                boost::function_requires< ReadWritePropertyMapConcept<PropertyMap, Key> >();
                boost::function_requires<ConvertibleConcept<Category, LvalueTag> >();

                BOOST_MPL_ASSERT((boost::is_same<value_type&, reference>));

                reference ref = pmap[k];
                ignore_unused_variable_warning(ref);
            }

            PropertyMap pmap;
            Key k;
        };

    }

    using concepts::ReadablePropertyMapConcept;
    using concepts::WritablePropertyMapConcept;
    using concepts::ReadWritePropertyMapConcept;
    using concepts::LvaluePropertyMapConcept;
    using concepts::MutableLvaluePropertyMapConcept;
} }
#include <boost/concept/detail/concept_undef.hpp>

#endif

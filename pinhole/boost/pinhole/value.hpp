// Pinhole Find.hpp file
//
// Copyright Jared McIntyre 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_VALUE
#define BOOST_PINHOLE_VALUE

#include <vector>
#include <boost/foreach.hpp>
#include <boost\..\..\boost_sandbox\pinhole\boost\pinhole\find.hpp>

namespace boost { namespace pinhole
{
    /**
     * Finds specified property_group and returns the value of one of its properties as the
     * type specified. Finds property_group based on a path that describes where that property_group
     * exists within the property group hierarchy. This version does not allow for relative
     * path accessing.
     * @n
     * Path format one: /A/X.Y=Z @n
     * A represents a property group name. The assumption is that A is the only property
     * group in the current level. The path search will start from root\
     * @n
     * Path format two: /A.B=C/X.Y=Z @n
     * A and X represent property group names. B and Y represent property names. C and Z
     * represent property values. The path search will start from root.
     *
     * @param current_property_group The property_group in which to search.
     * @param path The path to search with.
     * @param property The name of the property.
     * @return The value of the property.
     * @throw invalid_path The path does not meet the requirements for a valid path.
     * @throw multiple_property_groups There were multiple property groups matching this path.
     * @throw failed_to_find_group The path is not found.
     * @throw boost::bad_function_call There isn't a get_as_string function associated with this property.
     * @throw std::out_of_range The property requested does not exist.
     * @throw std::bad_cast The requested return_type was not the same as the property's type.
     */
    template<typename T>
    T get_single_value(boost::pinhole::property_group& current_property_group, const std::string& path, const std::string& property)
    {
        property_group* pGroup = select_single_node(current_property_group, path);
        if( NULL == pGroup )
        {
            throw boost::pinhole::failed_to_find_group();
        }
        return pGroup->get<T>(property);
    }

    template<typename T>
    T get_single_value(const std::string& path, const std::string& property)
    {
        property_group* pGroup = select_single_node(path);
        if( NULL == pGroup )
        {
            throw boost::pinhole::failed_to_find_group();
        }
        return pGroup->get<T>(property);
    }

    /**
     * Finds specified property_group and sets the value of one of its properties.
     * Finds property_group based on a path that describes where that property_group
     * exists within the property group hierarchy. This version does not allow for relative
     * path accessing.
     * @n
     * Path format one: /A/X.Y=Z @n
     * A represents a property group name. The assumption is that A is the only property
     * group in the current level. The path search will start from root\
     * @n
     * Path format two: /A.B=C/X.Y=Z @n
     * A and X represent property group names. B and Y represent property names. C and Z
     * represent property values. The path search will start from root.
     *
     * @param current_property_group The property_group in which to search.
     * @param path The path to search with.
     * @param property The name of the property.
     * @return The value to set on the property.
     * @throw invalid_path The path does not meet the requirements for a valid path.
     * @throw multiple_property_groups There were multiple property groups matching this path.
     * @throw failed_to_find_group The path is not found.
     * @throw boost::bad_function_call There isn't a set_as_string function associated with this property.
     * @throw std::out_of_range The property requested does not exist.
     * @throw std::bad_cast Type T is not the same as the property's type.
     */
    template<typename T>
    void set_single_value(boost::pinhole::property_group& current_property_group, const std::string& path, const std::string& property, T value)
    {
        property_group* pGroup = select_single_node(current_property_group, path);
        if( NULL == pGroup )
        {
            throw boost::pinhole::failed_to_find_group();
        }
        pGroup->set<T>(property, value);
    }

    template<typename T>
    void set_single_value(const std::string& path, const std::string& property, T value)
    {
        property_group* pGroup = select_single_node(path);
        if( NULL == pGroup )
        {
            throw boost::pinhole::failed_to_find_group();
        }
        pGroup->set<T>(property, value);
    }

    template<typename T>
    void set_all_values(boost::pinhole::property_group& current_property_group, const std::string& path, const std::string& property, T value)
    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range(path, current_property_group);

        BOOST_FOREACH(property_group* group, range)
        {
            group->set<T>(property, value);
        }
    }

    template<typename T>
    void set_all_values(const std::string& path, const std::string& property, T value)
    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range(path);

        BOOST_FOREACH(property_group* group, range)
        {
            group->set<T>(property, value);
        }
    }
}}

#endif // include guard
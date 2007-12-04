// Pinhole Find.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIND
#define BOOST_FIND

#include "property_manager.hpp"

namespace boost { namespace pinhole
{
    namespace detail
    {
        property_group* search_single_property_group_based_on_name( children_collection* current_property_group_collection,
                                                                    const string& property_group_name)
        {
            property_group* property_group_found = 0;
            for( children_collection::iterator iter = current_property_group_collection->begin(); iter != current_property_group_collection->end(); iter++)
            {
                property_group* current_property_group = *iter;
                if(current_property_group->get_name() == property_group_name)
                {
                    if(property_group_found != 0)
                    {
                        string strError = "Multiple " + property_group_name + " are found in property_manager::selectSingleNode";
                        throw multiple_property_groups(strError.c_str());
                    }
                    property_group_found = current_property_group;
                }
            }
            if(property_group_found == 0)
            {
                throw no_metadata_defined_error();
            }
            return property_group_found;
        }

        property_group* search_single_property_group_based_on_property_value( children_collection* current_property_group_collection, 
                                                                              const string& property_group_name, 
                                                                              const string& property_name,
                                                                              const string& property_value )
        {
            property_group* property_group_found = 0;
            for(children_collection::iterator iter = current_property_group_collection->begin(); iter != current_property_group_collection->end(); iter++)
            {
                property_group* current_property_group = *iter;
                if((current_property_group->get_name() == property_group_name) && (current_property_group->get_as_string(property_name) == property_value))
                {
                    if(property_group_found != 0)
                    {
                        string strError = "Multiple " + property_group_name + "." + property_name + "=" + property_value + " are found in property_manager::selectSingleNode";
                        throw multiple_property_groups(strError.c_str());
                    }
                    property_group_found = current_property_group;
                }
            }
            if(property_group_found == 0)
            {
                throw no_metadata_defined_error();
            }
            return property_group_found;
        }
    }

    /** 
     * Returns a property_group based on a path that describes where that property_group
     * exists within the property group hierarchy. This path can either be from the
     * root of the hierarchy, or relative to the property_group passed in to the function 
     * @n
     * Path format one: /A/X.Y=Z @n
     * A represents a property group name. The assumption is that A is the only property
     * group in the current level. The path search will start from root
     * @n
     * Path format two: A/X.Y=Z @n
     * A represents a property group name. The assumption is that A is the only property
     * group in the current level. The path search will start from current_property_group
     * @n
     * Path format three: /A.B=C/X.Y=Z @n
     * A and X represent property group names. B and Y represent property names. C and Z
     * represent property values. The path search will start from root.
     * @n
     * Path format four: A.B=C/X.Y=Z @n
     * A and X represent property group names. B and Y represent property names. C and Z
     * represent property values. The path search will start from current_property_group.
     *
     * @param current_property_group The property_group to perform relative searches under.
     * @param path The path to search with.
     * @return The property_group that was found to match the path.
     * @retval NULL No property_group was found to match the path.
     * @throw invalid_path The path does not meet the requirements for a valid path.
     * @throw multiple_property_groups There were multiple property groups matching this path.
     */
    property_group* select_single_node(property_group* current_property_group, const string& path)
    {
        children_collection* current_property_group_collection = NULL;
        if( NULL != current_property_group )
        {
            current_property_group_collection = &current_property_group->get_children_collection();
        }
        children_collection* root_property_group_collection = 0; //The root collection might not be needed if it is not referred.
        
        typedef vector< string > split_vector_type;
        split_vector_type vecproperty_groups; 
        boost::split( vecproperty_groups, path, boost::is_any_of("/") ); //Note: "/" is the separator for a property group
        try
        {
            for(split_vector_type::const_iterator iter_property_group = vecproperty_groups.begin(); iter_property_group != vecproperty_groups.end(); iter_property_group++)
            {
                string strproperty_group = *iter_property_group;
                split_vector_type properties;
                boost::split( properties, strproperty_group, boost::is_any_of(".=") ); 
                //Note: "." is the separator between Property group name and Property name.
                //"=" is the separator between property name and property value. If there's no "." or "=",
                //we will just use this property group.
                size_t nNumOfItems = properties.size();
                switch(nNumOfItems)
                {
                    case 1:
                    {
                        string strItem = properties[0]; //It either is "/" or the property group name.
                        boost::trim(strItem);
                        if(strItem.empty()) //It is "/"
                        {
                            property_manager *manager = property_manager::instance();
                            root_property_group_collection = new children_collection();
                            property_manager::iterator iter    = manager->begin();
                            property_manager::iterator iterEnd = manager->end();
                            for( ; iter != iterEnd; iter++)
                            {
                                root_property_group_collection->push_back(*iter);
                            }
                            current_property_group_collection = root_property_group_collection; //From now on, we only care about current_property_group_collection.
                        }
                        else
                        {
                            //strItem here must be a property group name.
                            //If multiple or none is found ,exception will be thrown.
                            current_property_group = detail::search_single_property_group_based_on_name(current_property_group_collection, strItem);
                            current_property_group_collection = &current_property_group->get_children_collection();
                        }
                    }
                        break;
                    case 3:
                    {
                        string property_group_name = properties[0];
                        boost::trim(property_group_name);
                        
                        string property_name = properties[1];
                        boost::trim(property_name);
                        
                        string property_value = properties[2];
                        boost::trim(property_value);
                        current_property_group = detail::search_single_property_group_based_on_property_value(current_property_group_collection, property_group_name, property_name, property_value);
                        current_property_group_collection = &current_property_group->get_children_collection();
                    }
                        break;
                    default:
                    {
                        string strError = path + " is not a valid path. Details: " + strproperty_group + " should be either A.B=C or A format";
                        throw invalid_path(strError.c_str());
                    }
                }
            }
        }
        catch(no_metadata_defined_error&)
        {
            current_property_group = NULL;
        }
        
        delete root_property_group_collection;
        return current_property_group;
    }

    property_group* select_single_node(property_group& current_property_group, const string& path)
    {
        return select_single_node(&current_property_group, path);
    }

    /** 
    * Returns a property_group based on a path that describes where that property_group
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
    * @param path The path to search with.
    * @return The property_group that was found to match the path.
    * @retval NULL No property_group was found to match the path.
    * @throw invalid_path The path does not meet the requirements for a valid path.
    * @throw multiple_property_groups There were multiple property groups matching this path.
    */
    property_group* select_single_node(const string& path)
    {
        return select_single_node(NULL, path);
    }
}}

#endif // include guard
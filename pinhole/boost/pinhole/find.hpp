// Pinhole Find.hpp file
//
// Copyright Jared McIntyre 2007-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_FIND
#define BOOST_PINHOLE_FIND

#include <vector>
#include <boost\..\..\boost_sandbox\pinhole\boost\pinhole\detail\path.hpp>
#include <boost\..\..\boost_sandbox\pinhole\boost\pinhole\detail\tokenizer.hpp>
#include <boost\..\..\boost_sandbox\pinhole\boost\pinhole\path_filtered_iterator.hpp>

namespace boost { namespace pinhole
{
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
    inline boost::pinhole::property_group* select_single_node(boost::pinhole::property_group* current_property_group, const std::string& path)
    {
        std::pair<boost::pinhole::children_collection::iterator,boost::pinhole::children_collection::iterator> range;

        if( NULL != current_property_group )
        {
            range.first  = current_property_group->get_children_collection().begin();
            range.second = current_property_group->get_children_collection().end();
        }
        else
        {
            detail::ThrowIfPathIsRelative(path);
        }
        boost::pinhole::children_collection* root_property_group_collection = 0; //The root collection might not be needed if it is not referred.
        
        typedef std::vector< std::string > split_vector_type;
        split_vector_type vecproperty_groups; 
        boost::split( vecproperty_groups, path, boost::is_any_of("/") ); //Note: "/" is the separator for a property group
        try
        {
            for(split_vector_type::const_iterator iter_property_group = vecproperty_groups.begin(); iter_property_group != vecproperty_groups.end(); iter_property_group++)
            {
                std::string strproperty_group = *iter_property_group;
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
                        std::string strItem = properties[0]; //It either is "/" or the property group name.
                        boost::trim(strItem);
                        if(strItem.empty()) //It is "/"
                        {
                            boost::pinhole::property_manager::instance_type manager = boost::pinhole::property_manager::instance();
                            range.first  = manager->begin();
                            range.second = manager->end();
                        }
                        else
                        {
                            //strItem here must be a property group name.
                            //If multiple or none is found ,exception will be thrown.
                            current_property_group = detail::search_single_property_group_based_on_name(range.first, range.second, strItem);
                            
                            range.first  = current_property_group->get_children_collection().begin();
                            range.second = current_property_group->get_children_collection().end();
                        }
                    }
                        break;
                    case 3:
                    {
                        std::string property_group_name = properties[0];
                        boost::trim(property_group_name);
                        
                        std::string property_name = properties[1];
                        boost::trim(property_name);
                        
                        std::string property_value = properties[2];
                        boost::trim(property_value);
                        current_property_group = detail::search_single_property_group_based_on_property_value(range.first, range.second, property_group_name, property_name, property_value);
                        
                        range.first  = current_property_group->get_children_collection().begin();
                        range.second = current_property_group->get_children_collection().end();
                    }
                        break;
                    default:
                    {
                        std::string strError = path + " is not a valid path. Details: " + strproperty_group + " should be either A.B=C or A format";
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

    inline property_group* select_single_node(boost::pinhole::property_group& current_property_group, const std::string& path)
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
    inline property_group* select_single_node(const std::string& path)
    {
        return select_single_node(NULL, path);
    }

    typedef boost::iterator_range< boost::pinhole::path_filtered_iterator > path_filtered_range;
    typedef boost::iterator_range< boost::pinhole::const_path_filtered_iterator > const_path_filtered_range;

    inline path_filtered_range get_path_filtered_range(const std::string& path)
    {
        detail::ThrowIfPathIsRelative(path);

        return path_filtered_range( path_filtered_iterator( path, property_manager::instance()->begin(), property_manager::instance()->end() ),
                                    path_filtered_iterator( property_manager::instance()->end() ) );
    }

    inline const_path_filtered_range get_const_path_filtered_range(const std::string& path)
    {

        detail::ThrowIfPathIsRelative(path);

        return const_path_filtered_range( const_path_filtered_iterator( path, property_manager::instance()->begin(), property_manager::instance()->end() ),
                                          const_path_filtered_iterator( property_manager::instance()->end() ) );
    }

    inline path_filtered_range get_path_filtered_range(const std::string& path, property_group& group)
    {
        if( path.length() > 0 && path[0] == '/' )
        {
            return get_path_filtered_range(path);
        }

        return path_filtered_range( path_filtered_iterator( path, group.get_children_collection().begin(), group.get_children_collection().end() ),
                                    path_filtered_iterator( group.get_children_collection().end() ) );
    }

    inline const_path_filtered_range get_const_path_filtered_range(const std::string& path, property_group& group)
    {
        if( path.length() > 0 && path[0] == '/' )
        {
            return get_const_path_filtered_range(path);
        }

        return const_path_filtered_range( const_path_filtered_iterator( path, group.get_children_collection().begin(), group.get_children_collection().end() ),
                                          const_path_filtered_iterator( group.get_children_collection().end() ) );
    }
}}

#endif // include guard
// Pinhole Find.hpp file
//
// Copyright Jared McIntyre 2007-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_FIND
#define BOOST_PINHOLE_FIND

#include <vector>

#include "./detail/tokenizer.hpp"
#include "path_filtered_iterator.hpp"
#include <boost/foreach.hpp>

namespace boost { namespace pinhole
{
    namespace detail
    {
        inline
            void ThrowIfPathIsRelative(const std::string& path)
        {
            if( path.length() > 0 && path[0] != '/' )
            {
                throw boost::pinhole::invalid_path("A relative path was requested, but no property_group to search from was given.")
                    << ::boost::pinhole::exception_path(path);
            }
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
    inline boost::pinhole::property_group* select_single_node(boost::pinhole::property_group* current_property_group, const std::string& path)
    {
        typedef boost::iterator_range< boost::pinhole::children_collection::iterator > child_range;
        
        child_range range;

        if( NULL != current_property_group )
        {
            range = child_range( current_property_group->get_children_collection().begin(),
                                 current_property_group->get_children_collection().end() );
        }
        else
        {
            detail::ThrowIfPathIsRelative(path);
        }

        detail::token_path token_path;
        process_path( path, token_path );

        if( false == token_path.relative_path )
        {
            boost::pinhole::property_manager::instance_type manager = boost::pinhole::property_manager::instance();
            range = child_range(manager->begin(), manager->end() );
        }

        boost::pinhole::property_group* property_group_found = NULL;

        BOOST_FOREACH( const boost::pinhole::detail::token_path::token& token, token_path.tokens )
        {
            property_group_found = NULL;
            
            BOOST_FOREACH( property_group* group, range )
            {
                if( token.name == group->get_name() &&
                    (true == token.property.empty() || 
                    (group->is_valid_property(token.property) && token.value == group->get_as_string(token.property)) ))
                {
                    if( NULL == property_group_found )
                    {
                        property_group_found = group;
                    }
                    else
                    {
                        throw ::boost::pinhole::multiple_property_groups("Requested path does not lead to a unique property group."); 
                    }
                }
            }

            if( NULL == property_group_found )
            {
                break;
            }

            range = child_range( property_group_found->get_children_collection().begin(),
                                 property_group_found->get_children_collection().end() );
        }

        return property_group_found;
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
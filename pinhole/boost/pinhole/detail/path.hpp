// Pinhole Find.hpp file
//
// Copyright Jared McIntyre 2007-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_PATH
#define BOOST_PINHOLE_PATH

#include <vector>

namespace boost { namespace pinhole
{
    namespace detail
    {
        inline
        boost::pinhole::property_group*
            search_single_property_group_based_on_name( boost::pinhole::children_collection::iterator itr, 
                                                        boost::pinhole::children_collection::iterator itrEnd,
                                                        const std::string& property_group_name)
        {
            boost::pinhole::property_group* property_group_found = 0;
            for( ; itr != itrEnd; ++itr)
            {
                boost::pinhole::property_group* current_property_group = *itr;
                if(current_property_group->get_name() == property_group_name)
                {
                    if(property_group_found != 0)
                    {
                        std::string strError = "Multiple " + property_group_name + " are found in property_manager::selectSingleNode";
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

        inline
        boost::pinhole::property_group*
            search_single_property_group_based_on_property_value( boost::pinhole::children_collection::iterator itr, 
                                                                  boost::pinhole::children_collection::iterator itrEnd, 
                                                                  const std::string& property_group_name, 
                                                                  const std::string& property_name,
                                                                  const std::string& property_value )
        {
            boost::pinhole::property_group* property_group_found = 0;
            for(; itr != itrEnd; ++itr)
            {
                boost::pinhole::property_group* current_property_group = *itr;
                if((current_property_group->get_name() == property_group_name) && (current_property_group->get_as_string(property_name) == property_value))
                {
                    if(property_group_found != 0)
                    {
                        std::string strError = "Multiple " + property_group_name + "." + property_name + "=" + property_value + " are found in property_manager::selectSingleNode";
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

        inline
        void ThrowIfPathIsRelative(const std::string& path)
        {
            if( path.length() > 0 && path[0] != '/' )
            {
                throw boost::pinhole::invalid_path("A relative path was requested, but no property_group to search from was given.");
            }
        }
    }
}}

#endif // include guard
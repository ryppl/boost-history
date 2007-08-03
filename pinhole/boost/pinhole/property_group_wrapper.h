// MscProperty system property_group_wrapper.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_GROUP_WRAPPER
#define BOOST_PROPERTY_GROUP_WRAPPER

#include "property_group.h"

namespace boost { namespace pinhole
{
    /**
     * The property_group_wrapper allows you to create property groups without inheriting directly
     * from property_group. This allows you to do things like create groups of properties for
     * display in the UI that don't have a matching logical object in the system.
     */
    class property_group_wrapper : public property_group
    {
    public:
        /**
         * Constructor.
         * @param name The name of this property group (like an xml node name)
         * @param parent The parent of the this object.
         */
        property_group_wrapper(std::string name, property_group *parent)
            : property_group(name, parent){;}
        
        ~property_group_wrapper(void){;}

        /**
         * Adds a property to the property list.
         * @param The name of the property.
         * @param description A brief description of the property for the user interface.
         * @param setter The function used to set the property.
         * @param getter The function used to get the property.
         */
        template<typename Value_Type>
        void add_property( std::string name, 
                           std::string description,
                           boost::function<void (const Value_Type&)> setter, 
                           boost::function<Value_Type ()> getter )
        {
            property_group::add_property<Value_Type>( name, description,setter, getter );
        }

        /**
         * Adds a property to the property list.
         * @param The name of the property.
         * @param description A brief description of the property for the user interface.
         * @param setter The function used to set the property.
         * @param getter The function used to get the property.
         * @param pEditor A pointer to the editor to be used with this property, or null
         *                if there isn't one.
         */
        template<typename Value_Type>
        void add_property( std::string name, 
                           std::string description,
                           boost::function<void (const Value_Type&)> setter, 
                           boost::function<Value_Type ()> getter,
                           Editor *pEditor )
        {
            
            property_group::add_property<Value_Type>( name, description,setter, getter, pEditor );
        }

        /**
         * Adds an action to the action list.
         * @param The name of the action.
         * @param description A brief description of the action for the user interface.
         * @param action The function used to trigger the action.
         */
        inline void add_action( std::string name, 
                                std::string description,
                                boost::function<void ()> action )
        {
            
            property_group::add_action( name, description, action );
        }

        /**
        * Gets an xml string representation of this property group
        * @param string The name of the new category.
        * @return The xml string representation of this property group.
        */
        inline void add_category( const std::string &category_name )
        {
            property_group::add_category( category_name );
        }

    private:
        property_group_wrapper() : property_group("", NULL) {;}
        property_group_wrapper(const property_group& object) : property_group(object) {;}
        property_group_wrapper(const property_group_wrapper& object) : property_group(object) {;}
    };
}}

#endif // include guard
// MscProperty system Editor.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_EDITOR
#define BOOST_EDITOR

#include <map>
#include <string>

using namespace std;
//using namespace boost;

namespace boost { namespace pinhole
{
    enum EditorPropertyType
    {
        StringType,
        IntegerType,
        BooleanType,
        FloatType,
        DoubleType
    };

    enum EditorControlType
    {
        None=0,
        EditBox=1,
        Tracker=2,
        DropDown=3,
        Radio=4,
        UpDown=5
    };

    class Editor
    {
    public:
        typedef std::map<std::string, std::string> property_collection;
        typedef property_collection::size_type size_type;
        typedef property_collection::iterator iterator;
        typedef property_collection::const_iterator const_iterator;

        /**
         * Constructor which creates a default editor that is defined as
         * no editor.
         */
        Editor() : m_UItype(EditBox) {;}

        /**
         * Constructor that created an editor of the specified type.
         * @param type The editor to use.
         */
        Editor(EditorControlType type)
        {
            m_UItype = type;
        }

        /**
        * Destructor
        */
        virtual ~Editor() {;}

        /**
         * Retrieves the type of the editor to use.
         */
        EditorControlType GetControlType() const
        {
            return m_UItype;
        }

        /**
         * Sets a property for the editor. Properties are simple key/value
         * string pairs that are used by the editor to make decision. Each
         * editor has a different set of properties that it will read.
         * @param key The property key.
         * @param value The property value.
         */
        Editor& operator()( std::string key, std::string value )
        {
            m_properties.insert( make_pair(key,value) );
            
            return *this;
        }

        /**
         * Overloaded operator equal.
         * @param editor The Editor class to copy.
         */
        virtual Editor& operator=( const Editor& editor )
        {
            m_UItype = editor.m_UItype;
            
            m_properties.clear();
            m_properties.insert( editor.m_properties.begin(), editor.m_properties.end() );
            
            return *this;
        }
        

        /**
        * Retrieves the type of the property for the editor.
        */
        virtual EditorPropertyType getEditorPropertyType() const = 0;

        protected:
            EditorControlType m_UItype;
            #if defined(BOOST_MSVC)
                #pragma warning(push)
                #pragma warning( disable: 4251 )
            #endif
            property_collection m_properties;
            #if defined(BOOST_MSVC)
                #pragma warning(pop)
            #endif
    };
}}

#endif // include guard
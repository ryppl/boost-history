// Pinhole StringEditor.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STRINGEDITOR
#define BOOST_STRINGEDITOR

#include "Editor.hpp"

namespace boost { namespace pinhole
{
    class StringEditor : public Editor
    {
    public:
        StringEditor()
        {
            m_UItype = EditBox;
        }

        ~StringEditor() {;}

        /**
        * Retrieves the type of the property for the editor.
        */
        EditorPropertyType getEditorPropertyType() const
        {
            return( StringType );
        }
    };

    class WStringEditor : public Editor
    {
    public:
        WStringEditor()
        {
            m_UItype = EditBox;
        }

        ~WStringEditor() {;}

        /**
        * Retrieves the type of the property for the editor.
        */
        EditorPropertyType getEditorPropertyType() const
        {
            return( WStringType );
        }
    };
}}

#endif // include guard
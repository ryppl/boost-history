// Pinhole BoolEditor.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BOOLEDITOR
#define BOOST_BOOLEDITOR

#include "Editor.hpp"

namespace boost { namespace pinhole
{
    class BoolEditor : public Editor
    {
    public:
        BoolEditor(EditorControlType controlType=Radio)
        {
            m_UItype = controlType;
        }
        
        ~BoolEditor() {;}

        /**
        * Retrieves the type of the property for the editor.
        */
        EditorPropertyType getEditorPropertyType() const
        {
            return( BooleanType );
        }
    };
}}

#endif // include guard
// Pinhole FloatEditor.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FLOATEDITOR
#define BOOST_FLOATEDITOR

#include "Editor.hpp"

namespace boost { namespace pinhole
{
    class FloatEditor : public Editor
    {
    public:
        FloatEditor()
        {
            m_UItype = EditBox;
        };

        FloatEditor( float fLowRange, 
                     float fHighRange, 
                     float fIncrement=0.5,
                     EditorControlType controlType=EditBox ) :
            m_fLowRange( fLowRange ), 
            m_fHighRange( fHighRange ),
            m_fIncrement( fIncrement )
        {
                m_UItype = controlType;
                m_bHasRange = true;
        }

        ~FloatEditor() {;}

        /**
        * Retrieves the low range of the property for the editor.
        */
        float getLowRange() const
        {
            return( m_fLowRange );
        }

        /**
        * Retrieves the high range of the property for the editor.
        */
        float getHighRange() const
        {
            return( m_fHighRange );
        }

        /**
        * Retrieves the increment of the property for the editor.
        */
        float getIncrement() const
        {
            return( m_fIncrement );
        }

        /**
        * Retrieves the flag to use range of the property for the editor.
        */
        bool UseRange() const
        {
            return( m_bHasRange );
        }

        /**
        * Retrieves the type of the property for the editor.
        */
        EditorPropertyType getEditorPropertyType() const
        {
            return( FloatType );
        }

    private:
        float m_fLowRange, m_fHighRange, m_fIncrement;
        bool m_bHasRange;
    };
}}

#endif // include guard
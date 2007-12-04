// Pinhole DoubleEditor.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DOUBLEEDITOR
#define BOOST_DOUBLEEDITOR

#include "Editor.hpp"

namespace boost { namespace pinhole
{
    class DoubleEditor : public Editor
    {
    public:
        DoubleEditor()
        {
            m_UItype = EditBox;
        }

        DoubleEditor( double dLowRange, 
                      double dHighRange, 
                      double dIncrement=0.5,
                      EditorControlType controlType=EditBox ) :
        m_dLowRange( dLowRange ), 
        m_dHighRange( dHighRange ),
        m_dIncrement( dIncrement ),
        m_bHasRange( true )
        {
            m_UItype = controlType;
        }
        

        ~DoubleEditor() {;}

        /**
        * Retrieves the low range of the property for the editor.
        */
        double getLowRange() const
        {
            return( m_dLowRange );
        }

        /**
        * Retrieves the high range of the property for the editor.
        */
        double getHighRange() const
        {
            return( m_dHighRange );
        }

        /**
        * Retrieves the increment of the property for the editor.
        */
        double getIncrement() const
        {
            return( m_dIncrement );
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
            return( DoubleType );
        }

    private:
        double m_dLowRange, m_dHighRange, m_dIncrement;
        bool m_bHasRange;
    };
}}

#endif // include guard
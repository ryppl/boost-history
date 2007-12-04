// Pinhole IntegerEditor.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTEGEREDITOR
#define BOOST_INTEGEREDITOR

#include "Editor.hpp"

namespace boost { namespace pinhole
{
    class IntegerEditor : public Editor
    {
    public:
        IntegerEditor()
        {
            m_UItype = EditBox;
            m_bHasRange = false;
        }
        
        IntegerEditor( int iLowRange, 
                       int iHighRange, 
                       int iIncrement=0,
                       EditorControlType controlType=EditBox ) :
            m_iLowRange( iLowRange ), 
            m_iHighRange( iHighRange ),
            m_iIncrement( iIncrement )
        {
                m_UItype = controlType;
                m_bHasRange = true;
        }
        
        virtual ~IntegerEditor() {;}
        

        /**
        * Retrieves the low range of the property for the editor.
        */
        int getLowRange() const
        {
            return( m_iLowRange );
        }

        /**
        * Retrieves the high range of the property for the editor.
        */
        int getHighRange() const
        {
            return( m_iHighRange );
        }

        /**
        * Retrieves the increment of the property for the editor.
        */
        int getIncrement() const
        {
            return( m_iIncrement );
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
            return( IntegerType );
        }

    private:
        int m_iLowRange, m_iHighRange, m_iIncrement;
        bool m_bHasRange;
    };
}}

#endif // include guard
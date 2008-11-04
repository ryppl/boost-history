/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/
#ifndef __itl_stringpattern_set_JOFA_040902_H__
#define __itl_stringpattern_set_JOFA_040902_H__

#include "prefix_set.h"
namespace boost{namespace itl
{
    /// Specifies a set of strings via prefix_set s include and exclude
    /** stringpattern_set repraesentiert eine Menge von Strings durch
        zwei PrefixSetTs, der zugehoerigen und der nicht zugehoerigen
        Strings.

        Mit 'include' fuegt man Praefixe fuer zugehoerige Strings ein.
        Mit 'exclude' fuegt man Praefixe fuer nicht zugehoerige Strings ein.
        
        stringpattern_set s;
        s.include("Han");
        s.include("Har");
        s.exclude("Hara");
        s.contains("Hans")==true;
        s.contains("Harald")==false;
    */
    class stringpattern_set
    {
    public:
        void include(const string& val)
        { m_Includes.insert(val); }

        void exclude(const string& val)
        { m_Excludes.insert(val); }
        
        bool contains(const string& val)const
        { return m_Includes.contains(val) && !m_Excludes.contains(val); }
        
    private:
        prefix_set m_Includes;
        prefix_set m_Excludes;
    };

}} // namespace itl boost

#endif // __itl_stringpattern_set_JOFA_040902_H__



/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_is_interval_container_JOFA_081004_H__
#define __itl_type_traits_is_interval_container_JOFA_081004_H__

namespace boost{ namespace itl
{
    template <class Type> struct is_interval_container
    { 
        typedef is_interval_container<Type> type;
        static const bool value = false; 
    };

    template<class Type>
    struct is_interval_map
    {
        typedef is_interval_map<Type> type; 
        static const bool value =
            is_interval_container<Type>::value && is_map<Type>::value; 
    };

    template<class Type>
    struct is_interval_set
    { 
        typedef is_interval_set<Type> type; 
        static const bool value =
            is_interval_container<Type>::value && !is_interval_map<Type>::value; 
    };


}} // namespace boost itl

#endif



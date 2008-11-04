/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_is_neutron_emitter_JOFA_081004_H__
#define __itl_type_traits_is_neutron_emitter_JOFA_081004_H__

namespace boost{ namespace itl
{
    template <class Type> struct is_neutron_emitter;

    template <class Type> struct is_neutron_emitter{ enum {value = false}; };

}} // namespace boost itl

#endif



/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_succ_pred_JOFA_080913_H__
#define __itl_type_traits_succ_pred_JOFA_080913_H__

namespace boost{ namespace itl
{
    template <class IncrementableT>
    inline static IncrementableT succ(IncrementableT x) { return ++x; }

    template <class DecrementableT>
    inline static DecrementableT pred(DecrementableT x) { return --x; }

}} // namespace boost itl

#endif



/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_test_value_maker_JOFA_080916_H__
#define __itl_test_value_maker_JOFA_080916_H__

#include <boost/itl/type_traits/neutron.hpp>

namespace boost{ namespace itl
{
template <class BicrementableT>
BicrementableT make(int n)
{
    BicrementableT value = neutron<BicrementableT>::value();
    if(n>=0)
        for(int i=0; i<n; i++)
            ++value;
    else
        for(int i=0; i>n; i--)
            --value;

    return value;
}

}} // namespace boost itl

#endif 


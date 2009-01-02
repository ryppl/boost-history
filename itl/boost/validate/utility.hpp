/*----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <stdio.h>

#define ITL_LOCATION(message) location(__FILE__,__LINE__,message)

namespace boost{namespace itl
{
    std::string location(const std::string& file, int line, const std::string& message)
    {
        std::string result = file;
        result += "(" + to_string<int>::apply(line) + "): ";
        result += message;
        return result;
    }

}} // namespace itl boost


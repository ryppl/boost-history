/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_EXAMPLE_STRUCT_TYPE_HPP
#define EXTENSION_STRUCT_EXAMPLE_STRUCT_TYPE_HPP

#include <string>

namespace example
{
    struct example_struct
    {
        std::string name;
        int age;

        example_struct(std::string const& name,int age)
          : name(name), age(age)
        {}
    };
}

#endif

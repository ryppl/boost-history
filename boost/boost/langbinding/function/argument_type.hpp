// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ARGUMENT_TYPE_DWA2004917_HPP
# define ARGUMENT_TYPE_DWA2004917_HPP

#include <typeinfo>

namespace boost { namespace langbinding { namespace function { 

struct argument_type
{
    argument_type();
    argument_type(std::type_info const& type, bool lvalue);

    std::type_info const* type;
    bool lvalue;
};

inline argument_type::argument_type()
    : type(&typeid(void))
    , lvalue(false)
{
}

inline argument_type::argument_type(std::type_info const& type, bool lvalue)
    : type(&type)
    , lvalue(lvalue)
{
}

}}} // namespace boost::langbinding::function

#endif // ARGUMENT_TYPE_DWA2004917_HPP

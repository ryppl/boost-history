// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONVERTER_FUNCTION_040923_HPP
#define CONVERTER_FUNCTION_040923_HPP

namespace boost { namespace langbinding { namespace function {

struct holder_installer;

}}} // namespace boost::langbinding::function

namespace boost { namespace langbinding { namespace converter {

struct from_xxx_data
{
    void* source;
    void* convertible;
    void*(*construct)(void*, void*);
};

typedef from_xxx_data (*from_xxx_function)(void* src);
typedef void* (*to_xxx_function)(void* src, function::holder_installer const& installer);

}}} // namespace boost::langbinding::converter

#endif // CONVERTER_FUNCTION_040923_HPP


// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONVERTER_FUNCTION_040923_HPP
#define CONVERTER_FUNCTION_040923_HPP

namespace boost { namespace langbinding { namespace converter {

struct arg_conversion
{
    void* source;
    void* convertible;
    void*(*construct)(void*, void*);
};

typedef arg_conversion (*converter_function)(void* src);

}}} // namespace boost::langbinding::converter

#endif // CONVERTER_FUNCTION_040923_HPP


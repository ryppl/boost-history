// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/converter/converter.hpp>
#include <boost/langbinding/converter/registry.hpp>
#include <boost/lexical_cast.hpp>

using boost::langbinding::util::type_id;
using namespace boost::langbinding::converter;

void* construct_int(void* src, void* storage)
{
    std::string& str(*static_cast<std::string*>(src));
    int* result = new (storage) int(boost::lexical_cast<int>(str));
    return result;
}

arg_conversion int_(void* src)
{
    arg_conversion result;
    result.source = src;

    try
    {
        boost::lexical_cast<int>(*static_cast<std::string*>(src));
    }
    catch (boost::bad_lexical_cast&)
    {
        result.convertible = 0;
        result.construct = 0;
        return result;
    }

    result.convertible = src;
    result.construct = &construct_int;
    return result;   
}

int main()
{
    registry::insert(true, type_id<int>(), &int_);

    std::string arg("42");
    arg_conversion result = registry::convert(
        &arg
      , registry::get(type_id<int>())->rvalue_converters
    );

    assert(result.convertible);

    int x;
    result.construct(result.source, &x);
    assert(x == 42);
}


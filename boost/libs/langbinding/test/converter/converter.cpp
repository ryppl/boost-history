// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/converter/converter.hpp>
#include <boost/langbinding/converter/registry.hpp>
#include <boost/lexical_cast.hpp>
#include <cassert>

using boost::langbinding::util::type_id;
using namespace boost::langbinding::converter;

struct int_ : converter<int_, std::string, int>
{
    static void construct(std::string const& src, void* storage)
    {
        new (storage) int(boost::lexical_cast<int>(src));
    }

    static void* convertible(std::string const& src)
    {
        try
        {
            boost::lexical_cast<int>(src);
        }
        catch (boost::bad_lexical_cast&)
        {
            return 0;
        }

        return yes;
    }
};

int main()
{
    int_ register_int;

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


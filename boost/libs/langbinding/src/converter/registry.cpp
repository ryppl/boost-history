// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/converter/registry.hpp>
#include <boost/langbinding/util/type_id.hpp>
#include <map>

namespace boost { namespace langbinding { namespace converter { namespace registry {

namespace {

    typedef std::map<util::type_info, registration> registry_type;

    registry_type& registry_()
    {
        static registry_type x;
        return x;
    }

} // namespace unnamed

BOOST_LANGBINDING_DECL void insert(
    bool rvalue, util::type_info type, converter_function x)
{
    registration* r = get(type);

    if (!r)
    {
        r = &registry_().insert(std::make_pair(type, registration())).first->second;
    }

    converter_chain* cv = new converter_chain(x);

    if (rvalue)
    {
        cv->next = r->rvalue_converters;
        r->rvalue_converters = cv;
    }
    else
    {
        cv->next = r->lvalue_converters;
        r->lvalue_converters = cv;
    }
}

BOOST_LANGBINDING_DECL registration* get(util::type_info type)
{
    registry_type::iterator i(registry_().find(type));
    return i == registry_().end() ? 0 : &i->second;
}

BOOST_LANGBINDING_DECL arg_conversion convert(void* src, converter_chain* chain)
{
    arg_conversion result;

    for (; chain != 0; chain = chain->next)
    {
        result = chain->convertible(src);

        if (result.convertible)
        {
            return result;
        }
    }

    return result;
}

}}}} // namespace boost::langbinding::converter::registry


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
    backend::id backend_, bool rvalue, util::type_info type, from_xxx_function x)
{
    backend_registration& r = acquire(type).get(backend_);
    converter_chain* cv = new converter_chain(x);

    if (rvalue)
    {
        cv->next = r.rvalue_from_xxx;
        r.rvalue_from_xxx = cv;
    }
    else
    {
        cv->next = r.lvalue_from_xxx;
        r.lvalue_from_xxx = cv;
    }
}

BOOST_LANGBINDING_DECL registration& acquire(util::type_info type)
{
    return registry_()[type];
}

BOOST_LANGBINDING_DECL registration* lookup(util::type_info type)
{
    registry_type::iterator i(registry_().find(type));
    return i == registry_().end() ? 0 : &i->second;
}

BOOST_LANGBINDING_DECL from_xxx_data convert(void* src, converter_chain* chain)
{
    from_xxx_data result;

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


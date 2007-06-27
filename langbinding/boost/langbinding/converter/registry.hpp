// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef REGISTRY_040922_HPP
#define REGISTRY_040922_HPP

#include <boost/langbinding/function/config.hpp>
#include <boost/langbinding/converter/converter_function.hpp>
#include <boost/langbinding/util/type_id.hpp>
#include <boost/langbinding/backend/id.hpp>
#include <boost/langbinding/backend/class.hpp>

#include <vector>

namespace boost { namespace langbinding { namespace converter { namespace registry {

struct converter_chain
{
    converter_chain(from_xxx_function);

    from_xxx_function convertible;
    converter_chain* next;
};

inline converter_chain::converter_chain(from_xxx_function x)
    : convertible(x)
    , next(0)
{
}

struct backend_registration
{
    backend_registration();

    backend::class_* class_;
    to_xxx_function to_xxx;
    converter_chain* lvalue_from_xxx;
    converter_chain* rvalue_from_xxx;
};

inline backend_registration::backend_registration()
    : class_(0)
    , to_xxx(0)
    , lvalue_from_xxx(0)
    , rvalue_from_xxx(0)
{
}

// The registration is specific to a typeid. For every back end
// and typeid there is a backend_registration instance.
struct registration
{
    inline backend_registration& get(backend::id);

private:
    std::vector<backend_registration> backends;
};

inline backend_registration& registration::get(backend::id id)
{
    if (backends.size() <= id)
    {
        backends.resize(id + 1);
    }

    return backends[id];
}

BOOST_LANGBINDING_DECL void insert(
    backend::id, bool rvalue, util::type_info, from_xxx_function);

BOOST_LANGBINDING_DECL registration& acquire(util::type_info);
BOOST_LANGBINDING_DECL registration* lookup(util::type_info);
BOOST_LANGBINDING_DECL from_xxx_data convert(void* src, converter_chain* chain);

}}}} // namespace boost::langbinding::converter::registry

#endif // REGISTRY_040922_HPP


// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef REGISTRY_040922_HPP
#define REGISTRY_040922_HPP

#include <boost/langbinding/function/config.hpp>
#include <boost/langbinding/converter/converter_function.hpp>
#include <boost/langbinding/util/type_id.hpp>

namespace boost { namespace langbinding { namespace converter { namespace registry {

struct converter_chain
{
    converter_chain(converter_function);

    converter_function convertible;
    converter_chain* next;
};

inline converter_chain::converter_chain(converter_function x)
    : convertible(x)
    , next(0)
{
}

struct registration
{
    registration();

    converter_chain* lvalue_converters;
    converter_chain* rvalue_converters;
};

inline registration::registration()
    : lvalue_converters(0)
    , rvalue_converters(0)
{
}

BOOST_LANGBINDING_DECL void insert(bool rvalue, util::type_info, converter_function);
BOOST_LANGBINDING_DECL void insert(bool rvalue, util::type_info, converter_function);
BOOST_LANGBINDING_DECL registration* get(util::type_info);
BOOST_LANGBINDING_DECL arg_conversion convert(void* src, converter_chain* chain);

}}}} // namespace boost::langbinding::converter::registry

#endif // REGISTRY_040922_HPP


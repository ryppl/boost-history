// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MAKE_FUNCTION_040916_HPP
#define MAKE_FUNCTION_040916_HPP

#include <boost/langbinding/function/aux_/invoker.hpp>
#include <boost/langbinding/function/aux_/get_signature.hpp>
#include <boost/langbinding/function/aux_/arg_extractor.hpp>

#include <memory>

namespace boost { namespace langbinding { namespace function {

template<class F, class Signature>
std::auto_ptr<invoker> make_function(F fn, Signature)
{
    return std::auto_ptr<invoker>(new aux::invoker<F, Signature>(fn));
}

template<class F>
std::auto_ptr<invoker> make_function(F fn)
{
    return function::make_function(fn, aux::get_signature(fn));
}

}}} // namespace boost::langbinding::function

#endif // MAKE_FUNCTION_040916_HPP


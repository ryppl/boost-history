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


#ifndef INVOKER_040916_HPP
#define INVOKER_040916_HPP

#include <boost/langbinding/function/argument_type.hpp>
#include <boost/langbinding/function/result_converter.hpp>
#include <boost/langbinding/converter/converter.hpp>
#include <typeinfo>

#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost { namespace langbinding { namespace function {

class invoker
{
public:
    virtual ~invoker() {}
    virtual int arity() const = 0;

    virtual void* invoke(
        backend::plugin const& backend_
      , converter::arg_conversion* args) const = 0;

    virtual argument_type const* arguments() const = 0;
    virtual argument_type const& return_type() const = 0;
};

}}} // namespace boost::langbinding::function

#endif // INVOKER_040916_HPP


#ifndef INVOKER_040916_HPP
#define INVOKER_040916_HPP

#include <boost/langbinding/function/argument_type.hpp>

#include <typeinfo>

namespace boost { namespace langbinding { namespace function {

class invoker
{
public:
    virtual ~invoker() {}
    virtual int arity() const = 0;
    virtual void invoke(void** args, void* result) const = 0;
    virtual argument_type const* arguments() const = 0;
    virtual argument_type const& return_type() const = 0;
};

}}} // namespace boost::langbinding::function

#endif // INVOKER_040916_HPP


#ifndef INVOKER_040916_HPP
#define INVOKER_040916_HPP

#include <boost/langbinding/function/argument_type.hpp>
#include <typeinfo>

#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost { namespace langbinding { namespace function {

class result_converter_base
{
public:
    virtual ~result_converter_base() {}
};

template<class T>
class result_converter : public result_converter_base
{
public:
    typedef typename mpl::eval_if<
        mpl::or_<
            is_pointer<T>
          , is_reference<T>
        >
      , mpl::identity<T>
      , add_reference<T const>
    >::type argument_type;

    virtual void* operator()(argument_type) = 0;
};

template<>
class result_converter<void> : public result_converter_base
{
public:
    virtual void* operator()() = 0;
};

struct arg_conversion
{
    void* source;
    void* convertible;
    void*(*construct)(void*, void*);
};

class invoker
{
public:
    virtual ~invoker() {}
    virtual int arity() const = 0;
    virtual void* invoke(arg_conversion* args, result_converter_base& rc) const = 0;
    virtual argument_type const* arguments() const = 0;
    virtual argument_type const& return_type() const = 0;
};

}}} // namespace boost::langbinding::function

#endif // INVOKER_040916_HPP


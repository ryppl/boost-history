// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INVOKER_DWA2004917_HPP
# define INVOKER_DWA2004917_HPP

#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/front.hpp>

#include <boost/spirit/fusion/sequence/type_sequence.hpp>
#include <boost/spirit/fusion/algorithm/transform.hpp>
#include <boost/spirit/fusion/sequence/generate.hpp>

#include <boost/langbinding/function/invoker.hpp>
#include <boost/langbinding/function/argument_type.hpp>
#include <boost/langbinding/function/aux_/arg_extractor.hpp>
#include <boost/langbinding/function/aux_/invoke.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux { 

template<class T>
struct wrap
{
    typedef wrap type;
    typedef T value_type;
};

struct fill_argument_types
{
    fill_argument_types(argument_type* args) : arg(args) {}

    template<class T>
    void operator()(wrap<T>)
    {
        arg->type = &typeid(T);
        arg->lvalue = false;
        ++arg;
    }

    argument_type* arg;
};

template<class Signature>
struct arg_vector
{
    arg_vector();
    argument_type args[mpl::size<Signature>::value];
    static arg_vector instance;
};

template<class Signature>
arg_vector<Signature> arg_vector<Signature>::instance;

template<class Signature>
inline arg_vector<Signature>::arg_vector()
{
    mpl::for_each<Signature, wrap<mpl::_> >(fill_argument_types(args));
}

struct transform_extractor_op
{
    transform_extractor_op(void**& args) : args(args) {}

    template<class T>
    struct apply
        : arg_extractor<typename T::value_type>
    {
    };

    template<class T>
    typename apply<wrap<T> >::type operator()(wrap<T>) const
    {
        return typename apply<wrap<T> >::type(*args++);
    }

    mutable void**& args;
};

template<class R, class Arguments>
struct converter_package
{
    converter_package(
        R& result_converter
      , Arguments const& converters)
        : return_(result_converter)
        , args(converters)
    {
    }

    R& return_;
    Arguments const& args;
};

template<class R, class Converters>
converter_package<R, Converters> make_converter_package(
    R& return_, Converters const& args)
{
    return converter_package<R, Converters>(return_, args);
}

template<class F, class Signature>
struct invoker : boost::langbinding::function::invoker
{
    invoker(F fn);

    typedef mpl::iterator_range<
        typename mpl::next<
            typename mpl::begin<Signature>::type
        >::type
      , typename mpl::end<Signature>::type
    > argument_types;

    typedef mpl::transform_view<
        argument_types
      , wrap<mpl::_>
    > argument_tuple;

    typedef typename mpl::front<Signature>::type return_type_;
    
    void* invoke(void** args, result_converter_base& rc_) const
    {
        result_converter<return_type_>& rc = static_cast<
            result_converter<return_type_>&>(rc_);
        
        aux::invoke(
            aux::invoke_tag<
                return_type_
              , F
            >()
          , typename mpl::size<argument_types>::type()
          , m_fn
          , make_converter_package(
                rc
              , fusion::generate(fusion::transform(
                    argument_tuple()
                  , transform_extractor_op(args))
            ))
        );
    }

    int arity() const
    {
        return mpl::size<argument_types>();
    }

    argument_type* arguments() const
    {
        return arg_vector<mpl::transform_view<
            argument_types
          , wrap<mpl::_>
        > >::instance.args;
    }

    argument_type const& return_type() const
    {
        static argument_type x;
        return x;
    }

private:
    F m_fn;
};

template<class F, class Signature>
inline invoker<F, Signature>::invoker(F fn)
    : m_fn(fn)
{
}

}}}} // namespace boost::langbinding::function::aux

#endif // INVOKER_DWA2004917_HPP

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

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/spirit/fusion/sequence/type_sequence.hpp>
#include <boost/spirit/fusion/algorithm/transform.hpp>
#include <boost/spirit/fusion/sequence/generate.hpp>

#include <boost/langbinding/function/invoker.hpp>
#include <boost/langbinding/function/argument_type.hpp>
#include <boost/langbinding/function/aux_/arg_extractor.hpp>
#include <boost/langbinding/function/aux_/invoke.hpp>
#include <boost/langbinding/function/aux_/fusion_arg_iterator.hpp>
#include <boost/langbinding/util/type_id.hpp>

#include <stdexcept>

namespace boost { namespace langbinding { namespace function { namespace aux { 

template<class T>
struct wrap
{
    typedef wrap type;
    typedef T value_type;
};

// TODO: rewrite this to just use type traits..
struct fill_argument_types
{
    fill_argument_types(argument_type* args) : arg(args) {}

    template<class T>
    void operator()(wrap<T>)
    {
        typedef typename remove_cv<
            typename remove_pointer<
                typename remove_reference<T>::type
            >::type
        >::type stripped_type;

        arg->type = util::type_id<stripped_type>();
        arg->lvalue = is_pointer<T>() 
            || boost::detail::is_reference_to_non_const<T>();
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
    mpl::for_each<Signature>(fill_argument_types(args));
}

template<class R, class Arguments>
struct converter_package
{
    converter_package(
        R const& result_converter
      , Arguments const& converters)
        : return_(result_converter)
        , args(converters)
    {
    }

    R const& return_;
    Arguments const& args;
};

template<class R, class Converters>
converter_package<R, Converters> make_converter_package(
    R const& return_, Converters const& args)
{
    return converter_package<R, Converters>(return_, args);
}

template<class F, class Signature>
struct invoker : boost::langbinding::function::invoker
{
    invoker(F fn);

    typedef mpl::iterator_range<
        typename mpl::next<typename mpl::begin<Signature>::type>::type
      , typename mpl::end<Signature>::type
    > argument_types;

    typedef typename mpl::front<Signature>::type return_type_;
    typedef result_converter<return_type_> rc_type;

    void* invoke(
        backend::plugin const& backend_
      , converter::from_xxx_data* args) const
    {
        aux::fusion_arg_iterator arg_iterator(args);

        typename fusion::meta::generate<
            mpl::transform_view<
                argument_types
              , arg_extractor<mpl::_>
            >
        >::type argument_converters(arg_iterator);

        // Find to_xxx converter in registry.
        converter::to_xxx_function rc_fn = get_result_converter(
            mpl::identity<return_type_>()
          , backend_.id()
        );

        return aux::invoke(
            aux::invoke_tag<
                return_type_
              , F
            >()
          , typename mpl::size<argument_types>::type()
          , m_fn
          , make_converter_package(
                rc_type(rc_fn, backend_)
              , argument_converters
            )
        );
    }

    // void return means no to_xxx converter in the registry
    static converter::to_xxx_function get_result_converter(
        mpl::identity<void>, backend::id)
    {
        return 0;
    }

    template<class T>
    static converter::to_xxx_function get_result_converter(
        mpl::identity<T>, backend::id backend_)
    {
        converter::to_xxx_function result = 
            converter::registered<T>::instance.get(backend_).to_xxx;

        if (!result)
        {
            throw std::runtime_error("No result converter found");
        }

        return result;
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
        static argument_type x(util::type_id<return_type_>(), false);
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


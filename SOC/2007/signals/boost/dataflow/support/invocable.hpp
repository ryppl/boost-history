// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_INVOCABLE_HPP
#define BOOST_DATAFLOW_SUPPORT_INVOCABLE_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>


namespace boost { namespace dataflow {

struct functor_invocable;

template<typename T, typename Enable=void>
struct is_invocable
    : public boost::false_type {};

template<typename T, typename Enable=void>
struct invocable_category_of;

template<typename T>
struct invocable_category_of<
    T,
    typename detail::enable_if_defined<typename T::invocable_category>::type >
{
    typedef typename T::invocable_category type;
};

template<typename T>
struct is_invocable<
    T,
    typename detail::enable_if_defined<invocable_category_of<T> >::type >
: public boost::true_type {};

namespace extension
{
    template<typename InvocableTag, typename Enable=void>
    struct invoke_impl
    {
        template<typename Invocable>
        struct apply
        {
            static void call(Invocable &)
            {
                // Error: invoke_impl has not been implemented for InvocableTag.
                BOOST_STATIC_ASSERT(sizeof(Invocable)==0);
            }
        };
    };
    
    template<>
    struct invoke_impl<boost::dataflow::functor_invocable>
    {
        template<typename Invocable>
        struct apply
        {
            static void call(Invocable &invocable)
            {
                invocable();
            }
        };
    };        
}

template<typename Invocable>
typename boost::enable_if<is_invocable<Invocable> >::type
invoke(Invocable &invocable)
{
    extension::invoke_impl<typename invocable_category_of<Invocable>::type>
        ::template apply<Invocable>::call(invocable);
}

template<typename Invocable>
typename boost::enable_if<is_invocable<Invocable> >::type
invoke(const Invocable &invocable)
{
    extension::invoke_impl<typename invocable_category_of<Invocable>::type>
        ::template apply<const Invocable>::call(invocable);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_INVOCABLE_HPP

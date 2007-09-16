// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PROXY_CONSUMER_HPP
#define BOOST_DATAFLOW_SUPPORT_PROXY_CONSUMER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/consumer.hpp>

#include <boost/type_traits/integral_constant.hpp>

namespace boost { namespace dataflow {

template<typename T, typename Enable=void>
struct proxied_consumer_of
{
};

template<typename T>
struct proxied_consumer_of<T,
typename detail::enable_if_defined<typename T::proxy_consumer_for >::type >
{
    typedef typename T::proxy_consumer_for type;
};

struct default_proxy_consumer;
struct mutable_proxy_consumer;

template<typename T, typename Enable=void>
struct proxy_consumer_category_of
{
    typedef default_proxy_consumer type;
};

template<typename T>
struct proxy_consumer_category_of<T,
typename detail::enable_if_defined<typename T::proxy_consumer_category >::type >
{
    typedef typename T::proxy_consumer_category type;
};

// trait determining whether a type is a consumer proxy.
template<typename T, typename Enable=void>
struct is_proxy_consumer
    : public boost::false_type {};

template<typename T>
struct is_proxy_consumer<T,
        typename detail::enable_if_defined<
            detail::all_of<
                typename proxied_consumer_of<T>::type,
                typename proxy_consumer_category_of<T>::type
            >
        >::type >
    : public boost::true_type {};

template<typename T, typename Enable=void>
struct get_proxied_consumer_type
{
    typedef T type;
};

template<typename T>
struct get_proxied_consumer_type<T, 
    typename boost::enable_if<is_proxy_consumer<T> >::type>
{
    typedef
        typename get_proxied_consumer_type<
            typename proxied_consumer_of<T>::type
        >::type type;
};

namespace extension
{
    template<typename ProxyProducerTag>
    struct get_proxied_consumer_impl
    {
        template<typename ProxyProducer>
        struct apply
        {
            static void call(const ProxyProducer &)
            {
                // Error: get_proxied_consumer_impl has not been implemented
                // for ProxyProducerTag.
                BOOST_STATIC_ASSERT(sizeof(ProxyProducer)==0);
            }
        };
    };

    template<>
    struct get_proxied_consumer_impl<default_proxy_consumer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type & type;
        };
        
        template<typename ProxyProducer>
        struct result<const ProxyProducer>
        {
            typedef const typename
                boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {            
            static typename
            boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type &
            call(ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
            static
            const typename boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
        };
    };
    
    template<>
    struct get_proxied_consumer_impl<mutable_proxy_consumer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {
            static
            typename boost::dataflow::get_proxied_consumer_type<ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
        };
    };

}

template<typename T>
typename boost::disable_if<
    is_proxy_consumer<T>,
    const T &
>::type
get_proxied_consumer(const T &t)
{
    return t;
}

template<typename T>
typename boost::disable_if<
    is_proxy_consumer<T>,
    T &
>::type
get_proxied_consumer(T &t)
{
    return t;
}

template<typename T>
typename boost::lazy_enable_if<
    is_proxy_consumer<T>,
    typename extension::get_proxied_consumer_impl<
        typename proxy_consumer_category_of<T>::type>::template result<T>
>::type
get_proxied_consumer(T &t)
{
    return extension::get_proxied_consumer_impl<
        typename proxy_consumer_category_of<T>::type>::template apply<T>::call(t);
}

template<typename T>
typename boost::lazy_enable_if<
    is_proxy_consumer<T>,
    typename extension::get_proxied_consumer_impl<
        typename proxy_consumer_category_of<T>::type>::template result<const T>
>::type
get_proxied_consumer(const T &t)
{
    return extension::get_proxied_consumer_impl<
        typename proxy_consumer_category_of<T>::type>::template apply<T>::call(t);
}

template<typename T>
struct consumer_category_of<T,
    typename boost::enable_if<is_proxy_consumer<T> >::type>
{
    typedef
        typename consumer_category_of<
            typename get_proxied_consumer_type<T>::type
        >::type type;
};

template<typename T>
struct produced_type_of<T, typename boost::enable_if<is_proxy_consumer<T> >::type>
{
    typedef
        typename produced_type_of<
            typename get_proxied_consumer_type<T>::type
        >::type type;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

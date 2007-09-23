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

template<typename Mechanism, typename T, typename Enable=void>
struct proxied_consumer_of
{
};

template<typename Mechanism, typename T>
struct proxied_consumer_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::proxy_consumer_for >::type >
{
    typedef typename T::template dataflow<Mechanism>::proxy_consumer_for type;
};

struct default_proxy_consumer;
struct mutable_proxy_consumer;

template<typename Mechanism, typename T, typename Enable=void>
struct proxy_consumer_category_of
{
    typedef default_proxy_consumer type;
};

template<typename Mechanism, typename T>
struct proxy_consumer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::proxy_consumer_category
    >::type >
{
    typedef typename T::template dataflow<Mechanism>::proxy_consumer_category type;
};

// trait determining whether a type is a consumer proxy.
template<typename Mechanism, typename T, typename Enable=void>
struct is_proxy_consumer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_proxy_consumer<Mechanism, T,
        typename detail::enable_if_defined<
            detail::all_of<
                typename proxied_consumer_of<Mechanism, T>::type,
                typename proxy_consumer_category_of<Mechanism, T>::type
            >
        >::type >
    : public boost::true_type {};

template<typename Mechanism, typename T, typename Enable=void>
struct get_proxied_consumer_type
{
    typedef T type;
};

template<typename Mechanism, typename T>
struct get_proxied_consumer_type<Mechanism, T, 
    typename boost::enable_if<is_proxy_consumer<Mechanism, T> >::type>
{
    typedef
        typename get_proxied_consumer_type<
            Mechanism,
            typename proxied_consumer_of<Mechanism, T>::type
        >::type type;
};

namespace extension
{
    template<typename Mechanism, typename ProxyProducerTag>
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

    template<typename Mechanism>
    struct get_proxied_consumer_impl<Mechanism, default_proxy_consumer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_consumer_type<
                    Mechanism,
                    ProxyProducer
                >::type & type;
        };
        
        template<typename ProxyProducer>
        struct result<const ProxyProducer>
        {
            typedef const typename
                boost::dataflow::get_proxied_consumer_type<
                    Mechanism, 
                    ProxyProducer
                >::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {            
            static typename
            boost::dataflow::get_proxied_consumer_type<Mechanism, ProxyProducer>::type &
            call(ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
            static
            const typename boost::dataflow::get_proxied_consumer_type<Mechanism, ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
        };
    };
    
    template<typename Mechanism>
    struct get_proxied_consumer_impl<Mechanism, mutable_proxy_consumer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_consumer_type<
                    Mechanism,
                    ProxyProducer
                >::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {
            static
            typename boost::dataflow::get_proxied_consumer_type<Mechanism, ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_consumer();
            }
        };
    };

}

template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_consumer<Mechanism, T>,
    const T &
>::type
get_proxied_consumer(const T &t)
{
    return t;
}

template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_consumer<Mechanism, T>,
    T &
>::type
get_proxied_consumer(T &t)
{
    return t;
}

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_consumer<Mechanism, T>,
    typename extension::get_proxied_consumer_impl<Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template result<T>
>::type
get_proxied_consumer(T &t)
{
    return extension::get_proxied_consumer_impl<Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template apply<T>::call(t);
}

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_consumer<Mechanism, T>,
    typename extension::get_proxied_consumer_impl<
        Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template result<const T>
>::type
get_proxied_consumer(const T &t)
{
    return extension::get_proxied_consumer_impl<
        Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template apply<T>::call(t);
}

template<typename Mechanism, typename T>
struct consumer_category_of<Mechanism, T,
    typename boost::enable_if<is_proxy_consumer<Mechanism, T> >::type>
{
    typedef
        typename consumer_category_of<
            Mechanism,
            typename get_proxied_consumer_type<Mechanism, T>::type
        >::type type;
};

template<typename Mechanism, typename T>
struct consumed_type_of<Mechanism, T, typename boost::enable_if<is_proxy_consumer<Mechanism, T> >::type>
{
    typedef
        typename consumed_type_of<
            Mechanism,
            typename get_proxied_consumer_type<Mechanism, T>::type
        >::type type;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

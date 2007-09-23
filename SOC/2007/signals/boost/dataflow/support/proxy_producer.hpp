// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PROXY_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PROXY_PRODUCER_HPP

#include <boost/dataflow/support/producer.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {


template<typename Mechanism, typename T, typename Enable=void>
struct proxied_producer_of
{
};

template<typename Mechanism, typename T>
struct proxied_producer_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::proxy_producer_for >::type >
{
    typedef typename T::template dataflow<Mechanism>::proxy_producer_for type;
};

struct default_proxy_producer;
struct mutable_proxy_producer;

template<typename Mechanism, typename T, typename Enable=void>
struct proxy_producer_category_of
{
    typedef default_proxy_producer type;
};

template<typename Mechanism, typename T>
struct proxy_producer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::proxy_producer_category
    >::type >
{
    typedef typename T::template dataflow<Mechanism>::proxy_producer_category type;
};

// trait determining whether a type is a producer proxy.
template<typename Mechanism, typename T, typename Enable=void>
struct is_proxy_producer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_proxy_producer<Mechanism, T,
        typename detail::enable_if_defined<
            detail::all_of<
                typename proxied_producer_of<Mechanism, T>::type,
                typename proxy_producer_category_of<Mechanism, T>::type
            >
        >::type >
    : public boost::true_type {};

template<typename Mechanism, typename T, typename Enable=void>
struct get_proxied_producer_type
{
    typedef T type;
};

template<typename Mechanism, typename T>
struct get_proxied_producer_type<Mechanism, T, 
    typename boost::enable_if<is_proxy_producer<Mechanism, T> >::type>
{
    typedef
        typename get_proxied_producer_type<
            Mechanism,
            typename proxied_producer_of<Mechanism, T>::type
        >::type type;
};

namespace extension
{
    template<typename Mechanism, typename ProxyProducerTag>
    struct get_proxied_producer_impl
    {
        template<typename ProxyProducer>
        struct apply
        {
            static void call(const ProxyProducer &)
            {
                // Error: get_proxied_producer_impl has not been implemented
                // for ProxyProducerTag.
                BOOST_STATIC_ASSERT(sizeof(ProxyProducer)==0);
            }
        };
    };

    template<typename Mechanism>
    struct get_proxied_producer_impl<Mechanism, default_proxy_producer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_producer_type<
                    Mechanism,
                    ProxyProducer
                >::type & type;
        };
        
        template<typename ProxyProducer>
        struct result<const ProxyProducer>
        {
            typedef const typename
                boost::dataflow::get_proxied_producer_type<
                    Mechanism, 
                    ProxyProducer
                >::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {            
            static typename
            boost::dataflow::get_proxied_producer_type<Mechanism, ProxyProducer>::type &
            call(ProxyProducer &t)
            {
                return t.get_proxied_producer();
            }
            static
            const typename boost::dataflow::get_proxied_producer_type<Mechanism, ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return ProxyProducer::template dataflow<Mechanism>::get_proxied_producer(t);
            }
        };
    };
    
    template<typename Mechanism>
    struct get_proxied_producer_impl<Mechanism, mutable_proxy_producer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_producer_type<
                    Mechanism,
                    ProxyProducer
                >::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {
            static
            typename boost::dataflow::get_proxied_producer_type<Mechanism, ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return ProxyProducer::template dataflow<Mechanism>::get_proxied_producer(t);
            }
        };
    };

}

template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_producer<Mechanism, T>,
    const T &
>::type
get_proxied_producer(const T &t)
{
    return t;
}

template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_producer<Mechanism, T>,
    T &
>::type
get_proxied_producer(T &t)
{
    return t;
}

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_producer<Mechanism, T>,
    typename extension::get_proxied_producer_impl<Mechanism,
        typename proxy_producer_category_of<Mechanism, T>::type>::template result<T>
>::type
get_proxied_producer(T &t)
{
    return extension::get_proxied_producer_impl<Mechanism,
        typename proxy_producer_category_of<Mechanism, T>::type>::template apply<T>::call(t);
}

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_producer<Mechanism, T>,
    typename extension::get_proxied_producer_impl<
        Mechanism,
        typename proxy_producer_category_of<Mechanism, T>::type>::template result<const T>
>::type
get_proxied_producer(const T &t)
{
    return extension::get_proxied_producer_impl<
        Mechanism,
        typename proxy_producer_category_of<Mechanism, T>::type>::template apply<T>::call(t);
}

template<typename Mechanism, typename T>
struct producer_category_of<Mechanism, T,
    typename boost::enable_if<is_proxy_producer<Mechanism, T> >::type>
{
    typedef
        typename producer_category_of<
            Mechanism,
            typename get_proxied_producer_type<Mechanism, T>::type
        >::type type;
};

template<typename Mechanism, typename T>
struct produced_type_of<Mechanism, T, typename boost::enable_if<is_proxy_producer<Mechanism, T> >::type>
{
    typedef
        typename produced_type_of<
            Mechanism,
            typename get_proxied_producer_type<Mechanism, T>::type
        >::type type;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PROXY_PRODUCER_HPP

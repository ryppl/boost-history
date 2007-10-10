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

template<typename ProxiedConsumer>
struct proxy_consumer_category
{
    typedef ProxiedConsumer proxied_consumer;
};

template<typename T, typename Enable=void>
struct is_proxy_consumer_category : public boost::false_type
{};

template<typename T>
struct is_proxy_consumer_category<T,
    typename detail::enable_if_defined<
        typename T::proxied_consumer
    >::type >
    : public boost::true_type
{};

template<typename T>
struct default_proxy_consumer : public proxy_consumer_category<T>
{};

template<typename T>
struct mutable_proxy_consumer : public proxy_consumer_category<T>
{};

template<typename Mechanism, typename T, typename Enable=void>
struct proxy_consumer_category_of;

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
            typename proxy_consumer_category_of<Mechanism, T>::type
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
            typename proxy_consumer_category_of<Mechanism, T>::type::proxied_consumer
        >::type type;
};

template<typename Mechanism, typename T, typename Enable=void>
struct get_proxied_consumer_result_type;

template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_consumer<Mechanism, T>,
    T &
>::type
get_proxied_consumer(T &t)
{
    return t;
}

namespace extension
{
    template<typename Mechanism, typename ProxyConsumerTag>
    struct get_proxied_consumer_impl
    {
        template<typename ProxyConsumer>
        struct apply
        {
            static void call(const ProxyConsumer &)
            {
                // Error: get_proxied_consumer_impl has not been implemented
                // for ProxyConsumerTag.
                BOOST_STATIC_ASSERT(sizeof(ProxyConsumer)==0);
            }
        };
    };
    
    template<typename Mechanism, typename ProxiedConsumer>
    struct get_proxied_consumer_impl<Mechanism, default_proxy_consumer<ProxiedConsumer> >
    {
        template<typename ProxyConsumer>
        struct apply
        {
            typedef typename
                get_proxied_consumer_result_type<
                    Mechanism,
                    ProxiedConsumer
                >::type type;
            
            static type call(ProxyConsumer &t)
            {
                return get_proxied_consumer<Mechanism>(
                    ProxyConsumer::template dataflow<Mechanism>
                        ::get_proxied_consumer(t));
            }
        };
        template<typename ProxyConsumer>
        struct apply<const ProxyConsumer>
        {
            typedef const typename
                get_proxied_consumer_result_type<
                    Mechanism,
                    ProxiedConsumer
                >::type type;
            
            static type call(const ProxyConsumer &t)
            {
                return get_proxied_consumer<Mechanism>(
                    ProxyConsumer::template dataflow<Mechanism>
                        ::get_proxied_consumer(t));
            }
        };
    };
    
    template<typename Mechanism, typename ProxiedConsumer>
    struct get_proxied_consumer_impl<Mechanism, mutable_proxy_consumer<ProxiedConsumer> >
    {
        template<typename ProxyConsumer>
        struct apply
        {
            typedef typename
                get_proxied_consumer_result_type<
                    Mechanism,
                    ProxiedConsumer
                >::type type;
            
            static type call(const ProxyConsumer &t)
            {
                return get_proxied_consumer<Mechanism>(
                    ProxyConsumer::template dataflow<Mechanism>
                        ::get_proxied_consumer(t));
            }
        };
    };
}

template<typename Mechanism, typename T>
struct get_proxied_consumer_result_type<Mechanism, T,
    typename boost::disable_if<is_proxy_consumer<Mechanism, T> >::type>
{
    typedef T & type;
};

template<typename Mechanism, typename T>
struct get_proxied_consumer_result_type<Mechanism, T, 
    typename boost::enable_if<is_proxy_consumer<Mechanism, T> >::type>
{
    typedef
        typename extension::get_proxied_consumer_impl<
            Mechanism,
            typename proxy_consumer_category_of<Mechanism, T>::type
            >::template apply<T>::type type;
};

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_consumer<Mechanism, T>,
    typename extension::get_proxied_consumer_impl<Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template apply<T>
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
        typename proxy_consumer_category_of<Mechanism, T>::type>::template apply<const T>
>::type
get_proxied_consumer(const T &t)
{
    return extension::get_proxied_consumer_impl<
        Mechanism,
        typename proxy_consumer_category_of<Mechanism, T>::type>::template apply<const T>::call(t);
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

} } // namespace boost::dataflow

#define DATAFLOW_PROXY_CONSUMER_CATEGORY(m,p,pc) \
template<> \
struct proxy_consumer_category_of<m, p> \
{ \
    typedef pc type; \
};

#define DATAFLOW_PROXY_CONSUMER_CATEGORY_ENABLE_IF(m,P,Cond,pc) \
template<typename P> \
struct proxy_consumer_category_of<m, P, \
    typename boost::enable_if< Cond >::type> \
{ \
    typedef pc type; \
};

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

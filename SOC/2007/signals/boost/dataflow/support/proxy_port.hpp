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
#include <boost/type_traits/remove_cv.hpp>


namespace boost { namespace dataflow {

template<typename ProxiedProducer>
struct proxy_producer_category
{
    typedef ProxiedProducer proxied_producer;
};

template<typename T, typename Enable=void>
struct is_proxy_producer_category : public boost::false_type
{};

template<typename T>
struct is_proxy_producer_category<T,
    typename detail::enable_if_defined<
        typename T::proxied_producer
    >::type >
    : public boost::true_type
{};

template<typename T>
struct default_proxy_producer : public proxy_producer_category<T>
{};

template<typename T>
struct mutable_proxy_producer : public proxy_producer_category<T>
{};

template<typename Mechanism, typename T, typename Enable=void>
struct proxy_producer_category_of;

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
            typename proxy_producer_category_of<
                Mechanism,
                typename boost::remove_cv<T>::type
            >::type
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
            typename proxy_producer_category_of<Mechanism, T>::type::proxied_producer
        >::type type;
};

template<typename Mechanism, typename T, typename Enable=void>
struct get_proxied_producer_result_type;


template<typename Mechanism, typename T>
typename boost::disable_if<
    is_proxy_producer<Mechanism, T>,
    T &
>::type
get_proxied_producer(T &t)
{
    return t;
}

namespace extension
{
    template<typename Mechanism, typename ProxyProducerTag>
    struct get_proxied_producer_impl
    {
        template<typename ProxyProducer>
        struct apply
        {
            static void call(ProxyProducer &p)
            {
                // Error: get_proxied_producer_impl has not been implemented
                // for ProxyProducerTag.
                BOOST_STATIC_ASSERT(sizeof(ProxyProducer)==0);
            }
        };
    };
    
    template<typename Mechanism, typename ProxiedProducer>
    struct get_proxied_producer_impl<Mechanism, default_proxy_producer<ProxiedProducer> >
    {
        template<typename ProxyProducer>
        struct apply
        {
            typedef typename
                get_proxied_producer_result_type<
                    Mechanism,
                    ProxiedProducer
                >::type type;
            
            static type call(ProxyProducer &t)
            {
                return get_proxied_producer<Mechanism>(
                    ProxyProducer::template dataflow<Mechanism>
                        ::get_proxied_producer(t));
            }
        };
        template<typename ProxyProducer>
        struct apply<const ProxyProducer>
        {
            typedef const typename
                get_proxied_producer_result_type<
                    Mechanism,
                    ProxiedProducer
                >::type type;
            
            static type call(const ProxyProducer &t)
            {
                return get_proxied_producer<Mechanism>(
                    ProxyProducer::template dataflow<Mechanism>
                        ::get_proxied_producer(t));
            }
        };
    };
    
    template<typename Mechanism, typename ProxiedProducer>
    struct get_proxied_producer_impl<Mechanism, mutable_proxy_producer<ProxiedProducer> >
    {
        template<typename ProxyProducer>
        struct apply
        {
            typedef typename
                get_proxied_producer_result_type<
                    Mechanism,
                    ProxiedProducer
                >::type type;
            
            static type call(const ProxyProducer &t)
            {
                return get_proxied_producer<Mechanism>(
                    ProxyProducer::template dataflow<Mechanism>
                        ::get_proxied_producer(t));
            }
        };
    };
}

template<typename Mechanism, typename T>
struct get_proxied_producer_result_type<Mechanism, T,
    typename boost::disable_if<is_proxy_producer<Mechanism, T> >::type>
{
    typedef T & type;
};

template<typename Mechanism, typename T>
struct get_proxied_producer_result_type<Mechanism, T, 
    typename boost::enable_if<is_proxy_producer<Mechanism, T> >::type>
{
    typedef
        typename extension::get_proxied_producer_impl<
            Mechanism,
            typename proxy_producer_category_of<Mechanism, T>::type
            >::template apply<T>::type type;
};

template<typename Mechanism, typename T>
typename boost::lazy_enable_if<
    is_proxy_producer<Mechanism, T>,
    typename extension::get_proxied_producer_impl<Mechanism,
        typename proxy_producer_category_of<
            Mechanism,
            typename boost::remove_cv<T>::type
        >::type
    >::template apply<T>
>::type
get_proxied_producer(T &t)
{
    return
        extension::get_proxied_producer_impl<
            Mechanism,
            typename proxy_producer_category_of<Mechanism, T>::type
        >::template apply<T>::call(t);
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

} } // namespace boost::dataflow

#define DATAFLOW_PROXY_PRODUCER_CATEGORY(m,p,pc) \
template<> \
struct proxy_producer_category_of<m, p> \
{ \
    typedef pc type; \
};

#define DATAFLOW_PROXY_PRODUCER_CATEGORY_ENABLE_IF(m,P,Cond,pc) \
template<typename P> \
struct proxy_producer_category_of<m, P, \
    typename boost::enable_if< Cond >::type> \
{ \
    typedef pc type; \
};

#endif // BOOST_DATAFLOW_SUPPORT_PROXY_PRODUCER_HPP

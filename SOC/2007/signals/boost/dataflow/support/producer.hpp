// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
//#include <boost/dataflow/support/proxy.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

template<typename T, typename Enable=void>
struct proxied_producer_of
{
};

template<typename T>
struct proxied_producer_of<T,
typename detail::enable_if_defined<typename T::proxy_producer_for >::type >
{
    typedef typename T::proxy_producer_for type;
};

struct default_proxy_producer;
struct mutable_proxy_producer;

template<typename T, typename Enable=void>
struct proxy_producer_category_of
{
    typedef default_proxy_producer type;
};

template<typename T>
struct proxy_producer_category_of<T,
typename detail::enable_if_defined<typename T::proxy_producer_category >::type >
{
    typedef typename T::proxy_producer_category type;
};

// trait determining whether a type is a producer proxy.
template<typename T, typename Enable=void>
struct is_proxy_producer
    : public boost::false_type {};

template<typename T>
struct is_proxy_producer<T,
        typename detail::enable_if_defined<
            detail::all_of<
                typename proxied_producer_of<T>::type,
                typename proxy_producer_category_of<T>::type
            >
        >::type >
    : public boost::true_type {};

template<typename T, typename Enable=void>
struct get_proxied_producer_type
{
    typedef T type;
};

template<typename T>
struct get_proxied_producer_type<T, 
    typename boost::enable_if<is_proxy_producer<T> >::type>
{
    typedef
        typename get_proxied_producer_type<
            typename proxied_producer_of<T>::type
        >::type type;
};

namespace extension
{
    template<typename ProxyProducerTag>
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

    template<>
    struct get_proxied_producer_impl<default_proxy_producer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_producer_type<ProxyProducer>::type & type;
        };
        
        template<typename ProxyProducer>
        struct result<const ProxyProducer>
        {
            typedef const typename
                boost::dataflow::get_proxied_producer_type<ProxyProducer>::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {            
            static typename
            boost::dataflow::get_proxied_producer_type<ProxyProducer>::type &
            call(ProxyProducer &t)
            {
                return t.get_proxied_producer();
            }
            static
            const typename boost::dataflow::get_proxied_producer_type<ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_producer();
            }
        };
    };
    
    template<>
    struct get_proxied_producer_impl<mutable_proxy_producer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef typename
                boost::dataflow::get_proxied_producer_type<ProxyProducer>::type & type;
        };

        template<typename ProxyProducer>
        struct apply
        {
            static
            typename boost::dataflow::get_proxied_producer_type<ProxyProducer>::type &
            call(const ProxyProducer &t)
            {
                return t.get_proxied_producer();
            }
        };
    };

}

template<typename T>
typename boost::disable_if<
    is_proxy_producer<T>,
    const T &
>::type
get_proxied_producer(const T &t)
{
    return t;
}

template<typename T>
typename boost::disable_if<
    is_proxy_producer<T>,
    T &
>::type
get_proxied_producer(T &t)
{
    return t;
}

template<typename T>
typename boost::lazy_enable_if<
    is_proxy_producer<T>,
    typename extension::get_proxied_producer_impl<
        typename proxy_producer_category_of<T>::type>::template result<T>
>::type
get_proxied_producer(T &t)
{
    return extension::get_proxied_producer_impl<
        typename proxy_producer_category_of<T>::type>::template apply<T>::call(t);
}

template<typename T>
typename boost::lazy_enable_if<
    is_proxy_producer<T>,
    typename extension::get_proxied_producer_impl<
        typename proxy_producer_category_of<T>::type>::template result<const T>
>::type
get_proxied_producer(const T &t)
{
    return extension::get_proxied_producer_impl<
        typename proxy_producer_category_of<T>::type>::template apply<T>::call(t);
}

// trait giving the producer category of a type.
template<typename T, typename Enable=void>
struct producer_category_of
{
};

template<typename T>
struct producer_category_of<T,
typename detail::enable_if_defined<typename T::producer_category >::type >
{
    typedef typename T::producer_category type;
};

template<typename T>
struct producer_category_of<T,
    typename boost::enable_if<is_proxy_producer<T> >::type>
{
    typedef
        typename producer_category_of<
            typename get_proxied_producer_type<T>::type
        >::type type;
};

template<typename T, typename Enable=void>
struct produced_type_of
{
};

template<typename T>
struct produced_type_of<T, typename boost::enable_if<is_proxy_producer<T> >::type>
{
    typedef
        typename produced_type_of<
            typename get_proxied_producer_type<T>::type
        >::type type;
};

// trait determining whether a type is a producer.
template<typename T, typename Enable=void>
struct is_producer
    : public boost::false_type {};

template<typename T>
struct is_producer<T, 
        typename detail::enable_if_defined<detail::all_of<
            typename producer_category_of<T>::type,
            typename produced_type_of<T>::type
        > >::type >
    : public boost::true_type {};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

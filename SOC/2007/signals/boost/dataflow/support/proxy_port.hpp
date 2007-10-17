// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PROXY_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_PROXY_PORT_HPP

#include <boost/dataflow/support/port.hpp>
#include <boost/dataflow/utility/copy_cv.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/static_assert.hpp>

namespace boost { namespace dataflow {

template<typename Mechanism, typename PortCategory>
struct proxy_port_traits
{
    typedef Mechanism mechanism;
    typedef PortCategory category;
};

template<typename T, typename Enable=void>
struct is_proxy_port_traits : public mpl::false_
{};

template<typename T>
struct is_proxy_port_traits<T,
    typename detail::enable_if_defined<
        detail::all_of<
            typename T::mechanism,
            typename T::category
        >
    >::type >
    : public mpl::true_
{};

template<typename Mechanism, typename PortCategory, typename T, typename GetProxiedObject>
struct default_proxy_port : public proxy_port_traits<Mechanism, PortCategory>
{};

template<typename Mechanism, typename PortCategory, typename T>
struct mutable_proxy_port : public proxy_port_traits<Mechanism, PortCategory>
{};

template<typename Mechanism, typename PortCategory, typename T, typename Enable=void>
struct proxy_port_traits_of;

template<typename Mechanism, typename PortCategory, typename T>
struct proxy_port_traits_of<Mechanism, PortCategory, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism, PortCategory>::proxy_port_traits
    >::type >
{
    typedef typename T::template dataflow<Mechanism, PortCategory>::proxy_port_traits type;
};

/// Specialization allowing intrusive specification of the PortTraits.
template<typename Mechanism, typename PortCategory, typename T>
struct proxy_port_traits_of<Mechanism, PortCategory, T,
    typename enable_if<
        mpl::and_<
            mpl::not_<mpl::is_sequence<typename T::proxy_port_traits> >,
            is_same<Mechanism, typename T::proxy_port_traits::mechanism>,
            is_same<PortCategory, typename T::proxy_port_traits::category>
        >
    >::type
>
{
    typedef typename T::proxy_port_traits type;
    BOOST_MPL_ASSERT(( is_proxy_port_traits<type> ));
};

} }

/// Specialization allowing intrusive specification of a sequence of PortTraits.
#include <boost/dataflow/support/port/detail/proxy_port_traits_sequence.hpp>

namespace boost { namespace dataflow {

template<typename Mechanism, typename PortCategory, typename T>
struct is_proxy_port<Mechanism, PortCategory, T,
        typename detail::enable_if_defined<
            typename proxy_port_traits_of<
                Mechanism,
                PortCategory,
                typename boost::remove_cv<T>::type
            >::type
        >::type >
    : public mpl::true_ {};

template<typename Mechanism, typename PortCategory, typename T>
typename boost::enable_if<
    is_proxy_port<Mechanism, PortCategory, T>,
    typename get_port_result_type<Mechanism, PortCategory, T>::type
>::type
get_port(T &t)
{
    return
        extension::get_port_impl<
            typename proxy_port_traits_of<Mechanism, PortCategory, T>::type
        >::template apply<T>::call(t);
}

namespace extension
{
    template<typename Mechanism, typename PortCategory, typename ProxiedPort, typename GetProxiedObject>
    struct get_port_impl<default_proxy_port<Mechanism, PortCategory, ProxiedPort, GetProxiedObject> >
    {
        template<typename ProxyPort>
        struct apply
        {
            typedef typename
                get_port_result_type<
                    Mechanism,
                    PortCategory,
                    typename utility::copy_cv<ProxiedPort,ProxyPort>::type
                >::type type;
            
            static type call(ProxyPort &t)
            {
                return  get_port<Mechanism, PortCategory>(GetProxiedObject()(t));
            }
        };
    };
    
    template<typename Mechanism, typename PortCategory, typename ProxiedPort>
    struct get_port_impl<mutable_proxy_port<Mechanism, PortCategory, ProxiedPort> >
    {
        template<typename ProxyPort>
        struct apply
        {
            typedef typename
                get_port_result_type<
                    Mechanism,
                    PortCategory,
                    ProxiedPort
                >::type type;
            
            static type call(const ProxyPort &t)
            {
                return get_port<Mechanism, PortCategory>(
                    ProxyPort::template dataflow<Mechanism, PortCategory>
                        ::get_proxied_port(t));
            }
        };
    };
}

template<typename Mechanism, typename PortCategory, typename T>
struct get_port_result_type<
    Mechanism,
    PortCategory,
    T,
    typename enable_if<is_proxy_port<Mechanism, PortCategory, T> >::type>
{
    typedef
        typename
            extension::get_port_impl<
                typename proxy_port_traits_of<
                    Mechanism,
                    PortCategory,
                    typename remove_cv<T>::type
                >::type
            >::template apply<T>::type type;
};

template<typename Mechanism, typename PortCategory, typename T>
struct port_traits_of<Mechanism, T,
    typename boost::enable_if<is_proxy_port<Mechanism, PortCategory, T> >::type>
{
    typedef
        typename port_traits_of<
            Mechanism,
            PortCategory,
            typename remove_cv<
                typename remove_reference<
                    typename get_port_result_type<Mechanism, PortCategory, T>::type
                >::type
            >::type
        >::type type;
};

} } // namespace boost::dataflow

#define DATAFLOW_PROXY_PORT_CATEGORY(ProxyPort,ProxyPortCategory) \
template<> \
struct proxy_port_traits_of< \
    typename ProxyPortCategory::mechanism, \
    typename ProxyPortCategory::category, \
    ProxyPort> \
{ \
    typedef ProxyPortCategory type; \
};

#define DATAFLOW_PROXY_PORT_CATEGORY_ENABLE_IF(P,Cond,ProxyPortCategory) \
template<typename P> \
struct proxy_port_traits_of< \
    typename ProxyPortCategory::mechanism, \
    typename ProxyPortCategory::category, \
    P, \
    typename boost::enable_if< Cond >::type> \
{ \
    typedef ProxyPortCategory type; \
};

#endif // BOOST_DATAFLOW_SUPPORT_PROXY_PORT_HPP

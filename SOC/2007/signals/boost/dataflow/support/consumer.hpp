// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP
#define BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/common.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

namespace concepts
{
    struct consumer;
}

template<typename ConsumerConcept, typename OutConnectionsStored=connections::none>
struct consumer_category
{
    typedef ConsumerConcept consumer_concept;
    typedef OutConnectionsStored in_connections_stored;
};

template<typename ConsumerCategory, typename Enable=void>
struct is_consumer_category : public boost::false_type
{};

template<typename ConsumerCategory>
struct is_consumer_category<ConsumerCategory,
    typename detail::enable_if_defined<detail::all_of<
        typename ConsumerCategory::in_connections_stored,
        typename ConsumerCategory::consumer_concept
    > >::type>
 : public boost::mpl::true_
{};

/// Trait giving the consumer category of a type.
template<typename Mechanism, typename T, typename Enable=void>
struct consumer_category_of
{};

/// Allows intrusive specification of the consumer category.
template<typename Mechanism, typename T>
struct consumer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::consumer_category>::type
    >
{
    typedef typename T::template dataflow<Mechanism>::consumer_category type;
    BOOST_MPL_ASSERT(( is_consumer_category<type> ));
};

/// Trait determining whether a type is a Consumer.
template<typename Mechanism, typename T, typename Enable=void>
struct is_consumer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_consumer<Mechanism, T,
        typename detail::enable_if_defined<
            typename consumer_category_of<Mechanism, T>::type
        >::type >
    : public boost::true_type {};

template<typename Mechanism, typename ConsumerTag>
struct consumer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ConsumerTag consumer_category;
    };
};

template<typename ConsumerTag>
struct consumer<all_mechanisms, ConsumerTag>
{
    template<typename M>
    struct dataflow
    {
        typedef ConsumerTag consumer_category;
    };
};

} } // namespace boost::dataflow

#define DATAFLOW_CONSUMER_CATEGORY(m,p,pc) \
template<> \
struct consumer_category_of<m, p> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( is_consumer_category<type> )); \
};

#define DATAFLOW_CONSUMER_CATEGORY_ENABLE_IF(m,P,Cond,pc) \
template<typename P> \
struct consumer_category_of<m, P, typename boost::enable_if< Cond >::type> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( is_consumer_category<type> )); \
};

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

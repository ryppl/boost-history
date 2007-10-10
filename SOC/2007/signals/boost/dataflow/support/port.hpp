// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/common.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

namespace concepts
{
    /// Tag
    struct producer;
    struct consumer;
}

namespace port
{
    struct producer;
    struct consumer;
}

/*/// Convenience class for ProducerCategory types.
template<typename ProducerConcept, typename OutConnectionsStored=connections::none>
struct producer_category
{
    typedef ProducerConcept producer_concept;
    typedef OutConnectionsStored out_connections_stored;
};*/

/// Trait determining adherence to the ProducerCategory concept.
template<typename PortCategory, typename Enable=void>
struct is_port_category : public boost::false_type
{};

template<typename PortCategory>
struct is_port_category<PortCategory,
    typename detail::enable_if_defined<
        typename ProducerCategory::port_concept
    >::type>
 : public boost::mpl::true_
{};

/// Trait returning the producer category of a type.
template<typename Mechanism, typename Port, typename T, typename Enable=void>
struct port_category_of
{};

/// Allows intrusive specification of the producer category.
template<typename Mechanism, typename Port, typename T>
struct port_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::port<Port>::category >::type
    >
{
    typedef typename T::template dataflow<Mechanism>::port<Port>::category type;
    BOOST_MPL_ASSERT(( is_port_category<type> ));
};

/// Trait determining adherence to the Producer concept.
template<typename Mechanism, typename Port, typename T, typename Enable=void>
struct is_port
    : public boost::false_type {};

template<typename Mechanism, typename Port, typename T>
struct is_producer<Mechanism, Port, T,
        typename detail::enable_if_defined<
            typename port_category_of<Mechanism, Port, T>::type
        >::type >
    : public boost::true_type {};

/// Convenience class for Producer types for a specific Mechanism. 
template<typename Mechanism, typename ProducerTag>
struct producer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ProducerTag producer_category;
    };
};

/// Convenience class for Producer types for all Mechanisms. 
template<typename ProducerTag>
struct producer<all_mechanisms, ProducerTag>
{
    template<typename M>
    struct dataflow
    {
        typedef ProducerTag producer_category;
    };
};

} } // namespace boost::dataflow

/// Macro simplifying non-intrusive specification of a type's producer category.
#define DATAFLOW_PRODUCER_CATEGORY(m,p,pc) \
template<> \
struct producer_category_of<m, p> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( is_producer_category<type> )); \
};

/// Macro simplifying non-intrusive specification of multiple types'
/// producer category, using a boost::enable_if condition.
#define DATAFLOW_PRODUCER_CATEGORY_ENABLE_IF(m,P,Cond,pc) \
template<typename P> \
struct producer_category_of<m, P, typename boost::enable_if< Cond >::type> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( is_producer_category<type> )); \
};

#endif // BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

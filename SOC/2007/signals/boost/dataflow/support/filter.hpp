// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FILTER_HPP
#define BOOST_DATAFLOW_SUPPORT_FILTER_HPP

#include <boost/dataflow/support/producer.hpp>
#include <boost/dataflow/support/consumer.hpp>

namespace boost { namespace dataflow {

// trait giving the producer category of a type.
template<typename Mechanism, typename T, typename Enable=void>
struct filter_category_of
{
};

template<typename Mechanism, typename T>
struct filter_category_of<Mechanism, T,
    typename detail::enable_if_defined<typename T::template dataflow<Mechanism>::filter_category>::type >
{
    typedef typename T::template dataflow<Mechanism>::filter_category type;
};

template<typename Mechanism, typename T>
struct producer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename filter_category_of<Mechanism, T>::type
    >::type >
{
    typedef typename filter_category_of<Mechanism, T>::type type;
};

template<typename Mechanism, typename T>
struct consumer_category_of<Mechanism, T,
    typename detail::enable_if_defined<typename filter_category_of<Mechanism, T>::type>::type >
{
    typedef typename filter_category_of<Mechanism, T>::type type;
};

} } // namespace boost::dataflow

#define DATAFLOW_FILTER_CATEGORY(m,p,pc) \
template<> \
struct filter_category_of<m, p> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( is_producer_category<type> )); \
    BOOST_MPL_ASSERT(( is_consumer_category<type> )); \
};

#define DATAFLOW_FILTER_CATEGORY_ENABLE_IF(m,P,Cond,pc) \
template<typename P> \
struct filter_category_of<m, P, typename boost::enable_if< Cond >::type> \
{ \
    typedef pc type; \
    BOOST_MPL_ASSERT(( mpl::and_<is_producer_category<type>,is_consumer_category<type> > )); \
};

#endif // BOOST_DATAFLOW_SUPPORT_FILTER_HPP

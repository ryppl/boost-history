// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_HPP
#define BOOST_DATAFLOW_SUPPORT_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/dataflow/detail/enable_if_defined.hpp>

namespace boost { namespace dataflow {

namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(producer_category)
    
    BOOST_MPL_HAS_XXX_TRAIT_DEF(consumer_category)

    BOOST_MPL_HAS_XXX_TRAIT_DEF(produced_type)
}

// contains mechanism tags
namespace mechanism
{
    
}

// returns the underlying mechanism associated with a type.
template<typename T>
struct mechanism_of;

// trait giving the producer category of a type.
template<typename T, typename Enable=void>
struct producer_category_of;

template<typename T>
struct producer_category_of<T,
typename boost::enable_if<detail::has_producer_category<T> >::type >
{
    typedef typename T::producer_category type;
};

// trait giving the consumer category of a type.
template<typename T, typename Enable=void>
struct consumer_category_of;

template<typename T>
struct consumer_category_of<T,
typename boost::enable_if<detail::has_consumer_category<T> >::type >
{
    typedef typename T::consumer_category type;
};

// trait determining whether a type is a producer.
template<typename T, typename Enable=void>
struct is_producer
    : public boost::false_type {};

template<typename T>
struct is_producer<T, 
        typename detail::enable_if_defined<producer_category_of<T> >::type >
    : public boost::true_type {};

// trait determining whether a type is a consumer.
template<typename T, typename Enable=void>
struct is_consumer
    : public boost::false_type {};

template<typename T>
struct is_consumer<T,
        typename detail::enable_if_defined<consumer_category_of<T> >::type >
    : public boost::true_type {};

template<typename T, typename Enable=void>
struct produced_type_of;

template<typename T>
struct produced_type_of<T,
typename boost::enable_if<detail::has_produced_type<T> >::type >
{
    typedef typename T::produced_type type;
};

namespace extension
{
    template<typename ProducerTag, typename ConsumerTag, typename Enable=void>
    struct connect_impl
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            
        };
    };
}

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<Producer,Consumer>
                ::call(producer,consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<Producer,Consumer>
                ::call(producer,consumer);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_HPP

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
#define SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP

#include <boost/dataflow/support.hpp>
#include <boost/dataflow/signal/connection/detail/bind_object.hpp>
#include <boost/dataflow/signal/connection/detail/result_of_defined.hpp>
#include <boost/dataflow/signal/connection/detail/replace_return_type.hpp>
#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/signal.hpp>

#include <boost/mpl/aux_/has_type.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename T>
    struct get_signature;
    
    template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
    struct get_signature<boost::signal<Signature, Combiner, Group, GroupCompare> >
    {
        typedef Signature type;
    };
}

} } // namespace boost::signals

namespace boost { namespace dataflow {

struct signal_producer {};
struct signal_consumer {};
struct signals_mechanism;

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct producer_category_of<signals_mechanism, boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    typedef signal_producer type;
};

namespace extension
{
    namespace signals
    {
        template<class T, typename Enable=void>
        struct get_signal;
        
        template<typename T, typename Enable=void>
        struct get_signal_type;
        
        template<typename Signature, typename T, typename Enable=void>
        struct get_slot;
        
        /** \brief Support for boost::signal.
            */
        template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
        struct get_signal<boost::signal<Signature, Combiner, Group, GroupCompare> >
        {
            boost::signal<Signature, Combiner, Group, GroupCompare> & operator()
            (boost::signal<Signature, Combiner, Group, GroupCompare> &s) {return s;}
        };
        
        /** \brief Support for boost::signal.
            */
        template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
        struct get_signal_type<boost::signal<Signature, Combiner, Group, GroupCompare> >
        {
            typedef boost::signal<Signature, Combiner, Group, GroupCompare> type;
        };
        
        /** \brief Support for function objects that for which boost::result_of is defined.
            */
        template<typename Signature, typename T>
        struct get_slot<Signature, T, typename boost::enable_if<boost::signals::detail::result_of_defined<
        typename boost::signals::detail::replace_return_type<Signature, T>::type > >::type>
        {
            boost::function<Signature> operator()(T &object)
            {
                return boost::signals::detail::bind_object<Signature, T>()
                (static_cast<typename boost::signals::detail::slot_type<Signature, T>::type>(&T::operator()), object);
            }
            boost::function<Signature> operator()(const T &object)
            {
                return boost::signals::detail::bind_object<Signature, T>()
                (static_cast<typename boost::signals::detail::slot_type<Signature, T>::type>(&T::operator()), object);
            }
        };
        
        /** \brief Support for function objects that for which boost::result_of is defined.
            */
        /*template<typename T>
        struct consumer_category_of<signals_mechanism, T, boost::enable_if<boost::signals::detail::result_of_defined<
        typename boost::signals::detail::replace_return_type<Signature, T>::type > >::type>
        {
            typedef boost::dataflow::signal_consumer type;
        };*/
    }
    
    template<typename ProducerTag, typename ConsumerTag>
    struct connect_impl<signals_mechanism, ProducerTag, ConsumerTag,
        typename boost::enable_if<boost::mpl::and_<
            boost::is_base_of<signal_producer, ProducerTag>,
            boost::is_base_of<signal_consumer, ConsumerTag>
        > >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                signals::get_signal<Producer>()(producer).connect(
                     signals::get_slot<typename dataflow::produced_type_of<
                         signals_mechanism, Producer>::type, Consumer>()(consumer));
            }
            static void call(const Producer &producer, Consumer &consumer)
            {
                signals::get_signal<Producer>()(producer).connect(
                     signals::get_slot<typename dataflow::produced_type_of<
                         signals_mechanism, Producer>::type, Consumer>()(consumer));
           }
            static void call(Producer &producer, const Consumer &consumer)
            {
                signals::get_signal<Producer>()(producer).connect(
                     signals::get_slot<typename dataflow::produced_type_of<
                         signals_mechanism, Producer>::type, Consumer>()(consumer));
            }
            static void call(Producer &producer, Consumer &consumer)
            {
                signals::get_signal<Producer>()(producer).connect(
                     signals::get_slot<typename dataflow::produced_type_of<
                         signals_mechanism, Producer>::type, Consumer>()(consumer));
            }
        };
    };
}

/*
template<typename T>
struct produced_type_of<
    T,
    typename boost::enable_if<
        boost::is_base_of<
            signal_producer,
            typename producer_category_of<T>::type
        >
    >::type >
{
    typedef typename boost::signals::detail::get_signature<typename extension::signals::get_signal_type<T>::type>::type type;
};*/

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct produced_type_of<
    signals_mechanism, 
    boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    typedef Signature type;
};

} } // namespace boost::dataflow

namespace boost { namespace signals {
    
template<typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::signals_mechanism>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::signals_mechanism>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, const Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::signals_mechanism>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, const Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::signals_mechanism>(producer, consumer);
}
    
} } // namespace boost::phoenix

#endif // SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
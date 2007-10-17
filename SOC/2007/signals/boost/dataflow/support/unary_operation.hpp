// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_DISCONNECT_ALL_OUTS_HPP
#define BOOST_DATAFLOW_SUPPORT_DISCONNECT_ALL_OUTS_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>


namespace boost { namespace dataflow {

namespace extension
{
    template<typename Mechanism, typename ProducerTag, typename Enable=void>
    struct disconnect_all_outs_impl
    {
        template<typename Producer>
        struct apply
        {
            struct detail
            {
                typedef void disconnect_all_outs_impl_unspecialized;
            };

            static void call(Producer &)
            {
                // Error: disconnect_all_outs_impl has not been implemented for
                // ProducerTag.
                BOOST_STATIC_ASSERT(sizeof(Producer)==0);
            }
        };
    };
}

template<typename Mechanism, typename T, typename Enable=void>
struct is_out_disconnectable
    : public boost::true_type {};

template<typename Mechanism, typename Producer>
struct is_out_disconnectable<
    Mechanism,
    Producer,
    typename detail::enable_if_defined<
        typename extension::disconnect_all_outs_impl<
            Mechanism,
            typename producer_category_of<Mechanism, Producer>::type
        >::detail::template apply<Producer>::disconnect_all_outs_impl_unspecified
    >::type
>
    : public boost::false_type {};

template<typename Mechanism, typename Producer>
void disconnect_all_outs(Producer &producer)
{
    extension::disconnect_all_outs_impl<
        Mechanism,
        typename producer_category_of<Mechanism, Producer>::type
    >::template apply<Producer>::call(producer);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_DISCONNECT_ALL_OUTS_HPP

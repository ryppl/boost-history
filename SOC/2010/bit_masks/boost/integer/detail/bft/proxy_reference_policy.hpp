//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_PROXY_REFERENCE_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_PROXY_REFERENCE_POLICY_HPP
#include <boost/type_traits/is_same.hpp>
#include <boost/bitfield/bitfield_orignal.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace detail {

/** Wraps up a policy so that it has a similar interface to that of bitfield.
 *  which basically means that since the policy itself only has static functions
 *  this is the thing of which copies are made and is also responsible for the
 *  keeping of a reference to the original data.
 */
template <
    typename StorageType,
    typename Policy
>
struct policy_wrapper {
    typedef StorageType                         storage_type;
    typedef Policy                              field_policy;
    typedef typename field_policy::value_type   value_type;

    policy_wrapper(storage_type& storage)
        :_field( storage )
    { }

    void set(value_type rhs) {
        _field = field_policy::template apply<storage_type>::set(_field, rhs);
    }

    value_type get() const {
        return field_policy::template apply<storage_type>::get( _field );
    }

private:
    storage_type& _field;
};



/** This is the policy which is going to be used for calculating the correct
 *  policy for retrieving the data from inside of teh bitfield_tuple
 *  this is instead of the bitfield being the policy.
 */
template <
    typename StorageType,
    typename Offset,
    typename Width,
    typename ValueType,
    typename Policy
>
struct select_packing_policy {
    // this is where the correct storage and retrieval policy is
    // selected based on the arguments supplied.
    typedef typename mpl::if_<
        is_same<
            Policy,
            mpl::void_
        >,
        typename mpl::if_<
            is_pod<StorageType>,
                ::boost::integer::bitfield_policy<
                    StorageType,
                    Offset::value,
                    Offset::value
                      +
                    Width::value - 1,
                    ValueType
                >,

                ::boost::integer::bitfield<
                    StorageType,
                    Offset::value,
                    Offset::value
                      +
                    Width::value - 1,
                    ValueType
                >
        >::type,
        policy_wrapper< StorageType, Policy >
    >::type                         type;
};


}} // end boost::detial

#endif

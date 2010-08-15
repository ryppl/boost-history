//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_REFERENCE_BUILDER_HPP
#define BOOST_BITFIELD_TUPLE_REFERENCE_BUILDER_HPP
#include <boost/static_assert.hpp>



/** The meta functions contained within this file are meant to aid in the 
 *  locating the correct value within member and deducing the correct constness
 *  and returning the correct reference type which will allow for only one
 *  reference type class within bitfield_tuple.
 */
namespace boost { namespace bitfields{ namespace detail {

template <typename BitfieldTuple, typename Name>
struct disable_if_proxy_reference_type_by_name {
    // search for the name,
    typedef typename mpl::find_if<
        typename BitfieldTuple::members,
        detail::match_name<
            mpl::_1,
            Name
        >
    >::type                             element_iter;

    // get the end iterator from members.
    typedef typename mpl::end<
        typename BitfieldTuple::members
    >::type                             member_end;

    // create the bitfield_reference type that will be returned if
    // disable_if is enabled.
    typedef typename BitfieldTuple::template bitfield_reference<
            typename mpl::if_<
                is_const<BitfieldTuple>,
                typename add_const<
                    typename mpl::deref<
                        element_iter
                    >::type
                >::type,
                typename mpl::deref<
                    element_iter
                >::type
            >::type
        >                               reference_type;

    // make disable_if statement.
    typedef typename disable_if<
        is_same <
            element_iter,
            member_end
        >,
        reference_type
    >::type                             type;
};


template <typename BitfieldTuple, std::size_t Index>
struct enable_if_proxy_reference_type_by_index {
    // check to make sure index is valid
    typedef typename mpl::less<
        mpl::size_t<
            Index
        >,
        mpl::size_t<
            mpl::size<
                typename BitfieldTuple::members
            >::type::value
        >
    >                   is_valid_index;

    // get the bitfield_element from members.
    typedef typename mpl::at_c<
        typename BitfieldTuple::members,
        Index
    >::type                     bft_element_t;

    // create the reference type
    typedef typename BitfieldTuple::template bitfield_reference<
        typename mpl::if_<
            is_const<BitfieldTuple>,
            typename add_const<bft_element_t>::type,
            bft_element_t
        >::type
    >                   reference_type;

    // return the reference type if its valid.
    typedef typename enable_if<
        is_valid_index,
        reference_type
    >::type                 type;
};





}}} // end boost::bitfields::detail

#endif

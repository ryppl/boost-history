//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BFT_INTERFACE_META_FUNCITONS_HPP
#define BOOST_BFT_INTERFACE_META_FUNCITONS_HPP

namespace boost { namespace bitfields {

/** Checks to see if a name exists.
 *  returns mpl::true_ or mpl::false_
 */
template <typename BitfieldTuple, typename Name>
struct name_exists {
    typedef typename mpl::not_<
        is_same <
            typename ::boost::mpl::find_if<
                typename BitfieldTuple::members,
                detail::match_name<
                    mpl::_1,
                    Name
                >
            >::type,
            typename mpl::end<
                typename BitfieldTuple::members
            >::type
        >
    >::type                     type;
};

/** Returns an element by which has a given name. */
template <typename BitfieldTuple, typename Name>
struct find_by_element_name {

    BOOST_STATIC_ASSERT(( name_exists<BitfieldTuple,Name>::type::value ));

    typedef typename mpl::deref<
        typename mpl::find_if<
            typename BitfieldTuple::members,
            detail::match_name<
                mpl::_1,
                Name
            >
        >::type
    >::type                 type;
};

/** Returns an element from members within bitfield_tuple
 *  at a given index.
 */
template <typename BitfieldTuple, std::size_t Index>
struct find_by_element_index {

    BOOST_STATIC_ASSERT((
        std::size_t(mpl::size<typename BitfieldTuple::members>::value)
            >=
        Index
    ));

    typedef typename mpl::at_c<
        typename BitfieldTuple::members,
        Index
    >::type             type;
};

template <typename BitfieldTuple, typename Name>
struct get_proxy_reference_type_by_name {
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
        >                               type;

};

template <typename BitfieldTuple, std::size_t Index>
struct get_proxy_reference_type_by_index {
    // check to make sure index is valid
    typedef typename mpl::less<
        mpl::size_t<
            Index
        >,
        mpl::size<
            typename BitfieldTuple::members
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
    >                           type;
};

}} // end boost::bitfields

#endif

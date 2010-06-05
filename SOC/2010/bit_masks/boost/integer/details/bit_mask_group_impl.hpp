//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_IMPL_HPP
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>

/** This is used for creating an extention into the boost fusion library for 
 *  the bit_mask_group.
 */
namespace boost {
namespace details {
/** Sequence tag.
 *  This is used for the boost.fusion extention.
 *  Used for dispatching to function overloads.
 *  The only thing needed for this is the declaration but just in case I have
 *  provided an empty definition.
 */
struct bit_mask_group_tag { };

}} // end boost::details



/** Next step in sequence extention.
 *  Adding dispatch look up mechanism.
 */
namespace boost { namespace fusion { namespace traits {

    template<>
    template <  typename Mask0, typename Mask1, typename Mask2, typename Mask3,
                typename Mask4, typename Mask5, typename Mask6, typename Mask7,
                typename Mask8,typename Mask9 >
    struct tag_of< boost::bit_mask_group<Mask0, Mask1, Mask2, Mask3, Mask4,
                           Mask5, Mask6, Mask7, Mask8, Mask9 > >
    {
        typedef boost::details::bit_mask_group_tag type;
    };
}}} // end boost::fusion::traits


/** Iterator type for fusion sequence.
 *  This is the iterator for iteration over our sequence.
 */
namespace boost {


/** This is my sequence for iteration over the items in bit_mask_group. */
template<typename MaskGroup, int Pos>
struct bit_mask_group_iterator
    : boost::fusion::iterator_base< bit_mask_group_iterator<MaskGroup, Pos> >
{
    // TODO: At a later time add preconditions for the iterator.
    // BOOST_STATIC_ASSERT(Pos >=0 && Pos < 3);
    typedef MaskGroup mask_group;
    typedef boost::mpl::int_<Pos>   index;
    typedef boost::fusion::random_access_traversal_tag category;

    // bit_mask_group_iterator(Struct& str) { }

    // Struct& struct_;
};

namespace details {
    struct bit_mask_group_iterator_tag { };
}} // end boost::details



/** providing another type_of specilization for the bit_mask_group's 
 * iterator.
 */ 
namespace boost { namespace fusion { namespace traits {

    template<>
    template <typename MaskGroup, int Pos>
    struct tag_of< boost::bit_mask_group_iterator<MaskGroup,Pos> > {
        typedef boost::details::bit_mask_group_iterator_tag type;
    };
}}} // end boost::fusion::traits


namespace boost { namespace fusion { namespace extension {

template<typename>
struct value_at_impl;

template <>
struct value_at_impl< boost::details::bit_mask_group_iterator_tag > {
    template<typename Iterator>
    struct apply;

    template<typename MaskGroup, int Pos>
    struct apply< boost::bit_mask_group_iterator<MaskGroup, Pos> > {
        typedef typename MaskGroup::template get_by_index<Pos>::type type;
    };
};

template <typename>
struct deref_impl;

template<>
struct deref_impl<boost::details::bit_mask_group_iterator_tag >
{
    template<typename Iterator>
    struct apply;

    template<typename MaskGroup, int Pos>
    struct apply< boost::bit_mask_group_iterator<MaskGroup,Pos> > {
        typedef typename MaskGroup::template get_by_index<Pos>::type mask_type;

        static typename mask_type::value_type
        call(boost::bit_mask_group_iterator<MaskGroup,Pos> const&) {
            return mask_type::value;
        }
    };
};

template <typename>
struct next_impl;

template<>
struct next_impl< boost::details::bit_mask_group_iterator_tag > {

    template<typename Iterator>
    struct apply {
        typedef typename Iterator::mask_group mask_group;
        typedef typename Iterator::index index;
        typedef boost::bit_mask_group_iterator<
            mask_group,
            index::value + 1
        > type;

        static type call(Iterator const&) {
             return type();
        }
    };
};

template <typename>
struct prior_impl;

template <>
struct prior_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename Iterator>
    struct apply {
        typedef typename Iterator::mask_group  mask_group;
        typedef typename Iterator::index        index;
        typedef typename boost::bit_mask_group_iterator<
            mask_group,
            index::value-1
        > type;

        static type call(Iterator const&) {
            return type();
        }
    };
};

template <typename>
struct advance_impl;

template <>
struct advance_impl< boost::details::bit_mask_group_iterator_tag > {

    template <typename Iterator, typename N>
    struct apply {
        typedef typename Iterator::mask_group mask_group;
        typedef typename Iterator::index index;
        typedef typename boost::bit_mask_group_iterator<
            mask_group,
            index::value+N::value
        > type;

        static type
        call(Iterator const&) {
            return type();
        }
    };
};

template <typename>
struct distance_impl;

template <>
struct distance_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename First, typename Last>
    struct apply
        : mpl::minus< typename Last::index, typename First::index>
    {
        static typename mpl::minus<
            typename Last::index,
            typename First::index
        >::type
        call(First const&, Last const&) {
            typedef typename mpl::minus<
                typename Last::index,
                typename First::index
            >::type result;

            return result();
        }
    };
};

template <typename>
struct equal_to_impl;

template <>
struct equal_to_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename I1, typename I2>
    struct apply             
        : is_same<
            typename I1::identity,
            typename I2::identity
        >::type
    { };
};

template<typename>
struct is_sequence_impl;

template<>
struct is_sequence_impl< boost::details::bit_mask_group_tag > {
    template<typename T>
    struct apply : mpl::true_ { };
};

template <typename>
struct is_view_impl;

template <>
struct is_view_impl< boost::details::bit_mask_group_tag > {
    template <typename Sequence>
    struct apply : mpl::false_ { };
};

template<typename>
struct begin_impl;

template<>
struct begin_impl< boost::details::bit_mask_group_tag > {
    template<typename MaskGroup>
    struct apply {
        typedef boost::bit_mask_group_iterator<MaskGroup, 0> type;
        static type call(MaskGroup&) {
            return type();
        }
    };
};


template<typename>
struct size_impl;

template<>
struct size_impl< boost::details::bit_mask_group_tag > {
    template<typename MaskGroup>
    struct apply
      : mpl::size< typename MaskGroup::mask_vector >
    { };
};

template<typename>
struct value_at_impl;

template <>
struct value_at_impl< boost::details::bit_mask_group_tag > {
    template <typename MaskGroup, typename N>
    struct apply {
        typedef typename MaskGroup::
            template get_by_index<
                N::value
            >::type
        type;
    };
};


template<typename>
struct at_impl;

template<>
struct at_impl< boost::details::bit_mask_group_tag > {
    template <typename MaskGroup, typename N>
    struct apply {
        typedef typename MaskGroup::
            template get_by_index<
                N::value
            >::type
        type;
        static typename type::value_type call(MaskGroup&) {
            return type::value;
        }
    };
};


template<typename>
struct at_key_impl;

template<>
struct at_key_impl< boost::details::bit_mask_group_tag > {

    template<typename MaskGroup, typename Key>
    struct apply {
        typedef typename MaskGroup::
            template get_by_name<
                Key
            >::type
        type;

        static typename type::value_type call(MaskGroup&) {
            return type::value;
        }
    };
};


}}} // end boost::fusion::extention


#endif


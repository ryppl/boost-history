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
template<typename Struct, int Pos>
struct bit_mask_group_iterator
    : boost::fusion::iterator_base< bit_mask_group_iterator<Struct, Pos> >
{
    // TODO: At a later time add preconditions for the iterator.
    // BOOST_STATIC_ASSERT(Pos >=0 && Pos < 3);
    typedef Struct struct_type;
    typedef boost::mpl::int_<Pos>   index;
    typedef boost::fusion::random_access_traversal_tag category;

    bit_mask_group_iterator(Struct& str) { }

    Struct& struct_;
};

namespace details {
    struct bit_mask_group_iterator_tag { };
}} // end boost::details



/** providing another type_of specilization for the bit_mask_group's 
 * iterator.
 */ 
namespace boost { namespace fusion { namespace traits {

    template<>
    template <typename Struct, int Pos>
    struct tag_of< boost::bit_mask_group_iterator<Struct,Pos> > {
        typedef boost::details::bit_mask_group_iterator_tag type;
    };
}}} // end boost::fusion::traits


namespace boost { namespace fusion { namespace extension {
template<typename T>
struct value_at_impl;

template <>
struct value_at_impl<details::bit_mask_group_iterator_tag> {
    template<typename Iterator>
    struct apply;


    template<typename Struct, int Pos>
    struct apply<boost::bit_mask_group_iterator<Struct,Pos> > {
        typedef typename Struct::template get_by_index<Pos>::type type;
    };
};

}}} // end boost::fusion::extention


#endif

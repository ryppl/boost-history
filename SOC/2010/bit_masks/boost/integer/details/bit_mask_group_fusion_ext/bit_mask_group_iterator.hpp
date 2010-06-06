//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)




#ifndef BOOST_BIT_MASK_GROUP_ITERATOR_HPP
#define BOOST_BIT_MASK_GROUP_ITERATOR_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>

namespace boost { namespace fusion {

/** This is the iterator which holds the maskgroup and allows
 *  the user to iterate over a bit_mask_group type.
 *  As the type bit_mask_group is trivially constructible, copyable and 
 *  destructable there is no member to actually hold any data.
 */
template<typename MaskGroup, unsigned int Pos>
struct bit_mask_group_iterator
    : boost::fusion::iterator_facade<
        bit_mask_group_iterator<MaskGroup, Pos>,
        boost::fusion::random_access_traversal_tag
    >
{
    // TODO: At a later time add preconditions for the iterator.
    typedef MaskGroup mask_group;
    typedef boost::mpl::int_<Pos> index;
    typedef boost::fusion::random_access_traversal_tag category;

    /** value_of. */
    template <typename Iterator>
    struct value_of {
        typedef typename Iterator::mask_group mask_group;
        typedef typename mask_group::template get_by_index<
            Iterator::index::value
        >::type type;
    };

    /** deref. */
    template <typename Iterator>
    struct deref {
        typedef typename Iterator::mask_group mask_group;
        typedef typename mask_group::template get_by_index<
            Iterator::index::value
        >::type type;

        static type call(Iterator const&){
            return type();
        }
    };

    /** Advance. */
    template <typename Iterator, typename N>
    struct advance {
        typedef typename Iterator::index index;
        typedef typename Iterator::mask_group mask_group;
        typedef bit_mask_group_iterator<
            mask_group,
            index::value + N::value
        > type;

        static type call(Iterator const& i) {
            return type();
        }
    };
    
    /** Next and Prior. */
    template <typename Iter> struct next : advance<Iter, mpl::int_< 1 > > { };
    template <typename Iter> struct prior : advance<Iter, mpl::int_<-1> > { };

    /** Distance. */
    template <typename I1, typename I2>
    struct distance
        : mpl::minus<typename I2::index, typename I1::index>
    {
        typedef typename
            mpl::minus<
                typename I2::index,
                typename I1::index
            >::type 
        type;

        static type call(I1 const&, I2 const&) {
            return type();
        }

    };

};

} } // end boost::fusion

#endif

//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP
#define BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP
#include "bitfield_vector_base.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/set.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/integer/high_bits_mask.hpp>
#include <cstddef>

namespace boost { namespace detail {

/** This calculates the size of the array needed to hold the entire mask
 *  based on an Offset and a Width.
 */
template <typename Offset, typename Width>
struct mask_size {
    BOOST_STATIC_ASSERT( Offset::value < 8 );
    
    typedef typename mpl::if_<
        mpl::greater<
            typename mpl::modulus<
                typename mpl::plus<
                    Offset,
                    Width
                >::type,
                mpl::size_t<8>
            >::type,
            mpl::size_t<0>
        >,
        typename mpl::plus<
            typename mpl::divides<
                typename mpl::plus<
                    Offset,
                    Width
                >::type,
                mpl::size_t<8>
            >::type,
            mpl::size_t<1>
        >::type,
        typename mpl::divides<
            typename mpl::plus<
                Offset,
                Width
            >::type,
            mpl::size_t<8>
        >::type
    >::type type;
};



template <
    std::size_t Offset,
    std::size_t Width,
    std::size_t Index = Offset,
    storage_t Mask = 0,
    bool = true
>
struct calc_first_byte_helper {
        typedef mpl::integral_c<storage_t,Mask> type;
};

template <
    std::size_t Offset,
    std::size_t Width,
    std::size_t Index,
    storage_t Mask
>
struct calc_first_byte_helper< Offset, Width, Index, Mask, true >
    :calc_first_byte_helper<
        Offset,
        Width,
        Index + 1,
        storage_t(Mask | (0x80 >> Index)),
        bool((Index + 1) < (Offset + Width))
          &&
        bool((Index + 1) < 8)
    >
        
{ };

template <std::size_t Offset, std::size_t Width>
struct calc_first_byte {
    typedef typename calc_first_byte_helper<Offset,Width>::type type;
};


template<std::size_t Offset, std::size_t Width>
struct calc_last_byte {
    typedef typename mpl::if_c<
        bool(mask_size<mpl::size_t<Offset>,mpl::size_t<Width> >::type::value == 1 ),
        mpl::size_t<0>,
        typename mpl::if_c<
            (((Offset + Width)%8) == 0),
            mpl::integral_c<storage_t,0xFF>,
            mpl::integral_c<storage_t,( 0xFF << (8-((Offset+Width)%8)))>
        >::type
    >::type type;
};

template<std::size_t Offset, std::size_t Width>
struct calc_last_left_shift {

};

/** This is responsible for storing information about a perticular
 *  mask but not the mask it self.
 */
template <
    std::size_t Offset,
    std::size_t Width
>
struct mask_info {
    BOOST_STATIC_CONSTANT(std::size_t, offset   = Offset);
    BOOST_STATIC_CONSTANT(std::size_t, width    = Width );
    BOOST_STATIC_CONSTANT(std::size_t, size =
        (mask_size<
            mpl::size_t<Offset>,
            mpl::size_t<Width>
        >::type::value)
    );
    BOOST_STATIC_CONSTANT(storage_t,
        first_value = (calc_first_byte<offset,width>::type::value));
    // BOOST_STATIC_CONSTANT( std::size_t,);
    // BOOST_STATIC_CONSTANT( storage_t, (last_value = LastIndexValue);
};



/** Helper meta-function which is used to determine valid offsets for a 
 *  perticular width.
 */
template <
    std::size_t Width,
    std::size_t Index = 0,
    typename Set = mpl::set<>,
    bool = true
    >
struct valid_offset_helper;

template <std::size_t Width, std::size_t Index, typename Set>
struct valid_offset_helper<Width,Index,Set,true>
    :valid_offset_helper<
        Width,
        Index + 1,
        typename mpl::insert<Set,bool,mpl::size_t<((Index * Width) % 8)> >::type,
        bool(Index < 8)
    >
{ };


template <std::size_t Width, std::size_t Index, typename Set>
struct valid_offset_helper< Width, Index, Set, false > {
    typedef Set type;
};

/** This class is responsible for determining all valid offsets of a perticular
 *  width and them storing them within a set.
 */
template <std::size_t Width>
struct determine_vaild_offsets {
    typedef typename valid_offset_helper<Width>::type type;
};



/** This will create some structure which will allow for quick retrieval of a
 *  all masks for a perticular width.
 */
template <std::size_t Width>
struct create_masks {
    typedef typename determine_vaild_offsets<Width>::type indices;
/*
    typedef typename mpl::if_<
        has_key<indices,mpl::size_t<0> >::type,
*/

};


}} // end booss::detail
#endif

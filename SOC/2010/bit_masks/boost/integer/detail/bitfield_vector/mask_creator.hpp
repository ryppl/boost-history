//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP
#define BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP
#include "bitfield_vector_base.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/void.hpp>
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
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <cstddef>
#include <climits>

namespace boost { namespace detail {

/** This calculates the size of the array needed to hold the entire mask
 *  based on an Offset and a Width.
 */
template <typename Offset, typename Width>
struct mask_size {
    BOOST_STATIC_ASSERT( bool((Offset::value) < CHAR_BIT ));
    
    typedef typename mpl::if_<
        mpl::greater<
            typename mpl::modulus<
                typename mpl::plus<
                    Offset,
                    Width
                >::type,
                mpl::size_t<CHAR_BIT>
            >::type,
            mpl::size_t<0>
        >,
        typename mpl::plus<
            typename mpl::divides<
                typename mpl::plus<
                    Offset,
                    Width
                >::type,
                mpl::size_t<CHAR_BIT>
            >::type,
            mpl::size_t<1>
        >::type,
        typename mpl::divides<
            typename mpl::plus<
                Offset,
                Width
            >::type,
            mpl::size_t<CHAR_BIT>
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
        bool((Index + 1) < CHAR_BIT)
    >
        
{ };

template <std::size_t Offset, std::size_t Width>
struct calc_first_byte {
    typedef typename calc_first_byte_helper<Offset,Width>::type type;
};


template<std::size_t Offset, std::size_t Width>
struct calc_last_byte {
    typedef typename mpl::if_c<
        (mask_size<mpl::size_t<Offset>,mpl::size_t<Width> >::type::value == 1 ),
        mpl::size_t<0>,
        typename mpl::if_c<
            (((Offset + Width)%8) == 0),
            mpl::integral_c<storage_t,0xFF>,
            mpl::integral_c<storage_t,( 0xFF << (8-((Offset+Width)%8)))>
        >::type
    >::type type;
};

template<std::size_t Offset, std::size_t Width>
struct calc_last_shift {
    typedef typename mpl::if_c<
        bool(mask_size<mpl::size_t<Offset>,mpl::size_t<Width> >::type::value==1),
        mpl::size_t<std::size_t(CHAR_BIT-(Width + Offset))>,
        mpl::size_t<std::size_t(CHAR_BIT-((Offset+Width)%8))>
    >::type                             type;
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
    BOOST_STATIC_CONSTANT( std::size_t,
        last_shift =(calc_last_shift<Offset,Width>::type::value));
    BOOST_STATIC_CONSTANT( storage_t,
        last_value = (calc_last_byte<Offset,Width>::type::value));
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
    
    typedef typename mpl::push_back<
        mpl::vector<>,
        typename mpl::if_<
            typename mpl::has_key<
                indices,
                mpl::size_t<0>
            >::type,
            mask_info<0,Width>,
            mpl::void_*
        >::type
    >::type     step_0;
    

#define BOOST_BUILD_INDEX_VECTOR( PREVIOUS_STEP,N ) \
    typedef typename mpl::push_back< \
        step_##PREVIOUS_STEP,        \
        typename mpl::if_<           \
            typename mpl::has_key<   \
                indices,             \
                mpl::size_t<N>       \
            >::type,                 \
            mask_info<N,Width>,      \
            mpl::void_*              \
        >::type                      \
    >::type     step_##N;

    BOOST_BUILD_INDEX_VECTOR(0,1)
    BOOST_BUILD_INDEX_VECTOR(1,2)
    BOOST_BUILD_INDEX_VECTOR(2,3)
    BOOST_BUILD_INDEX_VECTOR(3,4)
    BOOST_BUILD_INDEX_VECTOR(4,5)
    BOOST_BUILD_INDEX_VECTOR(5,6)
    BOOST_BUILD_INDEX_VECTOR(6,7)

#undef BOOST_BUILD_INDEX_VECTOR
    typedef step_7 type;
};

/** This is a type constructed over a single mask_info type and is used to
 *  relay some information between compile time and runtime data structures.
 */
struct mask_detail {
    mask_detail()
        :m_offset(0),
        m_size(0),
        m_last_shift(0),
        m_first_byte(0),
        m_last_byte(0)
    { }

    mask_detail(mask_detail const& x)
        :m_offset(x.m_offset),
        m_size(x.m_size),
        m_last_shift(x.m_last_shift),
        m_first_byte(x.m_first_byte),
        m_last_byte(x.m_last_byte)
    { }

    template <typename MaskInfo>
    mask_detail(MaskInfo const&)
        :m_offset(MaskInfo::offset),
        m_size(MaskInfo::size),
        m_last_shift(MaskInfo::last_shift),
        m_first_byte(MaskInfo::first_value),
        m_last_byte(MaskInfo::last_value)
    { }

    mask_detail(mpl::void_* )
        :m_offset(0),
        m_size(0),
        m_last_shift(0),
        m_first_byte(0),
        m_last_byte(0)
    { }

    mask_detail& operator=(mask_detail const& x) {
        m_offset = x.m_offset;
        m_size = x.m_size;
        m_last_shift = x.m_last_shift;
        m_first_byte = x.m_first_byte;
        m_last_byte = x.m_last_byte;
        return *this;
    }


    std::size_t m_offset;
    std::size_t m_size;
    std::size_t m_last_shift;
    storage_t   m_first_byte;
    storage_t   m_last_byte;
};

/** This is the function responsible for taking both the width and the offset
 *  and using them to get the correct constant out of the create_masks function
 *  then storing that information inside of a mask_detail and returning that
 *  mask detail.
 */
template <std::size_t Width>
mask_detail get_mask_detail(std::size_t offset) {
    typedef typename create_masks<Width>::type masks;
    switch(offset){
        case 0:
            return mask_detail(typename mpl::at_c<masks,0>::type());
        case 1:
            return mask_detail(typename mpl::at_c<masks,1>::type());
        case 2:
            return mask_detail(typename mpl::at_c<masks,2>::type());
        case 3:
            return mask_detail(typename mpl::at_c<masks,3>::type());
        case 4:
            return mask_detail(typename mpl::at_c<masks,4>::type());
        case 5:
            return mask_detail(typename mpl::at_c<masks,5>::type());
        case 6:
            return mask_detail(typename mpl::at_c<masks,6>::type());
        case 7:
            return mask_detail(typename mpl::at_c<masks,7>::type());
        default:
        BOOST_ASSERT(false);
    }
}

}} // end booss::detail
#endif

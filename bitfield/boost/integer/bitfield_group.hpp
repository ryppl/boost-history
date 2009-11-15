//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTEGER_BITFIELD_GROUP__HPP
#define BOOST_INTEGER_BITFIELD_GROUP__HPP

#include <boost/integer/bitfield.hpp>

namespace boost { namespace integer {
    
    namespace detail {
    }

    /*
        Used to easily manipulate groups of bitfields the same way as does C bitfields,
        but in a portable manner.
        \par Example declaration:
        \code
        struct Rgb565
        {
            struct red {};
            struct green {};
            struct blue {};
            typedef bitfields<mpl::vector<
                    member<unsigned char, red,   5>,
                    member<unsigned char, green, 6>,
                    member<unsigned char, blue,  5> 
            > > type; 
            //typedef bitfields_group<mpl::vector_c<5,6,5> > type2; 
        };
    
        \endcode
        \par Example usage:
        \code
        Rgb565::type r = make_bitfields<Rgb565::type, 1,2,3>;

        // Write to a bitfield.
        r.get<Rgb565::red>() = 23;same
        r.set<Rgb565::red>(23);

        // Read from a bitfield
        Rgb565::at<Rgb565::blue>::value_type b = r.get<Rgb565::blue>();

        // Access a bit within a bit field
        bool bit = r.get<Rgb565::blue>()[3];
        \endcode
        \note Bitfields within the  word may overlap.
        \param STORAGE_TYPE    Unsigned integer type of the word occupied by the bitfield
        \param F    Position of the first bit (0 is the least significant bit)
        \param L    Position of the last bit (0 is the least significant bit)
        \param VALUE_TYPE    Value type of the bitfield itself 
        \param REFERENCE_TYPE    Reference type of the word occupied by the bitfield 
    */
    
    template <typename IntegerType, typename Tag, std::size_t Bits>
    struct member {
        typedef IntegerType integer_type;
        typedef Tag tag_type;
        static const std::size_t BITS = Bits;
    };

    template <typename Member>
    struct bitfield_size {
        static const std::size_t value = Member::BITS;
    };
    
    template <typename StorageType>
    struct member_bitfield_type {
        template <typename Member, typename State>
        struct apply {
            static const std::size_t F= State::FIRST;
            struct type {
                static const FIRST=F+Member::BITS;
                typedef mpl::insert<State::map_type,
                                mpl::pair<typename Member::tag_type, bitfield<StorageType, F, F+Member::BITS-1, typename Member::integer_type>
                    >::type map_type;
            };
        };
    };
    
    struct bitfields_type_initial_state {
        struct type {
            static const FIRST=0;
            typedef mpl::map<> map_type;
        };
    };
    
    template <typename Members, typename Storage>
    struct bitfields_type {
        typedef mpl::fold<
            Members
            , bitfields_type_initial_state
            , member_bitfield_type<Storage>
        >::type::map_type type ;
    };
    

    template <typename Members>
    struct bitfields_size {
        typedef mpl::fold<
            Members
            , mpl::int_<0>
            , mpl::plus< _1, bitfield_size<_2> >
        >::type::value value ;
    };
    
    template <typename Members, typename Storage=mpl::void_>
    struct bitfields_group {
        
        // number of bits of all the bitfields
        static const std::size_t size = bitfields_size<Members>::value;
        // if not explicit select the storage associated to the number of bits of all the bitfields
        typedef mpl::if<is_void<Storage>, uint_t<size>::least, Storage> storage_type;
        // assert bits of storage_type >= size
        
        // mpl map of all the bitfields types
        typedef bitfields_type<Members, storage_type>::type map_type;
        storage_type value;
        bitfields_group() : value(0) {}
        
    };
    
    template <typename Key, typename typename Members, typename Storage>
    typename result_of::has_key<bitfields_group<Members, Storage>, Key>::type
    has_key(bitfields_group<Members, Storage> const& seq) {
        return mpl::has_key<bitfields_group<Members, Storage>::map_type>::type;
    };
    
    template <typename Key, typename typename Members, typename Storage>
    typename result_of::has_key<bitfields_group<Members, Storage>, Key>::type
    at_key(bitfields_group<Members, Storage> const& seq) {
        return mpl::at_key<Key,bitfields_group<Members, Storage>::map_type>::type(value);
    };
    
    template <typename Key, typename typename Members, typename Storage>
    typename result_of::has_key<bitfields_group<Members, Storage>, Key>::type
    at_key(bitfields_group<Members, Storage> & seq) {
        return mpl::at_key<Key,bitfields_group<Members, Storage>::map_type>::type(value);
    };
    
}}
#endif


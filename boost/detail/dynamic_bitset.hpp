// (C) Copyright Chuck Allison and Jeremy Siek 2001, 2002.
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.

// With optimizations by Gennaro Prota.

#ifndef BOOST_DETAIL_DYNAMIC_BITSET_HPP
#define BOOST_DETAIL_DYNAMIC_BITSET_HPP

#include <cstddef> // for std::size_t
#include "boost/config.hpp"
#include "boost/detail/workaround.hpp"
#include "boost/static_assert.hpp"

#if !(BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551)) || defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS))
#define BOOST_DYN_BITSET_USE_FRIENDS
#endif

namespace boost {

  namespace detail {


    // Gives access to the object representation
    // of an object of type T (3.9p4).
    // p must NOT point to a base sub-object
    //
    template <typename T>
    inline const unsigned char * object_representation (T* p)
    {
        return static_cast<const unsigned char *>(static_cast<const void *>(p));
    }



    // ------- count function implementation --------------

    namespace dynamic_bitset_count_impl {

    typedef unsigned char byte_type;

    enum mode { access_by_bytes, access_by_blocks };

    template <mode> struct mode_to_type {};

    // the table: wrapped in a class template, so
    // that it is only instantiated if/when needed
    //
    template <bool dummy_name = true>
    struct count_table {};

    template <>
    struct count_table<true> {
        static const byte_type table[];
    };


     const unsigned int table_width = 8;
     const byte_type count_table<true>::table[] =

     {
       // Automatically generated by GPTableGen.exe v.1.0
       //
     0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
     3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
     };

     BOOST_STATIC_ASSERT ( sizeof (count_table<true>::table) == (1u << table_width) );



     // overload for access by bytes
     //

     template <typename Iterator>
     inline std::size_t do_count(Iterator first, std::size_t length, int /*dummy param*/,
                                 mode_to_type<access_by_bytes>* ) {

         std::size_t num = 0;
         if (length)
         {
             const byte_type * p = object_representation(&*first);
             length *= sizeof(*first);

              do {
                 num += count_table<>::table[*p];
                 ++p;
                 --length;

             } while (length);
         }

         return num;
     }


     // overload for access by blocks
     //
     template <typename Iterator, typename ValueType>
     inline std::size_t do_count(Iterator first, std::size_t length, ValueType,
                                 mode_to_type<access_by_blocks>*) {

         std::size_t num = 0;

         while (length){

             ValueType value = *first;
             while (value) {
                 num += count_table<>::table[value & ((1u<<table_width) - 1)];
                 value >>= table_width;
             }

             ++first;
             --length;
         }

         return num;
     }


    } // dynamic_bitset_count_impl
    // -------------------------------------------------------




  } // namespace detail

} // namespace boost

#endif // BOOST_DETAIL_DYNAMIC_BITSET_HPP


// (C) Copyright Craig Henderson 2002, 2003
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

#ifndef BOOST_BOYER_MOORE_H
#define BOOST_BOYER_MOORE_H

#include <boost/config.hpp>

#if !defined(BOOST_NO_STD_ITERATOR_TRAITS)
#   include <iterator>      // iterator_traits
#endif

namespace boost {

    namespace detail {

#if defined(BOOST_NO_STD_ITERATOR_TRAITS)
        // Declare traits for the standard string classes
        template<typename Iter>
        struct boyer_moore_traits;

        template<>
        struct boyer_moore_traits<std::string::const_iterator>
        {
            typedef
            std::string::value_type
            alphabet_type;

            BOOST_STATIC_CONSTANT(unsigned long,
                                  alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
        };

        template<>
        struct boyer_moore_traits<std::wstring::const_iterator>
        {
            typedef
            std::wstring::value_type
            alphabet_type;

            BOOST_STATIC_CONSTANT(unsigned long,
                                  alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
        };
#else
        template<typename Iter>
        struct boyer_moore_traits
        {
          private:
            // the alphabet type is the type that is used to represent an
            // element in the sequence
            typedef
            typename std::iterator_traits<Iter>::value_type
            alphabet_type;

          public:
            // define the default alphabet size to be the maximum number of
            // elements that can be represented by the type
            // eg. for T=char, alphabet_size=255
            //     for T=long, alphabet_size=4294967295 (0xffffffff)
            //
            // the user should specialize this class and specify a smaller
            // alphabet size for non improved space efficiency, or non-integral
            // types for T
            BOOST_STATIC_CONSTANT(unsigned long,
                                  alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
        };
#endif

        // small helper function to initialise the pattern working array
        template<unsigned long alphabet_size, typename Array, typename Pattern>
        void init_pattern(Array &array, Pattern pattern, std::ptrdiff_t len)
        {
            std::ptrdiff_t loop;

            for (loop=0; loop<=alphabet_size; ++loop)
                array[loop] = len;
            --len;
            for (loop=0; loop<len; ++pattern, ++loop)
                array[*pattern] = len - loop;
        }


        template<typename ItS, typename ItP, typename Array>
        ItS boyer_moore_search(ItS            begin_sequence,
                               std::ptrdiff_t len_search,
                               ItP            begin_pattern,
                               std::ptrdiff_t len_pattern,
                               Array         &array)
        {
            std::ptrdiff_t loop  = len_pattern - 1;
            while (loop < len_search)
            {
                // start at the end of the search sequence
                ItS it_search = begin_sequence;
                std::advance<>(it_search, loop);

                // start at the end of the pattern
                ItP it_patt = begin_pattern;
                std::advance<>(it_patt, len_pattern-1);

                // store the last element for use later
                // (this is just a simple optimisation)
                typename
                boyer_moore_traits<ItS>::alphabet_type
                current = *it_search;

                // work from the end of the pattern to the beginning and compare
                // each element with the search sequence
                while (it_patt != begin_pattern  &&  *it_search == *it_patt)
                {
                    --it_search;
                    --it_patt;
                }

                // if all of the elements match, then return the iterator pointing
                // to the start of the pattern within the search sequence
                if (it_patt == begin_pattern  &&  *it_search == *it_patt)
                    return it_search;

                // skip over this position and continue around the loop
                loop += array[current];
            }

            ItS it_search = begin_sequence;
            std::advance<>(it_search, len_search);
            return it_search;
        }

    }   // namespace detail


    // class to search for a pattern in multiple sequences
    template<typename ItS,
             typename ItP,
             typename Alloc = std::allocator<std::ptrdiff_t>
            >
    struct boyer_moore_pattern_search : Alloc
    {
      private:
        typedef
        detail::boyer_moore_traits<ItS>
        traits;

        ItP              begin_pattern_;
        std::ptrdiff_t  *array_;
        std::ptrdiff_t   len_pattern_;

      public:
        boyer_moore_pattern_search(ItP begin_pattern, ItP end_pattern)
          : begin_pattern_(begin_pattern)
        {
            array_ = allocate(traits::alphabet_size+1);
            memset(array_, 0, (traits::alphabet_size+1)*sizeof(*array_));

            len_pattern_ = std::distance<>(begin_pattern, end_pattern);
            detail::init_pattern<traits::alphabet_size>(array_, begin_pattern_, len_pattern_);
        }

        ~boyer_moore_pattern_search()
        {
            deallocate(array_, traits::alphabet_size+1);
        }
        
        ItS search(ItS begin_sequence, ItS end_sequence)
        {
            // calculate the length of the pattern and the search sequence
            std::ptrdiff_t len_search = std::distance<>(begin_sequence, end_sequence);

            // if the pattern is longer than the search sequence, then we're not
            // going to find the pattern, so return immediately
            if (len_pattern_ > len_search)
                return end_sequence;

            // create and initialise the pattern alphabet
            return detail::boyer_moore_search<ItS, ItP>(begin_sequence,
                                                        len_search,
                                                        begin_pattern_,
                                                        len_pattern_,
                                                        array_);
        }
    };


    template<typename ItS, typename ItP>
    ItS boyer_moore_search(ItS begin_sequence,
                           ItS end_sequence,
                           ItP begin_pattern,
                           ItP end_pattern)
    {
        // calculate the length of the pattern and the search sequence
        const std::ptrdiff_t len_pattern = std::distance<>(begin_pattern, end_pattern);
        const std::ptrdiff_t len_search  = std::distance<>(begin_sequence, end_sequence);

        // if the pattern is longer than the search sequence, then we're not
        // going to find the pattern, so return immediately
        if (len_pattern > len_search)
            return end_sequence;

        // create and initialise the pattern alphabet
        typedef
        detail::boyer_moore_traits<ItS>
        traits;

        std::ptrdiff_t array[traits::alphabet_size+1] = { 0 };
        detail::init_pattern<traits::alphabet_size>(array, begin_pattern, len_pattern);
        return detail::boyer_moore_search<ItS, ItP>(begin_sequence,
                                                    len_search,
                                                    begin_pattern,
                                                    len_pattern,
                                                    array);
    }


    template<typename T1, typename T2>
    inline
    BOOST_DEDUCED_TYPENAME T1::const_iterator
    boyer_moore_search(const T1 &sequence,
                       const T2 &pattern)
    {
        return boyer_moore_search<
                   BOOST_DEDUCED_TYPENAME T1::const_iterator,
                   BOOST_DEDUCED_TYPENAME T2::const_iterator>(
                       sequence.begin(),
                       sequence.end(),
                       pattern.begin(),
                       pattern.end());
    }

}       // namespace boost

#endif  // BOOST_BOYER_MOORE_H


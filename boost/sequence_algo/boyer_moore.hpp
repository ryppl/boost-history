// (C) Copyright Craig Henderson 2002
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

#include <algorithm>
#include <boost/config.hpp>

namespace boost {

    template<typename Iter>
    struct boyer_moore_traits
    {
// MSVC6 chokes on using the iterator_traits, so this class must be
// specialised for that compiler
#if !(defined(BOOST_MSVC)  &&  (_MSC_VER <= 1300))
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
#endif
    };


    namespace detail {

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

        template<typename Val1, typename Val2>
        struct select_value
        {
            Val1 v1_;
            Val2 v2_;

            select_value(Val1 v1, Val2 v2) : v1_(v1), v2_(v2) { }
            operator Val1()                             { return v1_; }
            operator Val2()                             { return v2_; }
        };

    }   // namespace detail


    template<typename Res, typename ItIn1, typename ItIn2, typename ItOut>
    void
    boyer_moore_search(ItIn1 begin_search,  ItIn1 end_search,
                       ItIn2 begin_pattern, ItIn2 end_pattern,
                       ItOut output,
                       std::size_t max_count = 0)
    {
        std::size_t count = 0;

        // calculate the length of the pattern and the search sequence
        std::ptrdiff_t len_pattern = std::distance<>(begin_pattern, end_pattern);
        std::ptrdiff_t len_search  = std::distance<>(begin_search, end_search);

        // if the pattern is longer than the search sequence, then we're not
        // going to find the pattern, so return immediately
        if (len_pattern > len_search)
            return;

        // create and initialise the pattern alphabet
        typedef boyer_moore_traits<ItIn1> traits;
        std::ptrdiff_t array[traits::alphabet_size+1] = { 0 };
        detail::init_pattern<traits::alphabet_size>(array, begin_pattern, len_pattern);

        std::ptrdiff_t loop = len_pattern - 1;
        while ((count < max_count  ||  max_count == 0)  &&  loop < len_search)
        {
            // start at the end of the search sequence
            ItIn1 it_search = begin_search;
            std::advance<>(it_search, loop);

            // start at the end of the pattern
            ItIn2 it_patt = begin_pattern;
            std::advance<>(it_patt, len_pattern-1);

            // store the last element for use later
            // (this is just a simple optimisation)
            typename traits::alphabet_type current = *it_search;

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
            {
                detail::select_value<ItIn1, std::ptrdiff_t> val(it_search, loop - len_pattern + 1);
                *output++ = val;
                ++count;
            }

            // skip over this position and continue around the loop
            loop += array[current];
        }

        return;
    }

}       // namespace boost

#endif  // BOOST_BOYER_MOORE_H


// (C) Copyright Craig Henderson 2002
//
// Levenshtein Edit Distance algorithm in linear space
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
//
// Thanks to Philippe Lalande for input and discussion on the algorithm

// guarded header file to prevent multiple include compilation errors
#ifndef BOOST_EDIT_DISTANCE_H
#define BOOST_EDIT_DISTANCE_H

namespace boost {

    template<typename Alloc, typename ItIn1, typename ItIn2>
    std::size_t
    edit_distance(ItIn1 begin_first, ItIn1 end_first,
                  ItIn2 begin_second, ItIn2 end_second,
                  Alloc &alloc)
    {
        const std::ptrdiff_t size_first  = std::distance<>(begin_first, end_first);
        const std::ptrdiff_t size_second = std::distance<>(begin_second, end_second);
        const std::ptrdiff_t array_size  = (size_first + 1) << 1;

        std::size_t *pA = alloc.allocate(array_size);
        std::size_t *pB = pA+size_first+1;
        memset(pA, 0, sizeof(std::size_t)*array_size);

        std::ptrdiff_t col;
        for (col=1; col<=size_first; ++col)
            pA[col] = col;

        ItIn2 it2 = begin_second;
        for (std::ptrdiff_t row=1; row<=size_second+1; ++row, ++it2)
        {
            ItIn1 it1 = begin_first;
            for (col=1; col<=size_first; ++col, ++it1)
            {
                pB[0]   = row;
                pB[col] = std::min<>(
                              std::min<>(
                                  pB[col-1]+1, pA[col]+1),
                              pA[col-1] + ((*it1 == *it2)? 0 : 1));
            }

            std::swap(pA, pB);
        }

        std::size_t result = pB[size_first];
        alloc.deallocate(std::min(pA,pB), array_size);
        return result;
    }

    template<typename ItIn1, typename ItIn2>
    inline
    std::size_t
    edit_distance(ItIn1 begin_first, ItIn1 end_first,
                  ItIn2 begin_second, ItIn2 end_second)
    {
        return edit_distance<>(
                   begin_first,
                   end_first,
                   begin_second,
                   end_second,
                   std::allocator<std::size_t>);
    }

}   // namespace boost

#endif  // BOOST_EDIT_DISTANCE_H

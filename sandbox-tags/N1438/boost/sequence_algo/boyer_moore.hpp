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

#include <boost/config.hpp>
#include <string>
#include <vector>


#if !(defined(BOOST_MSVC)  &&  (_MSC_VER <= 1300))
#   include <iterator>      // iterator_traits
#endif

namespace boost {

    template<typename Iter>
    struct boyer_moore_traits;

// MSVC6 chokes on using the iterator_traits, so this class must be
// specialised for that compiler
#if !(defined(BOOST_MSVC)  &&  (_MSC_VER <= 1300))
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
#else
    // Declare traits for the standard string classes for MSVC6
    template<>
    struct boost::boyer_moore_traits<std::string::const_iterator>
    {
        typedef
        std::string::value_type
        alphabet_type;

        BOOST_STATIC_CONSTANT(unsigned long,
                              alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
    };

    template<>
    struct boost::boyer_moore_traits<std::wstring::const_iterator>
    {
        typedef
        std::wstring::value_type
        alphabet_type;

        BOOST_STATIC_CONSTANT(unsigned long,
                              alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
    };
#endif


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
            operator Val1() const { return v1_; }
            operator Val2() const { return v2_; }
        };


        template<typename ItS, typename ItP, typename ItOut, typename Array>
        std::size_t
        boyer_moore_search(ItS            begin_search,
                           std::ptrdiff_t len_search,
                           ItP            begin_pattern,
                           std::ptrdiff_t len_pattern,
                           ItOut          output,
                           std::size_t    max_count,
                           Array         &array)
        {
            std::size_t    count = 0;
            std::ptrdiff_t loop  = len_pattern - 1;
            while ((count < max_count  ||  max_count == 0)  &&  loop < len_search)
            {
                // start at the end of the search sequence
                ItS it_search = begin_search;
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
                {
                    detail::select_value<ItS, std::ptrdiff_t> val(it_search, loop - len_pattern + 1);
                    *output++ = val;
                    ++count;
                }

                // skip over this position and continue around the loop
                loop += array[current];
            }

            return count;
        }

    }   // namespace detail


    template<typename ItS, typename ItP, typename ItOut>
    std::size_t
    boyer_moore_search(ItS          begin_search,
                       ItS          end_search,
                       ItP          begin_pattern,
                       ItP          end_pattern,
                       ItOut        output,
                       std::size_t  max_count)
    {
        // calculate the length of the pattern and the search sequence
        std::ptrdiff_t len_pattern = std::distance<>(begin_pattern, end_pattern);
        std::ptrdiff_t len_search  = std::distance<>(begin_search, end_search);

        // if the pattern is longer than the search sequence, then we're not
        // going to find the pattern, so return immediately
        if (len_pattern > len_search)
            return 0;

        // create and initialise the pattern alphabet
        typedef boyer_moore_traits<ItS> traits;
        std::ptrdiff_t array[traits::alphabet_size+1] = { 0 };
        detail::init_pattern<traits::alphabet_size>(array, begin_pattern, len_pattern);
        return detail::boyer_moore_search<ItS, ItP, ItOut>(begin_search,
                                                           len_search,
                                                           begin_pattern,
                                                           len_pattern,
                                                           output,
                                                           max_count,
                                                           array);
    }


    // class to search for a pattern in multiple sequences
    template<typename Res,
             typename ItS,
             typename ItP,
             typename Alloc=std::allocator<std::ptrdiff_t>
            >
    class boyer_moore_pattern_search
    {
      private:
        typedef std::vector<Res>        result;
        typedef boyer_moore_traits<ItS> traits;

        ItP              begin_pattern_;
        std::ptrdiff_t  *array_;
        std::ptrdiff_t   len_pattern_;
        Alloc            alloc_;
        result           results_;

      public:
        boyer_moore_pattern_search(ItP begin_pattern, ItP end_pattern)
        : begin_pattern_(begin_pattern)
        {
            array_ = alloc_.allocate(traits::alphabet_size+1);
            memset(array_, 0, (traits::alphabet_size+1)*sizeof(*array_));
            len_pattern_ = std::distance<>(begin_pattern, end_pattern);
            detail::init_pattern<traits::alphabet_size>(array_, begin_pattern_, len_pattern_);
        }

        ~boyer_moore_pattern_search()
        {
            alloc_.deallocate(array_, traits::alphabet_size+1);
        }
        
        std::size_t search(ItS begin_search, ItS end_search)
        {
            // clear any existing results
            results_.clear();

            // calculate the length of the pattern and the search sequence
            std::ptrdiff_t len_search = std::distance<>(begin_search, end_search);

            // if the pattern is longer than the search sequence, then we're not
            // going to find the pattern, so return immediately
            if (len_pattern_ > len_search)
                return 0;

            // create and initialise the pattern alphabet
            return detail::boyer_moore_search<ItS, ItP>(begin_search,
                                                        len_search,
                                                        begin_pattern_,
                                                        len_pattern_,
                                                        std::back_inserter<>(results_),
                                                        0, array_);
        }
    };


    inline
    std::string::const_iterator
    boyer_moore(const std::string &text,
                const std::string &pattern)
    {
        std::string::const_iterator it = text.end();
        boyer_moore_search<std::string::const_iterator,
                           std::string::const_iterator>(text.begin(),
                                                        text.end(),
                                                        pattern.begin(),
                                                        pattern.end(),
                                                        &it, 1);
        return it;
    }


    inline
    std::wstring::const_iterator
    boyer_moore(const std::wstring &text,
                const std::wstring &pattern)
    {
        std::wstring::const_iterator it = text.end();
        boyer_moore_search<std::wstring::const_iterator,
                           std::wstring::const_iterator>(text.begin(),
                                                         text.end(),
                                                         pattern.begin(),
                                                         pattern.end(),
                                                         &it, 1);
        return it;
    }


    inline
    std::string::const_iterator
    boyer_moore(const std::string             &text,
                const std::string::value_type *pattern)
    {
        std::string p(pattern);
        std::string::const_iterator it = text.end();
        boyer_moore_search<std::string::const_iterator,
                           std::string::const_iterator>(text.begin(),
                                                        text.end(),
                                                        p.begin(),
                                                        p.end(),
                                                        &it, 1);
        return it;
    }


    inline
    std::wstring::const_iterator
    boyer_moore(const std::wstring             &text,
                const std::wstring::value_type *pattern)
    {
        std::wstring p(pattern);
        std::wstring::const_iterator it = text.end();
        boyer_moore_search<std::wstring::const_iterator,
                           std::wstring::const_iterator>(text.begin(),
                                                         text.end(),
                                                         p.begin(),
                                                         p.end(),
                                                         &it, 1);
        return it;
    }


    inline
    std::string::value_type *
    boyer_moore(std::string::value_type *text,
                std::string::value_type *pattern)
    {
        std::string    t(text);
        std::string    p(pattern);
        std::ptrdiff_t len = std::char_traits<std::string::value_type>::length(text);
        boyer_moore_search<std::string::const_iterator,
                           std::string::const_iterator>(t.begin(),
                                                        t.end(),
                                                        p.begin(),
                                                        p.end(),
                                                        &len, 1);
        return text + len;
    }

}       // namespace boost

#endif  // BOOST_BOYER_MOORE_H


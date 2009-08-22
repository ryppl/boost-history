#ifndef BOOST_UNICODE_CAT_HPP
#define BOOST_UNICODE_CAT_HPP

#include <boost/unicode/compose.hpp>
#include <boost/unicode/utf.hpp>
#include <boost/unicode/combining.hpp>

#include <algorithm>
#include <boost/utility.hpp>

#include <boost/iterator/join_iterator.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/tuple/tuple.hpp>

#if defined(BOOST_UNICODE_DOXYGEN_INVOKED) || !defined(BOOST_NO_RVALUE_REFERENCES)
/** INTERNAL ONLY */
#define BOOST_UNICODE_FWD_2(macro) \
    macro(,&&,,&&)
#else
#define BOOST_UNICODE_FWD_2(macro) \
    macro(,&,,&) \
    macro(,&,const,&) \
    macro(const,&,,&) \
    macro(const,&,const,&)
#endif

namespace boost
{
namespace unicode
{
    
namespace detail
{
    template<typename ValueType, typename OutputIterator>
    pipe_output_iterator<
        OutputIterator,
        utf_encoder<ValueType>
    > utf_encoded_out(OutputIterator out)
    {
	    return piped_output(out, utf_encoder<ValueType>());
    }

} // namespace detail

/** INTERNAL ONLY */
#define BOOST_UNICODE_CAT_LIMITS_FWD(cv1, ref1, cv2, ref2) \
/** Partitions the two input ranges into a total of four ranges,
   the two inner ranges requiring transformation to maintain a certain
   normalization form while concatenated. */ \
template<typename Range1, typename Range2> \
tuple< \
    sub_range<cv1 Range1>, \
    sub_range<cv1 Range1>, \
    sub_range<cv2 Range2>, \
    sub_range<cv2 Range2> \
> \
cat_limits(cv1 Range1 ref1 range1, cv2 Range2 ref2 range2) \
{ \
    iterator_range< \
        pipe_iterator< \
            typename range_iterator<cv2 Range2>::type, \
            utf_decoder \
        > \
    > decoded2 = utf_decoded(range2); \
\
    char32 ch = *begin(decoded2); \
    if(ucd::get_combining_class(ch) != 0) \
    { \
        iterator_range< \
            pipe_iterator< \
                typename range_iterator<cv1 Range1>::type, \
                utf_decoder \
            > \
        > decoded1 = utf_decoded(range1); \
\
\
        typename range_iterator<cv1 Range1>::type \
        new_end = combiner().rtl(begin(decoded1), end(decoded1)).base(); \
\
        typename range_iterator<cv2 Range2>::type \
        new_begin = combiner().ltr(begin(decoded2), end(decoded2)).base(); \
\
        return make_tuple( \
            make_iterator_range(begin(range1), new_end), \
            make_iterator_range(new_end, end(range1)), \
            make_iterator_range(begin(range2), new_begin), \
            make_iterator_range(new_begin, end(range2)) \
        ); \
    } \
\
    return make_tuple( \
        range1, \
        make_iterator_range(end(range1), end(range1)), \
        make_iterator_range(end(range2), end(range2)), \
        range2 \
    ); \
}
BOOST_UNICODE_FWD_2(BOOST_UNICODE_CAT_LIMITS_FWD)
    
/** Concatenates two ranges of UTF code units and puts the result in \c out.
 * 
 * Throws \c std::out_of_range if the input or resulting strings are not stream-safe.
 * \pre \c Range1 and \c Range2 are in Normalized Form D, have the same value type and are non-empty.
 * \post \c out is in Normalized Form D and is stream-safe. */
template<typename Range1, typename Range2, typename OutputIterator>
OutputIterator decomposed_concat(const Range1& range1, const Range2& range2, OutputIterator out)
{
    tuple<
        sub_range<const Range1>,
        sub_range<const Range1>,
        sub_range<const Range2>,
        sub_range<const Range2>
    >
    t = cat_limits(range1, range2);
    
    out = copy(t.get<0>(), out);
    out = pipe(joined_n(utf_decoded(t.get<1>()), utf_decoded(t.get<2>())), combine_sorter(), detail::utf_encoded_out<typename range_value<const Range1>::type>(out)).base();
    return copy(t.get<3>(), out);
}

/** Concatenates two ranges of UTF code units and puts the result in \c out.
 * 
 * Throws \c std::out_of_range if the input or resulting strings are not stream-safe.
 * \pre \c Range1 and \c Range2 are in Normalized Form C, have the same value type and are non-empty.
 * \post \c out is in Normalized Form C and is stream-safe. */
template<typename Range1, typename Range2, typename OutputIterator>
OutputIterator composed_concat(const Range1& range1, const Range2& range2, OutputIterator out, unsigned mask = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical))
{
    tuple<
        sub_range<const Range1>,
        sub_range<const Range1>,
        sub_range<const Range2>,
        sub_range<const Range2>
    >
    t = cat_limits(range1, range2);
    
    out = copy(t.get<0>(), out);
    out = pipe(joined_n(t.get<1>(), t.get<2>()), make_piped_pipe(utf_decoder(), normalizer(mask)), detail::utf_encoded_out<typename range_value<const Range1>::type>(out)).base();
    return copy(t.get<3>(), out);
}

/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSED_CONCATED_FWD(cv1, ref1, cv2, ref2) \
template<typename Range1, typename Range2> \
iterator_range< \
    join_iterator< \
        tuple< \
            sub_range<cv1 Range1>, \
            iterator_range< \
                pipe_iterator< \
                    join_iterator< \
                        tuple< \
                            sub_range<cv1 Range1>, \
                            sub_range<cv2 Range2> \
                        > \
                    >, \
                    piped_pipe< \
                        utf_decoder, \
                        multi_pipe< \
                            normalizer, \
                            utf_encoder<typename range_value<cv1 Range1>::type> \
                        > \
                    > \
                > \
            >, \
            sub_range<cv1 Range2> \
        > \
    > \
> composed_concated(cv1 Range1 ref1 range1, cv2 Range2 ref2 range2, unsigned mask = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical)) \
{ \
    tuple< \
        sub_range<cv1 Range1>, \
        sub_range<cv1 Range1>, \
        sub_range<cv2 Range2>, \
        sub_range<cv2 Range2> \
    > \
    t = cat_limits(range1, range2); \
     \
    return joined_n( \
        t.get<0>(), \
        piped( \
            joined_n(t.get<1>(), t.get<2>()), \
            make_piped_pipe( \
                utf_decoder(), \
                make_multi_pipe( \
                    normalizer(mask), \
                    utf_encoder<typename range_value<cv1 Range1>::type>() \
                ) \
            ) \
        ), \
        t.get<3>() \
    ); \
}
BOOST_UNICODE_FWD_2(BOOST_UNICODE_COMPOSED_CONCATED_FWD)

    
} // namespace unicode
} // namespace boost

#endif

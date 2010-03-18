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

#include <boost/detail/unspecified.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/cat.hpp>

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
        new_end = combiner().rtl(boost::begin(decoded1), boost::end(decoded1)).base(); \
\
        typename range_iterator<cv2 Range2>::type \
        new_begin = combiner().ltr(boost::begin(decoded2), boost::end(decoded2)).base(); \
\
        return make_tuple( \
            make_iterator_range(boost::begin(range1), new_end), \
            make_iterator_range(new_end, boost::end(range1)), \
            make_iterator_range(boost::begin(range2), new_begin), \
            make_iterator_range(new_begin, boost::end(range2)) \
        ); \
    } \
\
    return make_tuple( \
        range1, \
        make_iterator_range(boost::end(range1), boost::end(range1)), \
        make_iterator_range(boost::end(range2), boost::end(range2)), \
        range2 \
    ); \
}
BOOST_UNICODE_FWD_2(BOOST_UNICODE_CAT_LIMITS_FWD)

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSE_CONCAT_DEF(name, nf, pipe, n) \
/** Concatenates two ranges of UTF code units and puts the result in \c out.
   Throws \c std::out_of_range if the input or resulting strings are not stream-safe.
   \pre \c Range1 and \c Range2 are in Normalized Form nf, have the same value type and are non-empty.
   \post \c out is in Normalized Form nf and is stream-safe. */ \
template<typename Range1, typename Range2, typename OutputIterator, typename... T> \
OutputIterator name##_concat(const Range1& range1, const Range2& range2, OutputIterator out, const T&... args);
#else
#define BOOST_UNICODE_COMPOSE_CONCAT_DEF(name, nf, pipe, n) \
BOOST_PP_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_COMPOSE_CONCAT_DEF_A, (name)(pipe))
#endif

/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSE_CONCAT_DEF_A(z, n, seq) \
template<typename Range1, typename Range2, typename OutputIterator BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
OutputIterator BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, seq), _concat)(const Range1& range1, const Range2& range2, OutputIterator out BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    tuple< \
        sub_range<const Range1>, \
        sub_range<const Range1>, \
        sub_range<const Range2>, \
        sub_range<const Range2> \
    > \
    t = cat_limits(range1, range2); \
     \
    out = copy(t.get<0>(), out); \
    out = pipe(joined_n(t.get<1>(), t.get<2>()), make_piped_pipe(utf_decoder(), BOOST_PP_SEQ_ELEM(1, seq)(BOOST_PP_ENUM_PARAMS(n, t))), utf_encoded_out<typename range_value<const Range1>::type>(out)).base(); \
    return copy(t.get<3>(), out); \
}

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSE_CONCATED_DEF(name, nf, pipe, n) \
/** Concatenates two ranges of UTF code units and returns the result as a lazily
   evaluated range.
   Throws \c std::out_of_range if the input or resulting strings are not stream-safe.
   \pre \c Range1 and \c Range2 are in Normalized Form nf, have the same value type and are non-empty.
   \return Lazy stream-safe range in Normalized Form nf. */ \
template<typename Range1, typename Range2, typename... T> \
detail::unspecified<void> name##_concated(const Range1& range1, const Range2& range2, const T&... args);
#else
#define BOOST_UNICODE_COMPOSE_CONCATED_DEF(name, nf, pipe, n) \
BOOST_PP_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_COMPOSE_CONCATED_DEF_A, (name)(pipe))
#endif

/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSE_CONCATED_DEF_A(z, n, seq) \
template<typename Range1, typename Range2 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
iterator_range< \
    join_iterator< \
        tuple< \
            iterator_range<typename range_iterator<const Range1>::type>, \
            iterator_range< \
                pipe_iterator< \
                    join_iterator< \
                        tuple< \
                            iterator_range<typename range_iterator<const Range1>::type>, \
                            iterator_range<typename range_iterator<const Range2>::type> \
                        > \
                    >, \
                    piped_pipe< \
                        utf_decoder, \
                        multi_pipe< \
                            BOOST_PP_SEQ_ELEM(1, seq), \
                            utf_encoder<typename range_value<const Range1>::type> \
                        > \
                    > \
                > \
            >, \
            iterator_range<typename range_iterator<const Range2>::type> \
        > \
    > \
> BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, seq), _concated)(const Range1& range1, const Range2& range2 BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    tuple< \
        sub_range<const Range1>, \
        sub_range<const Range1>, \
        sub_range<const Range2>, \
        sub_range<const Range2> \
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
                    BOOST_PP_SEQ_ELEM(1, seq)(BOOST_PP_ENUM_PARAMS(n, t)), \
                    utf_encoder<typename range_value<const Range1>::type>() \
                ) \
            ) \
        ), \
        t.get<3>() \
    ); \
}

/** INTERNAL ONLY */
#define BOOST_UNICODE_COMPOSE_CAT_DEF(name, nf, pipe, n) \
BOOST_UNICODE_COMPOSE_CONCATED_DEF(name, nf, pipe, n) \
BOOST_UNICODE_COMPOSE_CONCAT_DEF(name, nf, pipe, n)

BOOST_UNICODE_COMPOSE_CAT_DEF(composed, C, normalizer, 1)
BOOST_UNICODE_COMPOSE_CAT_DEF(decomposed, D, combine_sorter, 0)

    
} // namespace unicode
} // namespace boost

#endif

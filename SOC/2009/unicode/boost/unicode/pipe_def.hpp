#ifndef BOOST_UNICODE_PIPE_DEF_HPP
#define BOOST_UNICODE_PIPE_DEF_HPP

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/detail/unspecified.hpp>

#include <boost/iterator/pipe_iterator.hpp>

namespace boost
{

/** INTERNAL ONLY */    
template<typename Pipe>
struct identity_pipe : Pipe
{
    identity_pipe() {}
    identity_pipe(Pipe p) : Pipe(p) {}
};

/** INTERNAL ONLY */
template<typename Pipe>
identity_pipe<Pipe> make_identity_pipe(Pipe p)
{
    return identity_pipe<Pipe>(p);
}

}

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_EAGER_DEF(z, n, text) \
/** Eagerly evaluates \c unicode##::##text##r until the whole input
   range \c range has been treated, copying the result to \c out and
   returning the past-the-end output iterator. */ \
template<typename Range, typename OutputIterator, typename... T> \
OutputIterator text(const Range& range, OutputIterator out, const T&... args);
#else
#define BOOST_UNICODE_PIPE_EAGER_DEF(z, n, text) \
template<typename Range, typename OutputIterator BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
OutputIterator BOOST_PP_TUPLE_ELEM(2, 0, text)(const Range& range, OutputIterator out BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    return pipe( \
        range, \
        BOOST_PP_CAT(make_, BOOST_PP_TUPLE_ELEM(2, 1, text))(BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), r)(BOOST_PP_ENUM_PARAMS(n, t))), \
        out \
    ); \
}
#endif 

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_LAZY_DEF(z, n, text) \
/** Lazily evalutes \c unicode::##text##r by returning a range
   adapter that wraps the range \c range and converts it step-by-step as
   the range is advanced. */ \
template<typename Range, typename... T> \
detail::unspecified<void> BOOST_PP_CAT(text, d)(const Range& range, const T&... args);
#else
#define BOOST_UNICODE_PIPE_LAZY_DEF(z, n, text) \
template<typename Range BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
iterator_range< \
    pipe_iterator< \
        typename range_iterator<const Range>::type, \
        BOOST_PP_TUPLE_ELEM(2, 1, text) < \
            unicode::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), r) \
        > \
    > \
> \
BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), d)(const Range& range BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    return piped(range, BOOST_PP_CAT(make_, BOOST_PP_TUPLE_ELEM(2, 1, text))(unicode::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), r)(BOOST_PP_ENUM_PARAMS(n, t)))); \
}
#endif

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_LAZY_2_DEF(z, n, text) \
/** Lazily evalutes \c unicode::##text##r by returning a range
   adapter that wraps the range \c range and converts it step-by-step as
   the range is advanced. */ \
template<typename Range, typename... T> \
detail::unspecified<void> BOOST_PP_CAT(text, d)(Range& range, const T&... args);
#else
#define BOOST_UNICODE_PIPE_LAZY_2_DEF(z, n, text) \
template<typename Range BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
iterator_range< \
    pipe_iterator< \
        typename range_iterator<Range>::type, \
        BOOST_PP_TUPLE_ELEM(2, 1, text) < \
            unicode::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), r) \
        > \
    > \
> \
BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), d)(Range& range BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    return piped(range, BOOST_PP_CAT(make_, BOOST_PP_TUPLE_ELEM(2, 1, text))(unicode::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, text), r)(BOOST_PP_ENUM_PARAMS(n, t)))); \
}
#endif

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_REPEAT(n, macro, tuple) \
BOOST_PP_REPEAT(n, macro, BOOST_PP_TUPLE_ELEM(2, 0, tuple))
#else
#define BOOST_UNICODE_REPEAT(n, macro, tuple) \
BOOST_PP_REPEAT(n, macro, tuple)
#endif

/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_COMMON_DEF(name, wrapper, n) \
BOOST_UNICODE_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_PIPE_EAGER_DEF, (name, wrapper)) \
BOOST_UNICODE_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_PIPE_LAZY_DEF, (name, wrapper)) \
BOOST_UNICODE_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_PIPE_LAZY_2_DEF, (name, wrapper))

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_PIPE_OUTPUT_DEF(z, n, text) \
/** Lazily evalutes \c unicode::##text##r by returning an output
  iterator that wraps \c out and converts every pushed element. */ \
template<typename OutputIterator, typename... T> \
detail::unspecified<void> \
BOOST_PP_CAT(text, d_out)(OutputIterator out, const T&... args);
#else
#define BOOST_UNICODE_PIPE_OUTPUT_DEF(z, n, text) \
template<typename OutputIterator BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)> \
pipe_output_iterator< \
    OutputIterator, \
    unicode::BOOST_PP_CAT(text, r) \
> BOOST_PP_CAT(text, d_out)(OutputIterator out BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
	return piped_output(out, unicode::BOOST_PP_CAT(text, r)(BOOST_PP_ENUM_PARAMS(n, t))); \
}
#endif

/** Defines helper functions for usage of a \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly.
 * Helper functions provide a pseudo-variadic interface where they forward all the extra arguments to
 * the constructor of the \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly.
 * \arg \c name Name of the type modelling the \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly.
 * \arg \c n Maximum number of optional arguments. */
#define BOOST_UNICODE_ONE_MANY_PIPE_DEF(name, n) \
BOOST_UNICODE_PIPE_COMMON_DEF(name, one_many_pipe, n) \
BOOST_UNICODE_REPEAT(BOOST_PP_INC(n), BOOST_UNICODE_PIPE_OUTPUT_DEF, name)

/** Defines helper functions for usage of a \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly.
 * Helper functions provide a pseudo-variadic interface where they forward all the extra arguments to
 * the constructor of the \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly.
 * \arg \c name Name of the type modelling the \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly.
 * \arg \c n Maximum number of optional arguments. */
#define BOOST_UNICODE_PIPE_DEF(name, n) \
BOOST_UNICODE_PIPE_COMMON_DEF(name, identity_pipe, n)

#endif

#ifndef BOOST_ITERATOR_CONSUMER_ITERATOR_HPP
#define BOOST_ITERATOR_CONSUMER_ITERATOR_HPP

#include <boost/iterator/dummy_output_iterator.hpp>

#include <boost/concept/requires.hpp>
#include <boost/range/concepts.hpp>
#include <boost/iterator/convert_iterator.hpp>

#include <boost/iterator/segment_iterator_fwd.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>


namespace boost
{

/** Model of \c \xmlonly<conceptname>Segmenter</conceptname>\endxmlonly
 * constructed from a model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly
 * by ignoring its output. */    
template<typename Converter>
struct converter_segmenter : private Converter
{
    BOOST_CONCEPT_ASSERT((ConverterConcept<Converter>));
    
    typedef typename Converter::input_type input_type;
    
    converter_segmenter() {} // singular
    
    converter_segmenter(Converter p_) : Converter(p_)
    {
    }
    
    template<typename In>
    void ltr(In& begin, In end)
    {
        Converter::ltr(begin, end, dummy_output_iterator());
    }
    
    template<typename In>
    void rtl(In begin, In& end)
    {
        Converter::rtl(begin, end, dummy_output_iterator());
    }
};

template<typename Converter>
BOOST_CONCEPT_REQUIRES(
    ((ConverterConcept<Converter>)),
    (converter_segmenter<Converter>)
) make_converter_segmenter(Converter p)
{
    return converter_segmenter<Converter>(p);
}

/** Model of \c \xmlonly<conceptname>Segmenter</conceptname>\endxmlonly
 * constructed from a model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * and that checks for boundaries until one is reached. */    
template<typename BoundaryChecker>
struct boundary_segmenter : private BoundaryChecker
{
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<BoundaryChecker>));
    
    typedef typename BoundaryChecker::input_type input_type;
    
    boundary_segmenter() {} // singular
    
    boundary_segmenter(BoundaryChecker b_) : BoundaryChecker(b_)
    {
    }
    
    template<typename In>
    void ltr(In& begin, In end)
    {
        In b = begin;
        do
            ++begin;
        while(begin != end && !BoundaryChecker::operator()(b, end, begin));
    }
    
    template<typename In>
    void rtl(In begin, In& end)
    {
        In e = end;
        do
            --end;
        while(end != begin && !BoundaryChecker::operator()(begin, e, end));
    }
};

template<typename BoundaryChecker>
BOOST_CONCEPT_REQUIRES(
    ((BoundaryCheckerConcept<BoundaryChecker>)),
    (boundary_segmenter<BoundaryChecker>)
) make_boundary_segmenter(BoundaryChecker b)
{
    return boundary_segmenter<BoundaryChecker>(b);
}

/** Model of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * constructed from two models of \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly
 * with a model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly applied in the middle. */    
template<typename B1, typename Converter, typename B2>
struct multi_boundary
{
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B1>));
    BOOST_CONCEPT_ASSERT((ConverterConcept<Converter>));
    BOOST_CONCEPT_ASSERT((BoundaryCheckerConcept<B2>));
    
    BOOST_CONCEPT_ASSERT((Convertible<typename B1::input_type, typename Converter::input_type>));
    BOOST_CONCEPT_ASSERT((Convertible<typename Converter::output_type, typename B2::input_type>));
    
    typedef typename B1::input_type input_type;
    
    multi_boundary() // singular
    {
    }
    
    multi_boundary(B1 b1_, Converter p_, B2 b2_) : b1(b1_), p(p_), b2(b2_)
    {
    }
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        return b1(begin, end, pos)
           &&  b2(
                make_convert_iterator(begin, end, begin, p),
                make_convert_iterator(begin, end, end, p),
                make_convert_iterator(begin, end, pos, p)
               );
    }
    
private:
    B1 b1;
    Converter p;
    B2 b2;
};

template<typename B1, typename Converter, typename B2>
BOOST_CONCEPT_REQUIRES(
    ((BoundaryCheckerConcept<B1>))
    ((ConverterConcept<Converter>))
    ((BoundaryCheckerConcept<B2>))
    ((Convertible<typename B1::input_type, typename Converter::input_type>))
    ((Convertible<typename Converter::output_type, typename B2::input_type>)),
    (multi_boundary<B1, Converter, B2>)
) make_multi_boundary(B1 b1, Converter p, B2 b2)
{
    return multi_boundary<B1, Converter, B2>(b1, p, b2);
}

/** Model of \c \xmlonly<conceptname>Segmenter</conceptname>\endxmlonly
 * that adapts the elements another \c \xmlonly<conceptname>Segmenter</conceptname>\endxmlonly
 * sees with a model of \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly, assuming its \c max_output is \c 1. */
template<typename Converter, typename Segmenter>
struct converted_segmenter
{
    BOOST_CONCEPT_ASSERT((ConverterConcept<Converter>));
    BOOST_CONCEPT_ASSERT((SegmenterConcept<Segmenter>));
    BOOST_CONCEPT_ASSERT((Convertible<typename Converter::output_type, typename Segmenter::input_type>));
    
    BOOST_MPL_ASSERT(( mpl::equal_to< typename Converter::max_output, mpl::int_<1> > ));
    
    typedef typename Converter::input_type input_type;
    
    converted_segmenter() // singular
    {
    }
    
    converted_segmenter(Converter p_, Segmenter c_) : p(p_), c(c_)
    {
    }
    
    template<typename In>
    void ltr(In& begin, In end)
    {
        convert_iterator<In, Converter> b = make_convert_iterator(begin, end, begin, p);
        convert_iterator<In, Converter> e = make_convert_iterator(begin, end, end, p);
        
        c.ltr(b, e);
        begin = b.base();
    }
    
    template<typename In>
    void rtl(In begin, In& end)
    {
        convert_iterator<In, Converter> b = make_convert_iterator(begin, end, begin, p);
        convert_iterator<In, Converter> e = make_convert_iterator(begin, end, end, p);
        
        c.rtl(b, e);
        end = e.base();
    }
    
private:
    Converter p;
    Segmenter c;
};

template<typename Converter, typename Segmenter>
BOOST_CONCEPT_REQUIRES(
    ((ConverterConcept<Converter>))
    ((SegmenterConcept<Segmenter>))
    ((Convertible<typename Converter::output_type, typename Segmenter::input_type>)),
    (converted_segmenter<Converter, Segmenter>)   
) make_converted_segmenter(Converter p, Segmenter c)
{
    return converted_segmenter<Converter, Segmenter>(p, c);
}

template<typename It, typename Segmenter>
BOOST_CONCEPT_REQUIRES(
    ((InputIterator<It>))
    ((SegmenterConcept<Segmenter>))
    ((Convertible<typename InputIterator<It>::value_type, typename Segmenter::input_type>)),
    (segment_iterator<It, Segmenter>)
) make_segment_iterator(It begin, It end, It pos, Segmenter c)
{
	return segment_iterator<It, Segmenter>(begin, end, pos, c);
}

/** Range wrapper around \c boost::segment_iterator */
template<typename Range, typename Segmenter>
struct segmented_range : boost::iterator_range<
    boost::segment_iterator<
        typename boost::range_iterator<Range>::type,
        Segmenter
    >
>
{
private:
    typedef boost::segment_iterator<
        typename boost::range_iterator<Range>::type,
        Segmenter
    > Iterator;
    
public:    
    segmented_range(Iterator begin, Iterator end) : boost::iterator_range<Iterator>(begin, end)
    {
    }
};

namespace adaptors
{
    template<typename Range, typename Segmenter>
    BOOST_CONCEPT_REQUIRES(
        ((SinglePassRangeConcept<Range>))
        ((SegmenterConcept<Segmenter>))
        ((Convertible<typename range_value<const Range>::type, typename Segmenter::input_type>)),
        (segmented_range<const Range, Segmenter>)
    ) segment(const Range& range, Segmenter c)
    {
        return boost::segmented_range<const Range, Segmenter>(
            make_segment_iterator(boost::begin(range), boost::end(range), boost::begin(range), c),
            make_segment_iterator(boost::begin(range), boost::end(range), boost::end(range), c)
        );
    }
    
    template<typename Range, typename Segmenter>
    BOOST_CONCEPT_REQUIRES(
        ((SinglePassRangeConcept<Range>))
        ((SegmenterConcept<Segmenter>))
        ((Convertible<typename range_value<Range>::type, typename Segmenter::input_type>)),
        (segmented_range<Range, Segmenter>)
    ) segment(Range& range, Segmenter c)
    {
        return boost::segmented_range<Range, Segmenter>(
            make_segment_iterator(boost::begin(range), boost::end(range), boost::begin(range), c),
            make_segment_iterator(boost::begin(range), boost::end(range), boost::end(range), c)
        );
    }
} // namespace adaptors
    
} // namespace boost

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
#define BOOST_SEGMENTER_DEF(segmenter_name, segment_name)              \
namespace adaptors                                                     \
{                                                                      \
    /** Adapts the range \c range into a range of ranges segmented by
     \c segmenter_name, each subrange being a segment. */              \
    template<typename Range, typename... T>                            \
    boost::segmented_range<                                            \
        Range,                                                         \
        segmenter_name                                                 \
    >                                                                  \
    segment_name(Range&& range, const T&...);                          \
}
#else
#define BOOST_SEGMENTER_DEF(segmenter_name, segment_name)              \
namespace adaptors                                                     \
{                                                                      \
    template<typename Range>                                           \
    boost::segmented_range<                                            \
        const Range,                                                   \
        segmenter_name                                                 \
    >                                                                  \
    segment_name(const Range& range)                                   \
    {                                                                  \
        return boost::adaptors::segment(range, segmenter_name());      \
    }                                                                  \
                                                                       \
    template<typename Range>                                           \
    boost::segmented_range<                                            \
        Range,                                                         \
        segmenter_name                                                 \
    >                                                                  \
    segment_name(Range& range)                                         \
    {                                                                  \
        return boost::adaptors::segment(range, segmenter_name());      \
    }                                                                  \
                                                                       \
    template<typename Range, typename T0>                              \
    boost::segmented_range<                                            \
        const Range,                                                   \
        segmenter_name                                                 \
    >                                                                  \
    segment_name(const Range& range, const T0& t0)                     \
    {                                                                  \
        return boost::adaptors::segment(range, segmenter_name(t0));    \
    }                                                                  \
                                                                       \
    template<typename Range, typename T0>                              \
    boost::segmented_range<                                            \
        Range,                                                         \
        segmenter_name                                                 \
    >                                                                  \
    segment_name(Range& range, const T0& t0)                           \
    {                                                                  \
        return boost::adaptors::segment(range, segmenter_name(t0));    \
    }                                                                  \
}
#endif

#endif

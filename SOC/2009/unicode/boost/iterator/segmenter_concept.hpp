#ifndef BOOST_ITERATOR_CONSUMER_CONCEPT_HPP
#define BOOST_ITERATOR_CONSUMER_CONCEPT_HPP

#include <boost/concept_archetype.hpp>
#include <boost/concept_check.hpp>

namespace boost
{

/** Concept checking class for the \c \xmlonly<conceptname>Segmenter</conceptname>\endxmlonly concept */    
template<typename X>
struct SegmenterConcept : DefaultConstructible<X>, CopyConstructible<X>
{
    typedef typename X::input_type input_type;
    
    BOOST_CONCEPT_USAGE(SegmenterConcept)
    {
        X c;
        begin = c.ltr(begin, end);
        end = c.rtl(begin, end);
    }
    
private:
    typedef bidirectional_iterator_archetype<input_type> in_type;

    in_type begin;
    in_type end;
};

/** Concept checking class for the \c \xmlonly<conceptname>BoundaryChecker</conceptname>\endxmlonly concept */
template<typename X>
struct BoundaryCheckerConcept : DefaultConstructible<X>, CopyConstructible<X>
{
    typedef typename X::input_type input_type;
    
    BOOST_CONCEPT_USAGE(BoundaryCheckerConcept)
    {
        X b;
        result = b(begin, end, pos);
    }
    
private:
    typedef bidirectional_iterator_archetype<input_type> in_type;
    in_type begin;
    in_type end;
    in_type pos;
    
    bool result;
};

} // namespace boost

#endif

#ifndef BOOST_ITERATOR_PIPE_CONCEPT_HPP
#define BOOST_ITERATOR_PIPE_CONCEPT_HPP

#include <boost/concept_archetype.hpp>
#include <boost/concept_check.hpp>

#include <utility>

namespace boost
{

/** Concept checking class for the \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly concept */    
template<typename X>
struct ConverterConcept : DefaultConstructible<X>, CopyConstructible<X>
{
    typedef typename X::input_type input_type;
    typedef typename X::output_type output_type;
    
    BOOST_CONCEPT_USAGE(ConverterConcept)
    {
        X convert;
        p = convert.ltr(begin, end, out);
        p = convert.rtl(begin, end, out);
    }
    
private:
    typedef bidirectional_iterator_archetype<input_type> in_type;
    typedef output_iterator_archetype<output_type> out_type;

    in_type begin;
    in_type end;
    out_type out;
    std::pair<in_type, out_type> p;
};

/** Concept checking class for the \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly concept */
template<typename X>
struct OneManyConverterConcept : ConverterConcept<X>
{
    
    BOOST_CONCEPT_USAGE(OneManyConverterConcept)
    {
        X convert;
        out = convert(in, out);
    }
    
private:
    typedef output_iterator_archetype<typename ConverterConcept<X>::output_type> out_type;
    
    typename ConverterConcept<X>::input_type in;
    out_type out;
};

} // namespace boost

#endif

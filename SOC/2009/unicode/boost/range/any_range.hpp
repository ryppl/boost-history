#ifndef BOOST_RANGE_ANY_RANGE_HPP
#define BOOST_RANGE_ANY_RANGE_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/iterator/any_iterator.hpp>

namespace boost
{

template<
    typename Value,
    typename CategoryOrTraversal,
    typename Reference = Value&,
    typename Difference = std::ptrdiff_t
>
struct any_range : iterator_range< any_iterator<Value, CategoryOrTraversal, Reference, Difference> >
{
    template<typename T>
    any_range(const T& t) : iterator_range< any_iterator<Value, CategoryOrTraversal, Reference, Difference> >(t)
    {
    }
};

template<typename Value, typename Reference = const Value, typename Difference = std::ptrdiff_t>
struct lazy_range : any_range<Value, bidirectional_traversal_tag, Reference, Difference>
{
    template<typename T>
    lazy_range(const T& t) : any_range<Value, bidirectional_traversal_tag, Reference, Difference>(t)
    {
    }
};

} // namespace boost

#endif


#ifndef BOOST_GRAPHS_LABEL_HPP
#define BOOST_GRAPHS_LABEL_HPP

#include <functional>

#include <boost/none.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace graphs {

/**
 * The label traits class provides a mechanism for generically accessing
 * information about an object's built in label. A label is data associated
 * with either a vertex or edge.
 */
template <typename Object>
struct label_traits
{
    typedef typename Object::label_type label_type;
};

/** @name Label Access
 * Return a reference to the object's label. The generic implementation requires
 * the object to have a non-const label() member.
 */
//@{
template <typename Object>
typename label_traits<Object>::label_type& label(Object& x)
{ return x.label(); }

template <typename Object>
typename label_traits<Object>::label_type const& label(Object const& x)
{ return x.label(); }
//@}

/**
 * This is a simple template wrapper for building comparison functors for
 * labelled objects.
 *
 * This requires that the first/second argument types are the same as the
 * Object's label type.
 */
template <typename Object, typename Compare>
struct labelled_compare : std::binary_function<Object, Object, bool>
{
    inline bool operator()(Object const& a, Object const& b) const
    { return comp(label(a), label(b)); }

    Compare comp;
};

/** Equality comparison of labelled objects. */
template <
    typename Object,
    typename Label = typename label_traits<Object>::label_type>
struct labelled_equal_to : labelled_compare<Object, std::equal_to<Label>> { };

/** Less than comparison of labelled objects. */
template <
    typename Object,
    typename Label = typename label_traits<Object>::label_type>
struct labelled_less : labelled_compare<Object, std::less<Label>> { };


} } /* namespace boost::graphs */


#endif

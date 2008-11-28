
#ifndef BOOST_GRAPHS_LABEL_HPP
#define BOOST_GRAPHS_LABEL_HPP

#include <functional>

#include <boost/none.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace graphs {

/** @name Label Traits
 * The label traits class provides a mechanism for generically accessing
 * information about an object's built in label. A label is data associated
 * with either a vertex or edge.
 */
//@{
template <typename Object>
struct label_traits
{
    typedef typename Object::label_type label_type;
};

template <typename Object>
struct label_traits<Object const> : label_traits<Object> { };
//@}

/** @name Label Access
 * Return a reference to the object's label. The generic implementation requires
 * the object to have a non-const label() member.
 *
 * @note These functions are currently disabled because they seem to be hiding
 * specializations. This is generally okay since these implemntations are
 * never actually valid.
 *
 * @bug Overloading and Instantiation Problem
 * There is some strange interplay with overloading during parsing and
 * instantiation. Apparently, if any overloads are visible during parsing, then
 * the selection of functions is restricted to that set of overloads during
 * instantiation. If no overloads are found, then name resolution is fully
 * deferred until instantiation.
 *
 * Here's the problem. Defining these functions will cause has_label_equal_to()
 * to refer to these functions - which will almost always result in a compiler
 * error when the function is instantiated in vs::dispatch_find(). Leaving these
 * out will allow the correct overloads to be found.
 *
 * One solution might be to create a firewall by invoking a templated get_label
 * function which is specialized by different parts of the library.
 */
//@{
// template <typename Object>
// typename label_traits<Object>::label_type& label(Object& x)
// { return x.label(); }
//
// template <typename Object>
// typename label_traits<Object>::label_type const& label(Object const& x)
// { return x.label(); }
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

/**
 * A helper for find_if, returns true if an object has the given label.
 * @note This is not a proper function object since the arguments to the
 * function are not known ahead of time.
 */
template <typename Label>
struct has_label_equal_to_func
{
    has_label_equal_to_func(Label const& l)
        : lbl(l)
    { }

    template <typename Object>
    bool operator()(Object const& x) const
    { return label(x) == lbl; }

    Label const& lbl;
};

// A generator the struct above. This is parameterized on the store rather th
template <typename Label>
has_label_equal_to_func<Label> has_label_equal_to(Label const& l)
{ return has_label_equal_to_func<Label>(l); }

} } /* namespace boost::graphs */


#endif

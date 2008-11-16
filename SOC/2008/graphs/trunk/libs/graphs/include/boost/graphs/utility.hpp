
#ifndef BOOST_GRAPHS_UTILITY_HPP
#define BOOST_GRAPHS_UTILITY_HPP

#include <boost/descriptors.hpp>

namespace boost { namespace graphs {

/**
 * @internal
 * The insertion result structure encapsulates information about the results
 * of an insertion. There are a number of possible results from insertsions.
 * First, the result could succeed, returning a descriptor to the newly inserted
 * element. The insertion could also fail because the element is already in the
 * target container (e.g., sets and maps). Third, the insertion could fail
 * because of some policy (e.g., no self loops or loose edges).
 *
 * The result type describes the result of the insertion operation. The
 * insertion can a) insert a new object, b) retain a previous object, or c)
 * do nothing becaue of a failure (mentioned above).
 * 
 * @param Desc The type of descriptor being returned.
 * @todo Should the results be more specific?
 */
template <typename Desc>
struct insertion_result
{
    typedef Desc descriptor_type;
    enum result_type { insert, retain, none };

    inline insertion_result()
        : value(), type(none)
    { }

    inline insertion_result(descriptor_type d)
        : value(d), type(insert)
    { }

    inline insertion_result(descriptor_type d, result_type t)
        : value(d), type(t)
    { }

    // A little help for unique associative containers.
    template <typename Container>
    inline insertion_result(Container& c, std::pair<typename Container::iterator, bool> i)
        : value(make_descriptor(c, i.first))
        , type(i.second ? insert : (i.first  != c.end() ? retain : none))
    { }

    /** Return true if the insertion succeeded, inserting a new element. */
    inline bool succeeded() const
    { return type == insert; }

    /**
     * Returns true if the insertion retains a previous value, returning a
     * descriptor to the existing object.
     */
    inline bool retained() const
    { return type == retain; }

    /**
     * Returns true if the insertion failed due to an assertion or policy.
     */
    inline bool failed() const
    { return type == none; }

    descriptor_type value;
    result_type     type;
};

// Helper functions for insertion results
template <typename Desc>
inline insertion_result<Desc> make_result()
{ return insertion_result<Desc>(); }

template <typename Desc>
inline insertion_result<Desc> make_result(Desc d)
{ return insertion_result<Desc>(d); }

template <typename Desc>
inline insertion_result<Desc>
make_result(Desc d, typename insertion_result<Desc>::result_type t)
{ return insertion_result<Desc>(d, t); }

template <typename Cont>
inline insertion_result<typename descriptor_traits<Cont>::descriptor_type>
make_result(Cont& c, std::pair<typename Cont::iterator, bool> i)
{ return insertion_result<typename descriptor_traits<Cont>::descriptor_type>(c, i); }

/**
 * @internal
 * A functor that returns true when we can find a propertied object with the
 * same value as those given in the cosntructor. This works for both vertices
 * and edges.
 * @param Properties The type of properties being compared.
 * @todo This goes away with lambda expressions. Used in vertex_[v|l]::find().
 */
template <typename Label>
struct label_finder
{
    inline label_finder(Label const& l)
        : label(l)
    { }

    template <typename Object>
    inline bool operator()(Object const& o) const
    { return o.label() == label; }

    Label const& label;
};

template <typename Label>
inline label_finder<Label>
find_label(Label const& l)
{ return label_finder<Label>(l); }

/**
 * @internal
 * A functor that returns true when the first value of a pair (or other tuple
 * with a named first element) matches that of the constructed value.
 * @param First The type of the first element.
 * @todo This goes away with lambda expression (in property_*::find and
 * incidence_*::find).
 */
template <typename First>
struct first_finder
{
    inline first_finder(First const& x)
        : value(x)
    { }

    template <typename Object>
    inline bool operator()(Object const& o) const
    { return o.first == value; }

    First const& value;
};

template <typename First>
inline first_finder<First>
find_first(First const& props)
{ return first_finder<First>(props); }

} } /* namespace boost::graphs */

#endif

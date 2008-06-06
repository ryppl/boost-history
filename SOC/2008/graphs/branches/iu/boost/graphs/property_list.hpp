
#ifndef PROPERTY_LIST_HPP
#define PROPERTY_LIST_HPP

#include <list>

// Forward descriptor
template <typename Iter> class proplist_descriptor;

/**
 * The property list implements global list of properties for node-based edge
 * storage. Note that we can get away with only a list because the edge
 * addition logic is implemented by the incidence list.
 */
template <typename Props, typename Alloc>
class property_list
{
    typedef std::list<Props, Alloc> store_type;
public:
    typedef Props property_type;
    typedef proplist_descriptor<typename store_type::iterator> property_descriptor;

    inline property_list();

    // Add properties
    inline property_descriptor add();
    inline property_descriptor add(property_type const&);

    // Remove properties
    inline void remove(property_descriptor);

    // Property access.
    inline property_type& properties(property_descriptor);

private:
    store_type _props;
};

/**
 * Partially specialize the property list for the case where the user does
 * not want properties. This will completely remove the property list from
 * the graph, instead creating a simple index enumerator.
 */
template <typename Alloc>
class property_list<none, Alloc>
{
public:
    typedef std::size_t property_descriptor;
};


template <typename P, typename A>
property_list<P,A>::property_list()
    : _props()
{ }

/**
 * Add an empty or default property to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add()
{
    return add(property_type());
}

/**
 * Add the specified properties to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add(property_type const& p)
{
    return _props.insert(_props.end(), p);
}

/**
 * Remove the indicated property from the list. This invalidates any
 * descriptors and iterators to the given property, but no others.
 *
 * @complexity O(1)
 */
template <typename P, typename A>
void
property_list<P,A>::remove(property_descriptor p)
{
    _props.erase(p.iter);
}

/**
 * Return the properties corresponding to the given descriptor.
 */
template <typename P, typename A>
typename property_list<P,A>::property_type&
property_list<P,A>::properties(property_descriptor p)
{
    return *p.iter;
}

/**
 * The proplist descriptor provides a wrapper around a list iterator that
 * provides comparability for the underlying iterator. Note that the comparison
 * has little to do with actual ordering of elements. This is to say that
 * i \< j !=> *i \< *j. This just provides a mechanism for ordering the
 * descriptors.
 */
template <typename Iter>
struct proplist_descriptor
{
    inline proplist_descriptor(Iter const&);

    inline bool operator==(proplist_descriptor const&) const;
    inline bool operator<(proplist_descriptor const&) const;

    Iter iter;
};

template <typename I>
proplist_descriptor<I>::proplist_descriptor(I const& iter)
    : iter(iter)
{ }

template <typename I>
bool
proplist_descriptor<I>::operator==(proplist_descriptor const& x) const
{
    return iter == x.iter;
}

template <typename I>
bool
proplist_descriptor<I>::operator<(proplist_descriptor const& x) const
{
    return &*iter < &*x.iter;
}

// Property list algorithm objects
template <typename PropList>
struct eraser
{
    eraser(PropList& props)
    { }
};


#endif


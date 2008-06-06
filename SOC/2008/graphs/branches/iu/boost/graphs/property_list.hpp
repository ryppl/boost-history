
#ifndef PROPERTY_LIST_HPP
#define PROPERTY_LIST_HPP

#include <list>

// Forward descriptor
template <typename Iter> class proplist_descriptor;

// The property list implements global list of properties for node-based
// edge storage. Note that we can get away with only a list because the
// edge addition logic is implemented by the incidence list.

template <typename Props, typename Alloc>
class property_list
{
    typedef std::list<Props, Alloc> store_type;
public:
    typedef Props property_type;
    typedef proplist_descriptor<typename store_type::iterator> property_descriptor;

    property_list();

    property_descriptor add();
    property_descriptor add(property_type const&);

private:
    store_type _props;
};

template <typename Alloc>
class property_list<none, Alloc>
{
public:
    typedef std::size_t property_descriptor;
};

// The proplist descriptor provides a wrapper around a list iterator
// that provides comparability for the underlying iterator. Note that
// the comparison has little to do with actual ordering of eleemnts.
// This is to say that i < j !=> *i < *j. This just provides a mechanism
// for ordering the descriptors.

template <typename Iter>
class proplist_descriptor
{
    proplist_descriptor(Iter const& iter)
        : iter(iter)
    { }

    bool operator <(proplist_descriptor const& x) const
    { return &*iter < &*x.iter; } 

    Iter iter;
};

template <typename P, typename A>
property_list<P,A>::property_list()
    : _props()
{ }

template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add()
{
    return add(property_type());
}

template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add(property_type const& p)
{
    return _props.insert(_props.end(), p);
}


#endif


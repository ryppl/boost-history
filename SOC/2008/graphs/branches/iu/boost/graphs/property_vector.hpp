
#ifndef PROPERTY_VERTEX_HPP
#define PROPERTY_VERTEX_HPP

#include <vector>

// The property vector implements a vector-based global property
// store for vector-based edge storage. Assuming, of course, that
// there are actually edge properties.

template <typename Props, typename Alloc>
class property_vector
{
    typedef std::vector<Props, Alloc> store_type;
public:
    typedef Props property_type;
    typedef std::size_t property_descriptor;

    property_vector();

    property_descriptor add();
    property_descriptor add(property_type const&);

private:
    store_type _props;
};

template <typename Alloc>
class property_vector<none, Alloc>
{
public:
    typedef std::size_t property_descriptor;
};

template <typename P, typename A>
property_vector<P,A>::property_vector()
    : _props()
{ }

template <typename P, typename A>
typename property_vector<P,A>::property_descriptor
property_vector<P,A>::add()
{
    return add(property_type());
}

template <typename P, typename A>
typename property_vector<P,A>::property_descriptor
property_vector<P,A>::add(property_type const& p)
{
    _props.push_back(p);
    return _props.size() - 1;
}

#endif


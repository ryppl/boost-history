
#ifndef BOOST_GRAPH_UTILITY_ORDERED_PAIR_HPP
#define BOOST_GRAPH_UTILITY_ORDERED_PAIR_HPP

namespace boost {
namespace graphs {

/**
 * The ordered pair template is essentially a homogenous container of two
 * values. This is essentially the same as std::pair<T, T>, although this
 * class provides a slightly different interface.
 */
template <typename T>
class ordered_pair
{
public:
    typedef T value_type;

    ordered_pair();
    ordered_pair(ordered_pair const& x);
    ordered_pair(T const& f, T const& s);

    T const& first() const;
    T const& second() const;

private:
    std::pair<T, T> _pair;
};

template <typename T>
ordered_pair<T>::ordered_pair()
    : _pair()
{ }

template <typename T>
ordered_pair<T>::ordered_pair(ordered_pair const& x)
    : _pair(x._pair)
{ }

template <typename T>
ordered_pair<T>::ordered_pair(T const& f, T const& s)
    : _pair(f, s)
{ }

template <typename T>
T const&
ordered_pair<T>::first() const
{ return _pair.first; }


template <typename T>
T const&
ordered_pair<T>::second() const
{ return _pair.second; }

/**
 * Make an ordered pair over the two values.
 */
template <typename T>
ordered_pair<T>
make_ordered_pair(T const& f, T const& s)
{
    ordered_pair<T> x(f, s);
    return x;
}

} /* namespace graphs */
} /* namespace boost */

#endif

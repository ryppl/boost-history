
#ifndef UNORDERED_PAIR_HPP
#define UNORDERED_PAIR_HPP

#include <functional>

#include <boost/functional/hash.hpp>

/**
 * The unordered pair template provides a homogenously typed 2-element
 * whose values are unordered. By unordered, we simply mean that two pairs
 * {x, y} and {y, x} are actually the same pair. The order of the objects
 * within the pair does not matter.
 *
 * Note that unordered pairs do not have mutators for the first and second
 * members. hese are not exposed since modifying only one key at a time can
 * cause problems. The better solution is to construct pairs on the fly and
 * copy them in their entirety.
 */
template <typename T, typename Compare = std::less<T> >
class unordered_pair
{
public:
    typedef T value_type;
    typedef Compare compare;

    unordered_pair();
    unordered_pair(unordered_pair const& x);
    unordered_pair(Compare const& comp);
    unordered_pair(T const& f, T const& s);
    unordered_pair(T const& f, T const& s, Compare const& comp);

    T const& first() const;
    T const& second() const;
    Compare comp() const;

private:
    void order();

private:
    std::pair<T, T> _pair;
    Compare _comp;
};

template <typename T, typename C>
unordered_pair<T,C>::unordered_pair()
    : _pair()
    , _comp()
{ }

template <typename T, typename C>
unordered_pair<T,C>::unordered_pair(unordered_pair const& x)
    : _pair(x._pair)
    , _comp(x._comp)
{ }

template <typename T, typename C>
unordered_pair<T,C>::unordered_pair(C const& comp)
    : _pair()
    , _comp(comp)
{ }

template <typename T, typename C>
unordered_pair<T,C>::unordered_pair(T const& f, T const& s)
    : _pair(f, s)
    , _comp()
{ order(); }


template <typename T, typename C>
unordered_pair<T,C>::unordered_pair(T const& f, T const& s, C const& comp)
    : _pair(f, s)
    , _comp(comp)
{ order(); }

template <typename T, typename C>
T const&
unordered_pair<T,C>::first() const
{ return _pair.first; }

template <typename T, typename C>
T const&
unordered_pair<T,C>::second() const
{ return _pair.second; }

/**
 * Return a copy of the comparator used by the unordered pair.
 */
template <typename T, typename C>
C
unordered_pair<T,C>::comp() const
{ return _comp; }

template <typename T, typename C>
void
unordered_pair<T,C>::order()
{
    // If the elements are out of order, reorder them.
    using std::swap;
    if(_comp(_pair.second, _pair.first)) {
        swap(_pair.first, _pair.second);
    }
}

// Convenience functions and Operators

template <typename T, typename C>
inline bool
operator==(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return a.first() == b.first() && a.second() == b.second(); }

template <typename T, typename C>
inline bool
operator!=(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return !(a == b); }

template <typename T, typename C>
inline bool
operator<(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return std::make_pair(a.first(), a.second()) < std::make_pair(b.first(), b.second()); }

template <typename T, typename C>
inline bool
operator>(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return b < a; }

template <typename T, typename C>
inline bool
operator<=(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return !(b < a); }

template <typename T, typename C>
inline bool
operator>=(unordered_pair<T,C> const& a, unordered_pair<T,C> const& b)
{ return !(a < b); }

/** Compute the hash value of the unordered pair. */
template <typename T, typename C>
inline std::size_t
hash_value(unordered_pair<T,C> const& x)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, x.first());
    boost::hash_combine(seed, x.second());
    return seed;
}

/**
 * Make an ordered pair over the two values.
 */
template <typename T>
unordered_pair<T>
make_unordered_pair(T const& f, T const& s)
{ return unordered_pair<T>(f, s); }

/**
 * A swap-like sort function that will "unorder" two objects.
 */
template <typename T>
void
reorder(T& a, T& b)
{
    unordered_pair<T> p(a, b);
    a = p.first();
    b = p.second();
}

#endif

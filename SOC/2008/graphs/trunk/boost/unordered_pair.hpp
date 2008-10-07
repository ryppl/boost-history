
#ifndef BOOST_UNORDERED_PAIR_HPP
#define BOOT_UNORDERED_PAIR_HPP

#include <functional>

#include <boost/functional/hash.hpp>

namespace boost {

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

    /** @name Constructors */
    //@{
    inline unordered_pair(Compare const& comp = Compare())
        : _pair(), _comp()
    { }

    inline unordered_pair(unordered_pair const& x)
        : _pair(x._pair), _comp(x._comp)
    { }

    inline unordered_pair(unordered_pair&& x)
        : _pair(std::move(x._pair)), _comp(std::move(x._comp))
    { }

    inline unordered_pair(T const& f, T const& s, Compare const& comp = Compare())
        : _pair(f, s), _comp()
    { order(); }
    //@}

    /** @name Operators */
    //@{
    inline unordered_pair& operator=(unordered_pair const& x)
    { return swap(unordered_pair(x)); }

    inline unordered_pair& operator=(unordered_pair&& x)
    { return swap(x); }
    //@}

    /** @name Accessors */
    //@{
    inline T const& first() const
    { return _pair.first; }

    inline T const& second() const
    { return _pair.second; }

    inline Compare comp() const
    { return _comp; }
    //@}

    inline unordered_pair& swap(unordered_pair&& x)
    { _pair.swap(x._pair); return *this; }

private:
    void order();

private:
    std::pair<T, T> _pair;
    Compare _comp;
};

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
 * A swap-like sort function that will "unorder" two objects using the given
 * comparator, which defaults to std::less.
 */
template <typename T, typename Comp = std::less<T>>
void
reorder(T& a, T& b, Comp = Comp())
{
    unordered_pair<T, Comp> p(a, b);
    a = p.first();
    b = p.second();
}

} /* namespace boost */

#endif

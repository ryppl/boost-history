
#ifndef BLOB_HPP
#define BLOB_HPP

#include <cstring>

#include <boost/functional/hash.hpp>

/**
 * The blob type is used to allocate a small buffer of memory that can be
 * reinterpreted as a type of the same size. This is used solely as a means
 * of deferring the immediate need for the type of an object if you can guess
 * its size.
 *
 * One can imagine this stands for "Binary Little OBject". Little because you
 * probably don't want to copy these around all over the place.
 *
 * If you're going to write something hacky, you may as well try to make it a
 * little bit pretty.
 */
template <unsigned int N>
struct blob
{
    inline blob()
        : mem()
    { std::memset(mem, 0, N); }

    inline blob(blob const& x)
        : mem()
    { std::memcpy(mem, x.mem, N); }

    template <typename T>
    inline blob(const T& x)
        : mem()
    { put(x); }

    /** Put the value of x into the blob object. */
    template <typename T>
    inline void put(T const& x)
    {
        static_assert(sizeof(T) == N, "Putting value of wrong size");
        char const* p = reinterpret_cast<char const*>(&x);
        std::memcpy(mem, p, N);
    }

    /** Get the value of the blob object, returning it as an object of type T. */
    template <typename T>
    inline T& get() const
    {
        static_assert(sizeof(T) == N, "Getting value of wrong size");
        return *reinterpret_cast<T*>(mem);
    }

    /** @name Equality Comparable
     * Two blob objects (of the same size) are equal iff their memory contents
     * are the same.
     */
    //@{
    inline bool operator==(const blob& x) const
    { return std::memcmp(mem, x.mem, N) == 0; }

    inline bool operator!=(const blob& x) const
    { return !operator==(x); }
    //@}

    /** @name Less Than Comparable
     * A blob is less than another (of the same size) iff its memory contents
     * are byte-wise less than those of the other. This comparison does not
     * guarantee that the stored object is less than the other, only that its
     * bit representation is bite-wise less, which is the same as numerically
     * less on little (or is it big) endian systems. The big thing is that you
     * shouldn't attach any real meaning on the ordering.
     */
    //@{
    inline bool operator<(const blob& x) const
    { return std::memcmp(mem, x.mem, N) < 0; }

    inline bool operator>(const blob& x) const
    { return std::memcmp(mem, x.mem, N) > 0; }

    inline bool operator<=(const blob& x) const
    { return std::memcmp(mem, x.mem, N) <= 0; }

    inline bool operator>=(const blob& x) const
    { return std::memcmp(mem, x.mem, N) >= 0; }
    //@}

    mutable char mem[N];
};

template <unsigned int N>
std::size_t
hash_value(blob<N> const& x)
{
    using boost::hash_value;
    std::size_t seed = 0;
    for(int i = 0; i < N; ++i) {
        boost::hash_combine(seed, x.mem[i]);
    }
    return seed;
}

#endif
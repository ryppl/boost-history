
#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

#include <algorithm>

#include <boost/static_assert.hpp>

/**
 * The placeholder type is used to allocate a small buffer of memory that can
 * be reinterpreted as a type of the same size. This is used solely as a means
 * of deferring the immediate need for the type of an object if you can guess
 * its size.
 *
 * If you're going to write something hacky, you may as well try to make it a
 * little bit pretty.
 */
template <int N>
struct placeholder
{
    inline placeholder()
        : mem()
    { std::fill(mem, mem + N, 0); }

    inline placeholder(placeholder const& x)
        : mem()
    { copy(x.mem, x.mem + N, mem); }

    template <typename T>
    inline placeholder(const T& x)
        : mem()
    { put(x); }

    /** Put the value of x into the placeholder. */
    template <typename T>
    inline void put(T const& x)
    {
        BOOST_STATIC_ASSERT(sizeof(T) == N);
        char const* p = reinterpret_cast<char const*>(&x);
        copy(p, p + N, mem);
    }

    /** Get the value of the placeholder and return it as an object of type T. */
    template <typename T>
    inline T& get() const
    {
        BOOST_STATIC_ASSERT(sizeof(T) == N);
        return *reinterpret_cast<T*>(mem);
    }

    mutable char mem[N];
};

/** A useful metafunction for generating placeholder types. */
template <typename T>
struct typeholder
{ typedef placeholder<sizeof(T)> type; }

#endif

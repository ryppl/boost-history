
#ifndef BOOST_WEAK_OPTIONAL_HPP
#define BOOST_WEAK_OPTIONAL_HPP

#include <utility>

namespace boost {

/**
 * This is purely a work-around for the boost::optional<T> class, which does
 * not seem to work with GCC-4.4. This works basically the same way as optional,
 * but isn't quite as aggressive about some of its exception guarantees and
 * isn't nearly as graceful.
 */
template <typename T>
struct optional
{
    // Does not initialize the optional.
    optional()
        : _ptr(0)
    { }

    // Copy the information held by the other optional value.
    optional(optional const& x)
        : optional()
    { if(x._ptr) copy_construct(*x._ptr); }

    // Move constructor initializes the otional with x.
    optional(optional&& x)
        : optional()
    { swap(x); }

    // Value constructor initalizes the optional with a copy of the value of x.
    optional(T const& x)
        : optional()
    { copy_construct(x); }

    ~optional()
    { destruct(); }

    // Assignment operators
    optional& operator=(optional const& x)
    { return swap(optional(x)); }

    optional& operator=(optional&& x)
    { return swap(optional().swap(x)); }

    optional& operator=(T const& x)
    { return swap(optional(x)); }

    T& operator*()
    { return get(); }

    T const& operator*() const
    { return get(); }

    T* operator->()
    { return _ptr; }

    T const* operator->() const
    { return _ptr; }

    operator bool() const
    { return _ptr != 0; }

    bool valid() const
    { return _ptr != 0; }

    T& get()
    { return *_ptr; }

    T const& get() const
    { return *_ptr; }

    // Make sure that the pointer is local.
    void reify()
    { _ptr = reinterpret_cast<T*>(_buf); }

    // Reset the internal pointer to indicate uninitialized.
    void reset()
    { _ptr = 0; }


    optional& swap(optional&& x)
    {
        using std::swap;
        swap(_buf, x._buf);

        // Swapping the state of these pointers is a little weird. If they're
        // both set or both null, then we don't have to do anything. If one
        // is set and the other null, then we have to emulate a swap by reifying
        // the pointer that was null and resetting the one that wasn't.
        if(!_ptr && x._ptr) {
            reify();
            x.reset();
        }
        else if(_ptr && !x._ptr) {
            reset();
            x.reify();
        }

        return *this;
    }

    void default_construct()
    { _ptr = new(_buf) T(); }

    void copy_construct(T const& x)
    { _ptr = new(_buf) T(x); }

    void destruct()
    { if(_ptr) _ptr->~T(); }

    unsigned char _buf[sizeof(T)];
    T* _ptr;
};

}

#endif

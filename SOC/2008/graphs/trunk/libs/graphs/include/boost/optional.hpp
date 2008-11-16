
#ifndef BOOST_OPTIONAL_HPP
#define BOOST_OPTIONAL_HPP

#include <utility>

namespace boost {

/**
 * This is purely a work-around for the boost::optional<T> class, which does
 * not seem to work with GCC-4.4. This works basically the same way as optional,
 * but isn't quite as aggressive about some of its exception guarantees and
 * isn't nearly as graceful.
 */
template <typename T>
class optional
{
    typedef optional<T> this_type;
    typedef T* this_type::* unspecified_bool_type;
public:
    typedef T value_type;

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

    inline optional& operator=(optional&& x)
    { return swap(optional().swap(x)); }

    inline optional& operator=(T const& x)
    { return swap(optional(x)); }

    inline bool operator==(optional const& x) const
    { return get() != x.get(); }

    inline bool operator!=(optional const& x) const
    { return !(*this == x); }

    T& operator*()
    { return get(); }

    T const& operator*() const
    { return get(); }

    T* operator->()
    { return _ptr; }

    T const* operator->() const
    { return _ptr; }

    operator unspecified_bool_type() const
    { return _ptr ? &this_type::_ptr : 0; }

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

/**
 * Specialize the optional type for pointers. This basically removes the
 * encoding of the object into the character buffer. The reason that this is
 * specilized on pointers is that the null pointer is a built-in indicator of
 * optionality. If null, the pointer does not exist.
 *
 * The purpose of this overload is to make optional ptrs work mostly just like
 * regular pointers.
 * 
 * @todo Should this include ptr math to make it look like an iterable ptr?
 */
template <typename T>
class optional<T*>
{
    typedef optional<T*> this_type;
    typedef T* this_type::* unspecified_bool_type;
public:
    typedef T* value_type;

    optional() : _ptr(0) { }
    optional(optional const& x) : _ptr(x._ptr) { }
    optional(optional&& x) : _ptr(std::move(x._ptr)) { }
    optional(T* x) : _ptr(x) { }

    optional& operator=(optional const& x)
    { return swap(optional(x)); }

    optional& operator=(optional&& x)
    { return swap(x); }

    optional& operator=(T* x)
    { return swap(optional(x)); }

    inline bool operator==(optional const& x) const
    { return _ptr == x._ptr; }

    inline bool operator!=(optional const& x) const
    { return _ptr != x._ptr; }

    T& operator*()
    { return get(); }

    T const& operator*() const
    { return get(); }

    T* operator->()
    { return _ptr; }

    T const* operator->() const
    { return _ptr; }

    operator unspecified_bool_type() const
    { return _ptr ? &this_type::_ptr : 0; }

    bool valid() const
    { return _ptr != 0; }

    T& get()
    { return *_ptr; }

    T const& get() const
    { return *_ptr; }

    // Reset the internal pointer to indicate uninitialized.
    void reset()
    { _ptr = 0; }

    optional& swap(optional&& x)
    {
        using std::swap;
        swap(_ptr, x._ptr);
        return *this;
    }

    T* _ptr;
};

} /* namespace boost */

#endif

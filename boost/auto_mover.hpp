//-----------------------------------------------------------------------------
// boost auto_mover.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_AUTO_MOVER_HPP
#define BOOST_AUTO_MOVER_HPP

#include "boost/aligned_storage.hpp"
#include "boost/move.hpp"

namespace boost {

namespace detail {

    // class storage_holder
    //
    // Works around problem of noncopyable aligned_storage under GCC 3.2.
    // Is simple, does not introduce problems for other compilers.
    //
    template <typename Storage>
    class storage_holder
    {
    private: // representation
        Storage storage_;

    public: // structors
        storage_holder()
        {
        }

    public: // override noncopyable restriction on Storage
        storage_holder(const storage_holder&)
        {
        }

        storage_holder& operator=(const storage_holder&)
        {
        }

    public: // accessors
        void* address()
        {
            return storage_.address();
        }

        const void* address() const
        {
            return storage_.address();
        }
    };

    // struct auto_mover_ref
    //
    // Similar in function to std::auto_ptr_ref.
    //
    template <typename T>
    struct auto_mover_ref
    {
        T& ref_;

    public:
        auto_mover_ref(T& ref)
            : ref_(ref)
        {
        }
    };
} // namespace detail

//////////////////////////////////////////////////////////////////////////
// class template auto_mover
//
// auto_ptr-esque wrapper simplifies move semantics for moveable objects.
//
template <typename T>
class auto_mover
{
    BOOST_STATIC_ASSERT((is_moveable<T>::value));

    typedef aligned_storage<
          sizeof(T)
        , alignment_of<T>::value
        > aligned_storage_t;

    detail::storage_holder<aligned_storage_t> storage_;

public:
    typedef T element_type;

public: // structors
    explicit auto_mover(T& operand)
    {
        move(storage_.address(), operand);
    }

    auto_mover(auto_mover& operand)
    {
        move(storage_.address(), operand.get());
    }

    ~auto_mover()
    {
        get().~T();
    }

public: // modifiers
    auto_mover& operator=(auto_mover& operand)
    {
        reset(operand.get());
    }

    auto_mover& swap(auto_mover& operand)
    {
        // Move operand off to the side...
        auto_mover temp(operand);

        // ...in order to move *this into its place...
        operand = *this;

        // ...and finish by moving operand's _old_ value to *this:
        return (*this = temp);
    }

    void reset(T& operand)
    {
        get().~T();
        move(storage_.address(), operand);
    }

public: // queries
    T& operator*()
    {
        return get();
    }

    const T& operator*() const
    {
        return get();
    }

    T* operator->()
    {
        return reinterpret_cast<T*>(storage_.address());
    }

    const T* operator->() const
    {
        return reinterpret_cast<T*>(storage_.address());
    }

    T& get()
    {
        return *reinterpret_cast<T*>(storage_.address());
    }

    const T& get() const
    {
        return *reinterpret_cast<const T*>(storage_.address());
    }

public: // conversions
    auto_mover(detail::auto_mover_ref<T> operand)
    {
        move(storage_.address(), operand.ref_);
    }

    operator detail::auto_mover_ref<T>()
    {
        return detail::auto_mover_ref<T>(get());
    }
};

} // namespace boost

#endif // BOOST_AUTO_MOVER_HPP

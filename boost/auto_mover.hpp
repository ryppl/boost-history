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

    aligned_storage_t storage_;

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

private: // conversions (see below)
    // struct auto_mover_ref
    //
    // Similar in function to std::auto_ptr_ref.
    //
    struct auto_mover_ref
    {
        T& ref_;

    public:
        auto_mover_ref(T& ref)
            : ref_(ref)
        {
        }
    };

public: // conversions
    auto_mover(auto_mover_ref operand)
    {
        move(storage_.address(), operand.ref_);
    }

    operator auto_mover_ref()
    {
        return auto_mover_ref(get());
    }
};

} // namespace boost

#endif // BOOST_AUTO_MOVER_HPP

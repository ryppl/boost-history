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
    : public moveable< auto_mover<T> >
{
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
        new(storage_.address()) T( move(operand) );
    }

    auto_mover(move_source<auto_mover> source)
    {
        auto_mover& operand = source.get();
        new(storage_.address()) T( move(operand.get()) );
    }

    ~auto_mover()
    {
        get().~T();
    }

public: // modifiers
    auto_mover& operator=(move_source<auto_mover> source)
    {
        auto_mover& operand = source.get();
        reset(operand.get());

        return *this;
    }

    void swap(auto_mover& operand)
    {
        move_swap(
              this->get()
            , operand.get()
            );
    }

    void reset(T& operand)
    {
        get() = move(operand);
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
        return static_cast<T*>(storage_.address());
    }

    const T* operator->() const
    {
        return static_cast<T*>(storage_.address());
    }

    T& get()
    {
        return *static_cast<T*>(storage_.address());
    }

    const T& get() const
    {
        return *static_cast<const T*>(storage_.address());
    }
};

template <typename T>
inline void swap(auto_mover<T>& lhs, auto_mover<T>& rhs)
{
    lhs.swap(rhs);
}

} // namespace boost

#endif // BOOST_AUTO_MOVER_HPP

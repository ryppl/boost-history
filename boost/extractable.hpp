//-----------------------------------------------------------------------------
// boost extractable.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_EXTRACTABLE_HPP
#define BOOST_EXTRACTABLE_HPP

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template extractable
//
// Derived by types supporting extraction of type T, i.e., type Deriving
// such that T* Deriving::extract<T>() is defined.
//

namespace detail { namespace extractable {

// (detail) struct directly_extractable_tag
//
// Indicates deriving type is directly extractable.
//
struct directly_extractable_tag { };

// (detail) struct private_forward_tag
//
// Disambiguates between forwarding function and target.
//
struct private_forward_tag { };

}} // namespace detail::extractable

template <typename Deriving>
class extractable
    : public detail::extractable::directly_extractable_tag
{
public: // forwarding functions

    template <typename T>
    T* extract(detail::extractable::private_forward_tag)
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::extract
        //   is not implemented. Check to make sure you desire this.

        return static_cast<Deriving*>(this)->template extract<T>();
    }

    template <typename T>
    T* extract(detail::extractable::private_forward_tag) const
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::extract const
        //   is not implemented. Check to make sure you desire this.

        return static_cast<const Deriving*>(this)->template extract<T>();
    }

protected: // noninstantiable

    ~extractable() { }

};

//////////////////////////////////////////////////////////////////////////
// class template extractable_traits
//
// Specialized for types wishing to offer (indirect) value extraction.
//

namespace detail { namespace extractable {

struct default_traits_tag { };

}} // namespace detail::extractable


template <typename Extractable>
struct extractable_traits
    : detail::extractable::default_traits_tag
{
public: // static functions

    // [NOTE: Both "default" implementations assume direct extractability.]

    template <typename T>
    static T* extract(Extractable& operand)
    {
        // Extract directly:
        extractable<Extractable>& x = operand;
        return x.template extract<T>(detail::extractable::private_forward_tag());
    }

    template <typename T>
    static T* extract(const Extractable& operand)
    {
        // Const-extract directly:
        const extractable<Extractable>& x = operand;
        return x.template extract<T>(detail::extractable::private_forward_tag());
    }

};

} // namespace boost

#endif // BOOST_EXTRACTABLE_HPP

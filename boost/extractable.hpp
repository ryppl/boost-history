//-----------------------------------------------------------------------------
// boost extractable.hpp header file
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

#ifndef BOOST_EXTRACTABLE_HPP
#define BOOST_EXTRACTABLE_HPP

#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/logical/or.hpp"

#include "boost/mpl/aux_/lambda_support.hpp" // used by is_extractable

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template extractable
//
// Derived by types supporting extraction of type T, i.e., type Deriving
// such that T* Deriving::extract<T>() is defined.
//

namespace detail { namespace extractable {

struct directly_extractable_tag { }; // indicates type is directly extractable
struct private_forward_tag { }; // used to disambiguate between forwarding func and target

}} // namespace detail::extractable


template <typename Deriving>
class extractable
    : public detail::extractable::directly_extractable_tag
{
public:
    template <typename T>
    T* extract(detail::extractable::private_forward_tag)
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::extract
        //   is not implemented. Check to make sure you desire this.

        return static_cast<Deriving*>(this)->extract<T>();
    }

    template <typename T>
    T* extract(detail::extractable::private_forward_tag) const
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::extract const
        //   is not implemented. Check to make sure you desire this.

        return static_cast<const Deriving*>(this)->extract<T>();
    }

protected:
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

    // [NOTE: Both "default" implementations assume direct extractability.]

    template <typename T>
    static T* extract(Extractable& operand)
    {
        // Extract directly:
        extractable<Extractable>& x = operand;
        return x.extract<T>(detail::extractable::private_forward_tag());
    }

    template <typename T>
    static T* extract(const Extractable& operand)
    {
        // Const-extract directly:
        const extractable<Extractable>& x = operand;
        return x.extract<T>(detail::extractable::private_forward_tag());
    }
};

//////////////////////////////////////////////////////////////////////////
// metafunction is_extractable
//
// Value metafunction indicates whether the specified type supports
// extraction of a specified type T.
// 
// NOTE: This template never needs to be specialized!
//
template <typename T>
struct is_extractable
{
    typedef typename mpl::logical_or< // directly-extractable || NOT default-traits
          is_base_and_derived<
              detail::extractable::directly_extractable_tag
            , T
            >
        , mpl::logical_not<
              is_base_and_derived<
                  detail::extractable::default_traits_tag
                , extractable_traits<T>
                >
            >
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = type::value);

    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_extractable,(T))
};

} // namespace boost

#endif // BOOST_EXTRACTABLE_HPP

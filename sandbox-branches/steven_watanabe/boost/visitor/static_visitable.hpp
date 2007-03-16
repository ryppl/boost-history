//-----------------------------------------------------------------------------
// boost static_visitable.hpp header file
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

#ifndef BOOST_STATIC_VISITABLE_HPP
#define BOOST_STATIC_VISITABLE_HPP

#include "boost/config.hpp"
#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/mpl/not.hpp"
#include "boost/mpl/or.hpp"

#include "boost/mpl/aux_/lambda_support.hpp" // used by is_static_visitable

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template static_visitable
//
// Derived by types visitable by static visitors, i.e., a type Deriving
// such that Deriving::apply_visitor(visitor) is defined.
//

namespace detail { namespace static_visitable {

struct directly_visitable_tag { }; // indicates type is directly visitable
struct private_forward_tag { }; // used to disambiguate between forwarding func and target

}} // namespace detail::static_visitable


template <typename Deriving>
class static_visitable
    : public detail::static_visitable::directly_visitable_tag
{
public:
    template <typename Visitor>
        typename Visitor::result_type
    apply_visitor(Visitor& visitor, detail::static_visitable::private_forward_tag)
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::apply_visitor
        //   is not implemented. Check to make sure you desire this.

        return static_cast<Deriving*>(this)->apply_visitor(visitor);
    }

    template <typename Visitor>
        typename Visitor::result_type
    apply_visitor(Visitor& visitor, detail::static_visitable::private_forward_tag) const
    {
        // NOTE TO USER:
        //   If following line fails to compile, Deriving::apply_visitor const
        //   is not implemented. Check to make sure you desire this.

        return static_cast<const Deriving*>(this)->apply_visitor(visitor);
    }

protected:
    ~static_visitable() { }
};

//////////////////////////////////////////////////////////////////////////
// class template static_visitable_traits
//
// Class template meant to be specialized for types wishing to offer
//   (indirect) visitability by a static visitor.
//
// NOTE: Default implementations assume direct visitability.
//

namespace detail { namespace static_visitable {

struct default_traits_tag { };

}} // namespace detail::static_visitable


template <typename Visitable>
struct static_visitable_traits
    : detail::static_visitable::default_traits_tag
{
public: // static functions

    template <typename Visitor>
    static
        typename Visitor::result_type
    apply_visitor(Visitor& visitor, Visitable& operand)
    {
        // Visit directly:
        static_visitable<Visitable>& v = operand;
        return v.apply_visitor(
              visitor
            , detail::static_visitable::private_forward_tag()
            );
    }

    template <typename Visitor>
    static
        typename Visitor::result_type
    apply_visitor(Visitor& visitor, const Visitable& operand)
    {
        // Const-visit directly:
        const static_visitable<Visitable>& v = operand;
        return v.apply_visitor(
              visitor
            , detail::static_visitable::private_forward_tag()
            );
    }

};

//////////////////////////////////////////////////////////////////////////
// metafunction is_static_visitable
//
// Value metafunction indicates whether the specified type is visitable
// by a static visitor.
// 
// NOTE: This template never needs to be specialized!
//
template <typename T>
struct is_static_visitable
{
    typedef typename mpl::or_< // directly-visitable || NOT default-traits
          is_base_and_derived<
              detail::static_visitable::directly_visitable_tag
            , T
            >
        , mpl::not_<
              is_base_and_derived<
                  detail::static_visitable::default_traits_tag
                , static_visitable_traits<T>
                >
            >
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = type::value);

    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_static_visitable,(T))
};

} // namespace boost

#endif // BOOST_STATIC_VISITABLE_HPP

//-----------------------------------------------------------------------------
// boost visitor/detail/wrap_dynamic_visitor.hpp header file
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

#ifndef BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP
#define BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP

#include "boost/visitor/static_visitor.hpp"
#include "boost/visitor/bad_visit.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/try_dynamic_visit.hpp"
#include "boost/visitor/detail/dynamic_return_error.hpp"

#include "boost/type_traits/is_void.hpp"
#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/if.hpp"
#include "boost/utility.hpp" // for noncopyable

namespace boost {

namespace detail { namespace visitor {

class wrap_dynamic_visitor_base_t
    : boost::noncopyable
    , public static_visitor<>
{
private: // representation

    dynamic_visitor_base& visitor_;

public: // structors

    explicit wrap_dynamic_visitor_base_t(dynamic_visitor_base& visitor)
        : visitor_(visitor)
    {
    }

public: // visitor interfaces

    template <typename T>
    void operator()(T& operand)
    {
        if ( !try_dynamic_visit(visitor_, operand) )
            throw bad_visit();
    }

};

template <typename R>
class wrap_dynamic_visitor_return_base_t
    : boost::noncopyable
    , public static_visitor<R>
{
private: // representation

    dynamic_visitor_return_base<R>& visitor_;

public: // structors

    explicit wrap_dynamic_visitor_return_base_t(dynamic_visitor_return_base<R>& visitor)
        : visitor_(visitor)
    {
    }

public: // visitor interfaces

    template <typename T>
    R operator()(T& operand)
    {
        visitor_.result_value.clear();

        if ( !try_dynamic_visit(visitor_, operand) )
            throw bad_visit();

        if (target.result_value.empty())
            throw detail::dynamic_return_error();

        return target.result_value.get();
    }

};

template <typename DynamicVisitor>
struct wrap_dynamic_visitor
{
public: // metafunction result

    typedef typename mpl::apply_if<
          is_static_visitor<DynamicVisitor>   // in case visitor is both dynamic and static
        , mpl::identity<DynamicVisitor&>
        , mpl::if_<
              is_void< typename DynamicVisitor::result_type >
            , wrap_dynamic_visitor_base_t
            , wrap_dynamic_visitor_return_base_t< typename DynamicVisitor::result_type >
            >
        >::type type;

};

}} // namespace detail::visitor

} // namespace boost

#endif // BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP

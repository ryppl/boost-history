//-----------------------------------------------------------------------------
// boost visitor/detail/wrap_dynamic_visitor.hpp header file
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

#ifndef BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP
#define BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP

#include "boost/visitor/bad_visit.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/detail/dynamic_return_error.hpp"

#include "boost/type_traits/is_void.hpp"
#include "boost/mpl/if.hpp"

namespace boost {

namespace detail { namespace visitor {

class wrap_dynamic_visitor_base_t
{
private: // representation
    dynamic_visitor_base& visitor_;

public: // typedefs
    typedef void
        result_type;

public: // structors
    explicit wrap_dynamic_visitor_base_t(dynamic_visitor_base& visitor)
        : visitor_(visitor)
    {
    }

public: // operators
    template <typename T>
    void operator()(T& operand)
    {
        if ( !try_dynamic_visit(visitor_, operand) )
            throw bad_visit();
    }
};

template <typename R>
class wrap_dynamic_visitor_return_base_t
{
private: // representation
    dynamic_visitor_return_base<R>& visitor_;

public: // typedefs
    typedef R
        result_type;

public: // structors
    explicit wrap_dynamic_visitor_return_base_t(dynamic_visitor_return_base<R>& visitor)
        : visitor_(visitor)
    {
    }

public: // operators
    template <typename T>
    result_type operator()(T& operand)
    {
        visitor_.return_value.clear();

        if ( !try_dynamic_visit(visitor_, operand) )
            throw bad_visit();

        if (target.return_value.empty())
            throw detail::dynamic_return_error();

        return target.return_value.get();
    }
};

template <typename DynamicVisitor>
class wrap_dynamic_visitor_t
{
public: // typedefs
    typedef typename DynamicVisitor::result_type
        result_type;

private: // representation
    typedef typename mpl::if_<
          is_void< result_type >
        , wrap_dynamic_visitor_base_t
        , wrap_dynamic_visitor_return_base_t<result_type>
        >::type wrapper_t;

    wrapper_t& wrapped_;

public: // structors
    explicit wrap_dynamic_visitor_t(DynamicVisitor& visitor)
        : wrapped_(visitor)
    {
    }

public: // operators
    template <typename T>
    result_type operator()(T& operand)
    {
        return wrapped_(operand);
    }
};

}} // namespace detail::visitor

} // namespace boost

#endif // BOOST_VISITOR_DETAIL_WRAP_DYNAMIC_VISITOR_HPP

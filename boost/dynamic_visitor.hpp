//-----------------------------------------------------------------------------
// boost visitor/dynamic_visitor.hpp header file
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

#ifndef BOOST_DYNAMIC_VISITOR_HPP
#define BOOST_DYNAMIC_VISITOR_HPP

#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/detail/cyclic_visitor_tag.hpp"

#include "boost/type_traits/is_void.hpp"

#include "boost/mpl/bool.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/protect.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template dynamic_visitor<Deriving, Types, ResultType = void>
//
// Publicly inherited by Deriving to to support dynamic visitation of the
// types specified by Types, with return value of type ResultType.
//

namespace detail {

template <typename Deriving, typename ResultType>
struct make_dynamic_visitor_impl
{
    template <typename Base, typename T>
    struct apply
    {
        struct type
            : Base
            , dynamic_visitor_interface<T>
        {
        private:
            ResultType visit_invoke(T& operand)
            {
                return (*static_cast< Deriving * >(this))(operand);
            }

            void visit_impl(
                  T& operand
                , mpl::true_// void_return
                )
            {
                visit_invoke(operand);
            }

            void visit_impl(
                  T& operand
                , mpl::false_// void_return
                )
            {
                result_value.reset(visit_invoke(operand));
            }

            virtual void visit(T& operand)
            {
                visit_impl(
                      operand
                    , mpl::bool_< is_void<ResultType>::type::value >()
                    );
            }

        protected:
            ~type()
            {
            }
        };
    };
};

} // namespace detail

template <typename Deriving, typename Types, typename ResultType = void>
struct dynamic_visitor
    : detail::visitor::cyclic_visitor_tag
    , mpl::fold<
          Types
        , typename mpl::if_<
              is_void< ResultType >
            , dynamic_visitor_base
            , dynamic_visitor_return_base<ResultType>
            >::type
        , mpl::protect< detail::make_dynamic_visitor_impl<Deriving, ResultType> >
        >::type
{
    typedef ResultType result_type;

protected:
    ~dynamic_visitor()
    {
    }
};

} // namespace boost

#endif // BOOST_DYNAMIC_VISITOR_HPP

///////////////////////////////////////////////////////////////////////////////
/// \file proto.hpp
/// The proto expression template compiler and supporting utilities.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_HPP_EAN_04_01_2005
#define BOOST_PROTO_HPP_EAN_04_01_2005

#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/op_tags.hpp>
#include <boost/xpressive/proto/op_base.hpp>
#include <boost/xpressive/proto/operators.hpp>
#include <boost/xpressive/proto/arg_traits.hpp>

namespace boost { namespace proto
{
    ///////////////////////////////////////////////////////////////////////////////
    // compile_result
    template<typename Node, typename State, typename Visitor, typename DomainTag>
    struct compile_result
    {
        typedef typename as_op<Node>::type op_type;
        typedef compiler<typename tag_type<op_type>::type, DomainTag> compiler;
        typedef typename compiler::BOOST_NESTED_TEMPLATE apply<op_type, State, Visitor>::type type;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // compile
    template<typename Node, typename State, typename Visitor, typename DomainTag>
    typename compile_result<Node, State, Visitor, DomainTag>::type const
    compile(Node const &node, State const &state, Visitor &visitor, DomainTag)
    {
        typedef typename as_op<Node>::type op_type;
        typedef compiler<typename tag_type<op_type>::type, DomainTag> compiler;
        return compiler::call(as_op<Node>::make(node), state, visitor);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // literal, for storing and naming proto-ified constants.
    template<typename T>
    struct literal
      : unary_op<T, proto::noop_tag>
    {
        literal(T const &t)
          : unary_op<T, noop_tag>(t)
        {}

        template<typename U>
        literal(literal<U> const &that)
          : unary_op<T, noop_tag>(that.arg)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////////
    // lit(), for creating proto literals
    template<typename T>
    literal<T> lit(T const &t)
    {
        return literal<T>(t);
    }

}} // namespace boost::proto

#endif

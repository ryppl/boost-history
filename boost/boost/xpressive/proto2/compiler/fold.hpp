///////////////////////////////////////////////////////////////////////////////
/// \file fold.hpp
/// A special-purpose proto2 compiler for merging sequences of binary operations.
/// It compiles the right operand and passes the result as state while compiling
/// the left. Or, it might do the left first, if you choose.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO2_COMPILER_FOLD_HPP_EAN_04_01_2005
#define BOOST_PROTO2_COMPILER_FOLD_HPP_EAN_04_01_2005

#include <boost/xpressive/proto2/proto_fwd.hpp>

namespace boost { namespace proto2
{

    ///////////////////////////////////////////////////////////////////////////////
    // fold_compiler
    //  Compiles the left side and passes the result as state while compiling the right.
    //  This is useful for serializing a tree.
    template<typename DomainTag>
    struct fold_compiler
    {
        // sample compiler implementation for sequencing
        template<typename Node, typename State, typename Visitor>
        struct apply
        {
            typedef typename Node::arg0_type left_type;
            typedef typename Node::arg1_type right_type;

            // compile the right branch
            typedef typename compiler<typename left_type::tag_type, DomainTag>::
                BOOST_NESTED_TEMPLATE apply
            <
                left_type
              , State
              , Visitor
            >::type left_compiled_type;

            // forward the result of the right branch to the left
            typedef typename compiler<typename right_type::tag_type, DomainTag>::
                BOOST_NESTED_TEMPLATE apply
            <
                right_type
              , left_compiled_type
              , Visitor
            >::type type;
        };

        template<typename Node, typename State, typename Visitor>
        static typename apply<Node, State, Visitor>::type
        call(Node const &node, State const &state, Visitor &visitor)
        {
            return proto2::compile(
                proto2::right(node)
              , proto2::compile(proto2::left(node), state, visitor, DomainTag())
              , visitor
              , DomainTag()
            );
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // reverse_fold_compiler
    //  Compiles the right side and passes the result as state while compiling the left.
    //  This is useful for serializing a tree.
    template<typename DomainTag>
    struct reverse_fold_compiler
    {
        // sample compiler implementation for sequencing
        template<typename Node, typename State, typename Visitor>
        struct apply
        {
            typedef typename Node::arg0_type left_type;
            typedef typename Node::arg1_type right_type;

            // compile the right branch
            typedef typename compiler<typename right_type::tag_type, DomainTag>::
                BOOST_NESTED_TEMPLATE apply
            <
                right_type
              , State
              , Visitor
            >::type right_compiled_type;

            // forward the result of the right branch to the left
            typedef typename compiler<typename left_type::tag_type, DomainTag>::
                BOOST_NESTED_TEMPLATE apply
            <
                left_type
              , right_compiled_type
              , Visitor
            >::type type;
        };

        template<typename Node, typename State, typename Visitor>
        static typename apply<Node, State, Visitor>::type
        call(Node const &node, State const &state, Visitor &visitor)
        {
            return proto2::compile(
                proto2::left(node)
              , proto2::compile(proto2::right(node), state, visitor, DomainTag())
              , visitor
              , DomainTag()
            );
        }
    };

}}

#endif

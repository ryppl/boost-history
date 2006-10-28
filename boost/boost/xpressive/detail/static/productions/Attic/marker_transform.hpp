///////////////////////////////////////////////////////////////////////////////
// marker_transform.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_STATIC_PRODUCTIONS_MARKER_TRANSFORM_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_STATIC_PRODUCTIONS_MARKER_TRANSFORM_HPP_EAN_10_04_2005

#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/proto2/proto.hpp>
#include <boost/xpressive/proto2/compiler/transform.hpp>

namespace boost { namespace xpressive { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////////
    // is_marker
    // (s1= ...) is a marker
    template<typename Node, long Arity = Node::arity::value>
    struct is_marker
      : mpl::false_
    {};

    template<typename Node>
    struct is_marker<Node, 2>
      : mpl::and_<
            is_same<proto2::assign_tag, typename Node::tag_type>
          , is_same<mark_tag, typename proto2::unref<typename Node::arg0_type>::type>
        >
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // is_marker_predicate
    struct is_marker_predicate
    {
        template<typename Node, typename, typename>
        struct apply
          : is_marker<Node>
        {};
    };

    ///////////////////////////////////////////////////////////////////////////////
    // marker_insert_transform
    //   Insert mark tags before and after the expression
    struct marker_insert_transform
    {
        template<typename Node, typename, typename>
        struct apply
        {
            typedef typename proto2::binary_op
            <
                proto2::right_shift_tag
              , proto2::literal<mark_begin_matcher>::type
              , typename proto2::binary_op
                <
                    proto2::right_shift_tag
                  , Node
                  , proto2::literal<mark_end_matcher>::type
                >::type
            >::type type;
        };

        template<typename Node, typename State, typename Visitor>
        static typename apply<Node, State, Visitor>::type
        call(Node const &node, State const &, Visitor &visitor, int mark_nbr = 0)
        {
            // if we're inserting a mark, and we're not being told the mark number,
            // we're inserting a hidden mark ... so grab the next hidden mark number.
            if(0 == mark_nbr)
            {
                mark_nbr = visitor.get_hidden_mark();
            }

            typename apply<Node, State, Visitor>::type that =
                {
                    {mark_begin_matcher(mark_nbr)}
                  , {
                        node
                      , {mark_end_matcher(mark_nbr)}
                    }
                };
            return that;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // marker_replace_transform
    struct marker_replace_transform
      : proto2::compose_transforms<proto2::right_transform, marker_insert_transform>
    {
        template<typename Node, typename State, typename Visitor>
        static typename apply<Node, State, Visitor>::type
        call(Node const &node, State const &state, Visitor &visitor)
        {
            return marker_insert_transform::call
            (
                proto2::right(node)
              , state
              , visitor
              , proto2::arg(proto2::left(node)).mark_number_
            );
        }
    };

}}}

#endif

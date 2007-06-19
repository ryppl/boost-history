///////////////////////////////////////////////////////////////////////////////
/// \file fold_tree.hpp
/// A higher-level transform that uses the fold, and branch transforms
/// to recursively fold a tree.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_FOLD_TREE_HPP_EAN_06_18_2007
#define BOOST_PROTO_TRANSFORM_FOLD_TREE_HPP_EAN_06_18_2007

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/xpressive/proto/transform/fold.hpp>
#include <boost/xpressive/proto/transform/branch.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto { namespace transform
{

    namespace detail
    {
        template<typename Tag, typename Grammar>
        struct fold_tree_
          : or_<
                transform::fold<
                    nary_expr<Tag, vararg<fold_tree_<Tag, Grammar> > >
                >
              , Grammar
            >
        {};

        template<typename Tag, typename Grammar>
        struct reverse_fold_tree_
          : or_<
                transform::reverse_fold<
                    nary_expr<Tag, vararg<reverse_fold_tree_<Tag, Grammar> > >
                >
              , Grammar
            >
        {};
    }

    /// fold_tree
    ///
    template<typename Tag, typename Grammar, typename State = void>
    struct fold_tree
      : transform::branch<
            transform::fold<
                nary_expr<Tag, vararg<detail::fold_tree_<Tag, Grammar> > >
            >
          , State
        >
    {};

    template<typename Tag, typename Grammar>
    struct fold_tree<Tag, Grammar, void>
      : transform::fold<
            nary_expr<Tag, vararg<detail::fold_tree_<Tag, Grammar> > >
        >
    {};

    /// reverse_fold_tree
    ///
    template<typename Tag, typename Grammar, typename State = void>
    struct reverse_fold_tree
      : transform::branch<
            transform::reverse_fold<
                nary_expr<Tag, vararg<detail::reverse_fold_tree_<Tag, Grammar> > >
            >
          , State
        >
    {};

    template<typename Tag, typename Grammar>
    struct reverse_fold_tree<Tag, Grammar, void>
      : transform::reverse_fold<
            nary_expr<Tag, vararg<detail::reverse_fold_tree_<Tag, Grammar> > >
        >
    {};

}}}

#endif

///////////////////////////////////////////////////////////////////////////////
// predicate_matcher.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_PREDICATE_MATCHER_HPP_EAN_03_22_2007
#define BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_PREDICATE_MATCHER_HPP_EAN_03_22_2007

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/core/quant_style.hpp>
#include <boost/xpressive/detail/core/matcher/action_matcher.hpp>
#include <boost/xpressive/detail/core/state.hpp>
#include <boost/xpressive/proto/proto.hpp>

namespace boost { namespace xpressive { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////////
    // predicate_context
    //
    template<typename BidiIter>
    struct predicate_context
    {
        explicit predicate_context(int sub, match_results<BidiIter> const &what)
          : sub_(sub)
          , what_(what)
        {}

        // eval_terminal
        template<typename Expr, typename Arg = typename proto::result_of::arg<Expr>::type>
        struct eval_terminal
          : proto::default_eval<Expr, predicate_context const>
        {};

        template<typename Expr, typename Arg>
        struct eval_terminal<Expr, reference_wrapper<Arg> >
        {
            typedef Arg &result_type;
            result_type operator()(Expr &expr, predicate_context const &) const
            {
                return proto::arg(expr).get();
            }
        };

        template<typename Expr>
        struct eval_terminal<Expr, any_matcher>
        {
            typedef sub_match<BidiIter> const &result_type;
            result_type operator()(Expr &expr, predicate_context const &ctx) const
            {
                return ctx.what_[ctx.sub_];
            }
        };

        template<typename Expr>
        struct eval_terminal<Expr, mark_placeholder>
        {
            typedef sub_match<BidiIter> const &result_type;
            result_type operator()(Expr &expr, predicate_context const &ctx) const
            {
                return ctx.what_[expr];
            }
        };

        // eval
        template<typename Expr, typename Tag = typename Expr::proto_tag>
        struct eval
          : proto::default_eval<Expr, predicate_context const>
        {};

        template<typename Expr>
        struct eval<Expr, proto::tag::terminal>
          : eval_terminal<Expr>
        {};

        #if BOOST_VERSION >= 103500
        template<typename Expr>
        struct eval<Expr, proto::tag::mem_ptr>
          : mem_ptr_eval<Expr, predicate_context const>
        {};
        #endif

        int sub_;
        match_results<BidiIter> const &what_;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // predicate_matcher
    //
    template<typename Predicate>
    struct predicate_matcher
      : quant_style_assertion
    {
        int sub_;
        Predicate predicate_;

        predicate_matcher(Predicate const &pred, int sub)
          : sub_(sub)
          , predicate_(pred)
        {
        }

        template<typename BidiIter, typename Next>
        bool match(match_state<BidiIter> &state, Next const &next) const
        {
            typedef typename Predicate::proto_arg0::predicate_type predicate_type;
            return this->match_(state, next, proto::is_expr<predicate_type>());
        }

    private:
        template<typename BidiIter, typename Next>
        bool match_(match_state<BidiIter> &state, Next const &next, mpl::false_) const
        {
            sub_match<BidiIter> const &sub = state.sub_match(this->sub_);
            return proto::arg(this->predicate_).pred(sub) && next.match(state);
        }

        template<typename BidiIter, typename Next>
        bool match_(match_state<BidiIter> &state, Next const &next, mpl::true_) const
        {
            predicate_context<BidiIter> ctx(this->sub_, *state.context_.results_ptr_);
            return proto::eval(proto::arg(this->predicate_).pred, ctx) && next.match(state);
        }
    };

}}}

#endif // BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_PREDICATE_MATCHER_HPP_EAN_03_22_2007

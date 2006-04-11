///////////////////////////////////////////////////////////////////////////////
// optional_matcher.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_OPTIONAL_MATCHER_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_OPTIONAL_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <vector>

#include <boost/version.hpp>
#if BOOST_VERSION <= 103200
// WORKAROUND for Fusion bug in Boost 1.32
namespace boost { namespace fusion
{
    namespace detail { struct iterator_root; }
    using detail::iterator_root;
}}
#endif

#include <boost/shared_ptr.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/core/quant_style.hpp>
#include <boost/xpressive/detail/core/state.hpp>
#include <boost/xpressive/detail/static/width_of.hpp>
#include <boost/xpressive/detail/static/is_pure.hpp>
#include <boost/xpressive/detail/dynamic/matchable.hpp>
#include <boost/xpressive/detail/utility/hash_peek_bitset.hpp>
#include <boost/xpressive/detail/utility/algorithm.hpp>
#include <boost/xpressive/detail/utility/any.hpp>

namespace boost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // optional_matcher
    template<typename Xpr, bool Greedy>
    struct optional_matcher
      : quant_style<quant_variable_width, unknown_width::value, Xpr::pure>
    {
        Xpr xpr_;

        explicit optional_matcher(Xpr const &xpr)
          : xpr_(xpr)
        {
        }

        template<typename BidiIter, typename Next>
        bool match(state_type<BidiIter> &state, Next const &next) const
        {
            return this->match_(state, next, mpl::bool_<Greedy>());
        }

    private:
        template<typename BidiIter, typename Next>
        bool match_(state_type<BidiIter> &state, Next const &next, mpl::true_) const // Greedy
        {
            return this->xpr_.BOOST_NESTED_TEMPLATE push_match<Next>(state)
                || next.match(state);
        }

        template<typename BidiIter, typename Next>
        bool match_(state_type<BidiIter> &state, Next const &next, mpl::false_) const // Non-greedy
        {
            return next.match(state)
                || this->xpr_.BOOST_NESTED_TEMPLATE push_match<Next>(state);
        }

        optional_matcher &operator =(optional_matcher const &);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // optional_mark_matcher
    template<typename BidiIter, typename Next>
    inline bool match_next(state_type<BidiIter> &state, Next const &next, int mark_number)
    {
        sub_match_impl<BidiIter> &br = state.sub_match(mark_number);

        bool old_matched = br.matched;
        br.matched = false;

        if(next.match(state))
        {
            return true;
        }

        br.matched = old_matched;
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // optional_mark_matcher
    template<typename Xpr, bool Greedy>
    struct optional_mark_matcher
      : quant_style<quant_variable_width, unknown_width::value, Xpr::pure>
    {
        Xpr xpr_;
        int mark_number_;

        explicit optional_mark_matcher(Xpr const &xpr, int mark_number)
          : xpr_(xpr)
          , mark_number_(mark_number)
        {
        }

        template<typename BidiIter, typename Next>
        bool match(state_type<BidiIter> &state, Next const &next) const
        {
            return this->match_(state, next, mpl::bool_<Greedy>());
        }

    private:
        template<typename BidiIter, typename Next>
        bool match_(state_type<BidiIter> &state, Next const &next, mpl::true_) const // Greedy
        {
            return this->xpr_.BOOST_NESTED_TEMPLATE push_match<Next>(state)
                || match_next(state, next, this->mark_number_);
        }

        template<typename BidiIter, typename Next>
        bool match_(state_type<BidiIter> &state, Next const &next, mpl::false_) const // Non-greedy
        {
            return match_next(state, next, this->mark_number_)
                || this->xpr_.BOOST_NESTED_TEMPLATE push_match<Next>(state);
        }

        optional_mark_matcher &operator =(optional_mark_matcher const &);
    };

}}}

#endif

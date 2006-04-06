///////////////////////////////////////////////////////////////////////////////
// matchable.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_DYNAMIC_MATCHABLE_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_DYNAMIC_MATCHABLE_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <utility>
#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/xpressive/detail/core/state.hpp>
#include <boost/xpressive/detail/core/quant_style.hpp>
#include <boost/xpressive/regex_error.hpp>

namespace boost { namespace xpressive { namespace detail
{

template<typename BidiIter>
struct dynamic_xpression_base;

///////////////////////////////////////////////////////////////////////////////
// width_
//
struct width_
{
    explicit width_(std::size_t v = 0)
      : value(v)
    {
    }

    width_ &operator +=(width_ const &that)
    {
        this->value =
            this->value != unknown_width() && that.value != unknown_width()
          ? this->value + that.value
          : unknown_width();
        return *this;
    }

    width_ &operator |=(width_ const &that)
    {
        this->value = 
            this->value == that.value
          ? this->value
          : unknown_width();
        return *this;
    }

    operator std::size_t () const
    {
        return this->value;
    }

private:
    std::size_t value;
};

///////////////////////////////////////////////////////////////////////////////
// sequence
//
template<typename BidiIter>
struct sequence
  : std::pair
    <
        shared_ptr<dynamic_xpression_base<BidiIter> const>
      , shared_ptr<dynamic_xpression_base<BidiIter> const> *
    >
{
    typedef shared_ptr<dynamic_xpression_base<BidiIter> const> matchable_ptr_t;
    typedef std::pair<matchable_ptr_t, matchable_ptr_t *> base_t;

    explicit sequence
    (
        matchable_ptr_t head = matchable_ptr_t()
      , matchable_ptr_t *tail_ptr = 0
      , std::size_t width = 0
      , bool pure = true
    )
      : base_t(head, tail_ptr)
      , width_of(width)
      , is_pure(pure)
    {
    }

    bool is_empty() const
    {
        return !this->first;
    }

    sequence &operator +=(sequence that)
    {
        if(is_empty())
        {
            *this = that;
        }
        else if(!that.is_empty())
        {
            *this->second = that.first;
            this->second = that.second;
            // keep track of sequence width and purity
            this->width_of += that.width_of;
            this->is_pure = this->is_pure && that.is_pure;
        }
        return *this;
    }

    width_ width_of;
    bool is_pure;
};

//////////////////////////////////////////////////////////////////////////
// quant_spec
//
struct quant_spec
{
    unsigned int min_;
    unsigned int max_;
    bool greedy_;
};

///////////////////////////////////////////////////////////////////////////////
// matchable
//
template<typename BidiIter>
struct matchable
  : xpression_base
{
    typedef BidiIter iterator_type;
    typedef typename iterator_value<iterator_type>::type char_type;
    virtual ~matchable() {}
    virtual bool match(state_type<BidiIter> &state) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
// dynamic_xpression_base
//
template<typename BidiIter>
struct dynamic_xpression_base
  : matchable<BidiIter>
{
    typedef BidiIter iterator_type;
    typedef typename iterator_value<iterator_type>::type char_type;

    virtual void link(xpression_linker<char_type> &) const
    {
    }

    virtual void peek(xpression_peeker<char_type> &peeker) const
    {
        peeker.fail();
    }

    virtual sequence<BidiIter> quantify
    (
        quant_spec const & //spec
      , std::size_t & //hidden_mark_count
      , sequence<BidiIter> //seq
      , alternates_factory<BidiIter> const & //factory
    ) const
    {
        throw regex_error(regex_constants::error_badrepeat, "expression cannot be quantified");
    }

    virtual bool is_quantifiable() const
    {
        BOOST_ASSERT(false);
        throw regex_error(regex_constants::error_internal, "internal error, sorry!");
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // The following 4 functions (push_match, top_match, pop_match and skip_match) are
    // used to implement looping and branching across the matchers. Call push_match to record
    // a position. Then, another matcher further down the xpression chain has the
    // option to call either top_match, pop_match or skip_match. top_match and pop_match will
    // jump back to the place recorded by push_match, whereas skip_match will skip the jump and
    // pass execution down the xpression chain. top_match will leave the xpression on top of the
    // stack, whereas pop_match will remove it. Each function comes in 2 flavors: one for
    // statically bound xpressions and one for dynamically bound xpressions.
    //

    template<typename Top>
    bool push_match(state_type<BidiIter> &state) const
    {
        BOOST_MPL_ASSERT((is_same<Top, dynamic_xpression_base<BidiIter> >));
        return this->match(state);
    }

    static bool top_match(state_type<BidiIter> &state, xpression_base const *top)
    {
        return static_cast<dynamic_xpression_base<BidiIter> const *>(top)->match(state);
    }

    static bool pop_match(state_type<BidiIter> &state, xpression_base const *top)
    {
        return static_cast<dynamic_xpression_base<BidiIter> const *>(top)->match(state);
    }

    bool skip_match(state_type<BidiIter> &state) const
    {
        return this->match(state);
    }
};

}}} // namespace boost::xpressive::detail

#endif

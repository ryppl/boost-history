///////////////////////////////////////////////////////////////////////////////
//
// algorithm.hpp
//
/////////////////////////////////////////////////////////////////////////////

// Copyright 2004 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// (C) Copyright Thorsten Ottosen 2002-3. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Mutating algorithms originally written by Vladimir Prus'
// <ghost@cs.msu.su> code from Boost Wiki

// Problem: should member functions be called automatically? Or should the user
// know that it is better to call map::find() than find( map )?

#if _MSC_VER> 1000
#pragma once
#endif // _MSC_VER> 1000

#ifndef ALGORITHM_EN_14_12_2004
#define ALGORITHM_EN_14_12_2004

#include <algorithm>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include "./detail/adl_begin_end.hpp"
#include "./detail/has_find.hpp"
#include "./detail/has_remove.hpp"
#include "./detail/has_remove_if.hpp"
#include "./detail/has_unique.hpp"
#include "./detail/has_reverse.hpp"
#include "./detail/has_sort.hpp"
#include "./detail/has_lower_bound.hpp"
#include "./detail/has_upper_bound.hpp"
#include "./detail/has_equal_range.hpp"

namespace boost
{
    namespace detail
    {
        template<typename Rng>
        struct iter_pair
        {
            typedef BOOST_DEDUCED_TYPENAME ::boost::range_result_iterator<
                Rng
            >::type iterator;

            typedef std::pair<iterator,iterator> type;
        };
    }

    /////////////////////////////////////////////////////////////////////////
    // Non-Modifying Sequence Operations
    /////////////////////////////////////////////////////////////////////////

    // template function for_each
    template<typename Rng,typename Fun>
    inline Fun for_each(Rng & rng,Fun fun)
    {
        return std::for_each(detail::adl_begin(rng),detail::adl_end(rng),fun);
    }

    template<typename Rng,typename Fun>
    inline Fun for_each(Rng const & rng,Fun fun)
    {
        return std::for_each(detail::adl_begin(rng),detail::adl_end(rng),fun);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_find<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        find_impl(Rng & rng,Val const & val)
        {
            return rng.find(val);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_find<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        find_impl(Rng & rng,Val const & val)
        {
            return std::find(detail::adl_begin(rng),detail::adl_end(rng),val);
        }
    }

    // template function find
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    find(Rng & rng,Val const & val)
    {
        return detail::find_impl(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    find(Rng const & rng,Val const & val)
    {
        return detail::find_impl(rng,val);
    }

    // template function find_if
    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    find_if(Rng & rng,Pred pred)
    {
        return std::find_if(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    find_if(Rng const & rng,Pred pred)
    {
        return std::find_if(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function find_end
    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    find_end(Rng1 & rng1,Rng2 const & rng2)
    {
        return std::find_end(detail::adl_begin(rng1),detail::adl_end(rng1),
                             detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    find_end(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::find_end(detail::adl_begin(rng1),detail::adl_end(rng1),
                             detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    find_end(Rng1 & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::find_end(detail::adl_begin(rng1),detail::adl_end(rng1),
                             detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    find_end(Rng1 const & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::find_end(detail::adl_begin(rng1),detail::adl_end(rng1),
                             detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    // template function find_first_of
    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    find_first_of(Rng1 & rng1,Rng2 const & rng2)
    {
        return std::find_first_of(detail::adl_begin(rng1),detail::adl_end(rng1),
                                  detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    find_first_of(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::find_first_of(detail::adl_begin(rng1),detail::adl_end(rng1),
                                  detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    find_first_of(Rng1 & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::find_first_of(detail::adl_begin(rng1),detail::adl_end(rng1),
                                  detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    find_first_of(Rng1 const & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::find_first_of(detail::adl_begin(rng1),detail::adl_end(rng1),
                                  detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    // template function adjacent_find
    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    adjacent_find(Rng & rng)
    {
        return std::adjacent_find(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    adjacent_find(Rng const & rng)
    {
        return std::adjacent_find(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    adjacent_find(Rng & rng,BinPred pred)
    {
        return std::adjacent_find(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    adjacent_find(Rng const & rng,BinPred pred)
    {
        return std::adjacent_find(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function count
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    count(Rng & rng,Val const & val)
    {
        return detail::count_impl_(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    count(Rng const & rng,Val const & val)
    {
        return detail::count_impl_(rng,val);
    }

    // template function count_if
    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    count_if(Rng & rng,Pred pred)
    {
        return std::count_if(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    count_if(Rng const & rng,Pred pred)
    {
        return std::count_if(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function mismatch
    template<typename Rng,typename InIter>
    inline std::pair<BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type,InIter>
    mismatch(Rng & rng,InIter first)
    {
        return std::mismatch(detail::adl_begin(rng),detail::adl_end(rng),first);
    }

    template<typename Rng,typename InIter>
    inline std::pair<BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type,InIter>
    mismatch(Rng const & rng,InIter first)
    {
        return std::mismatch(detail::adl_begin(rng),detail::adl_end(rng),first);
    }

    template<typename Rng,typename InIter,typename BinPred>
    inline std::pair<BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type,InIter>
    mismatch(Rng & rng,InIter first,BinPred pred)
    {
        return std::mismatch(detail::adl_begin(rng),detail::adl_end(rng),first,pred);
    }

    template<typename Rng,typename InIter,typename BinPred>
    inline std::pair<BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type,InIter>
    mismatch(Rng const & rng,InIter first,BinPred pred)
    {
        return std::mismatch(detail::adl_begin(rng),detail::adl_end(rng),first,pred);
    }

    // template function equal
    template<typename Rng,typename InIter>
    inline bool equal(Rng & rng,InIter first)
    {
        return std::equal(detail::adl_begin(rng),detail::adl_end(rng),first);
    }

    template<typename Rng,typename InIter>
    inline bool equal(Rng const & rng,InIter first)
    {
        return std::equal(detail::adl_begin(rng),detail::adl_end(rng),first);
    }

    template<typename Rng,typename InIter,typename BinPred>
    inline bool equal(Rng & rng,InIter first,BinPred pred)
    {
        return std::equal(detail::adl_begin(rng),detail::adl_end(rng),first,pred);
    }

    template<typename Rng,typename InIter,typename BinPred>
    inline bool equal(Rng const & rng,InIter first,BinPred pred)
    {
        return std::equal(detail::adl_begin(rng),detail::adl_end(rng),first,pred);
    }

    // template function search
    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    search(Rng1 & rng1,Rng2 const & rng2)
    {
        return std::search(detail::adl_begin(rng1),detail::adl_end(rng1),
                           detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    search(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::search(detail::adl_begin(rng1),detail::adl_end(rng1),
                           detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng1>::type
    search(Rng1 & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::search(detail::adl_begin(rng1),detail::adl_end(rng1),
                           detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng1>::type
    search(Rng1 const & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::search(detail::adl_begin(rng1),detail::adl_end(rng1),
                           detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    /////////////////////////////////////////////////////////////////////////
    // Modifying Sequence Operations
    /////////////////////////////////////////////////////////////////////////

    // template function copy
    template<typename Rng,typename OutIter>
    inline OutIter copy(Rng const & rng,OutIter out)
    {
        return std::copy(detail::adl_begin(rng),detail::adl_end(rng),out);
    }

    // template function copy_backwards
    template<typename Rng,typename BidiIter>
    inline BidiIter copy_backward(Rng const & rng,BidiIter out)
    {
        return std::copy_backward(detail::adl_begin(rng),detail::adl_end(rng),out);
    }

    // template function transform
    template<typename Rng,typename OutIter,typename UnaryOp>
    inline OutIter transform(Rng const & rng,OutIter out,UnaryOp fun)
    {
        return std::transform(detail::adl_begin(rng),detail::adl_end(rng),out,fun);
    }

    template<typename Rng,typename InIter,typename OutIter,typename BinOp>
    inline OutIter transform(Rng const & rng,InIter first2,OutIter out,BinOp fun)
    {
        return std::transform(detail::adl_begin(rng),detail::adl_end(rng),first2,out,fun);
    }

    // template function replace
    template<typename Rng,typename Val>
    inline void replace(Rng & rng,Val const & what,Val const & with_what)
    {
        return std::replace(detail::adl_begin(rng),detail::adl_end(rng),what,with_what);
    }

    template<typename Rng,typename Val>
    inline void replace(Rng const & rng,Val const & what,Val const & with_what)
    {
        return std::replace(detail::adl_begin(rng),detail::adl_end(rng),what,with_what);
    }

    // template function replace_if
    template<typename Rng,typename Pred,typename Val>
    inline void replace_if(Rng & rng,Pred pred,Val const & val)
    {
        return std::replace_if(detail::adl_begin(rng),detail::adl_end(rng),pred,val);
    }

    template<typename Rng,typename Pred,typename Val>
    inline void replace_if(Rng const & rng,Pred pred,Val const & val)
    {
        return std::replace_if(detail::adl_begin(rng),detail::adl_end(rng),pred,val);
    }

    // template function replace_copy
    template<typename Rng,typename OutIter,typename Val>
    inline OutIter replace_copy(Rng const & rng,OutIter out,Val const & what,Val const & with_what)
    {
        return std::replace_copy(detail::adl_begin(rng),detail::adl_end(rng),out,what,with_what);
    }

    // template function replace_copy_if
    template<typename Rng,typename OutIter,typename Pred,typename Val>
    inline OutIter replace_copy_if(Rng const & rng,OutIter out,Pred pred,Val const & val)
    {
        return std::replace_copy_if(detail::adl_begin(rng),detail::adl_end(rng),out,pred,val);
    }

    // template function fill
    template<typename Rng,typename Val>
    inline void fill(Rng & rng,Val const & val)
    {
        std::fill(detail::adl_begin(rng),detail::adl_end(rng),val);
    }

    template<typename Rng,typename Val>
    inline void fill(Rng const & rng,Val const & val)
    {
        std::fill(detail::adl_begin(rng),detail::adl_end(rng),val);
    }

    // template function fill_n
    template<typename Rng,typename Int,typename Val>
    inline void fill_n(Rng & rng,Int size,Val const & val)
    {
        std::fill_n(detail::adl_begin(rng),size,val);
    }

    template<typename Rng,typename Int,typename Val>
    inline void fill_n(Rng const & rng,Int size,Val const & val)
    {
        std::fill_n(detail::adl_begin(rng),size,val);
    }

    // template function generate
    template<typename Rng,typename Generator>
    inline void generate(Rng & rng,Generator gen)
    {
        std::generate(detail::adl_begin(rng),detail::adl_end(rng),gen);
    }

    template<typename Rng,typename Generator>
    inline void generate(Rng const & rng,Generator gen)
    {
        std::generate(detail::adl_begin(rng),detail::adl_end(rng),gen);
    }

    // template function generate_n
    template<typename Rng,typename Int,typename Generator>
    void generate_n(Rng & rng,Int size,Generator gen)
    {
        std::generate_n(detail::adl_begin(rng),size,gen);
    }

    template<typename Rng,typename Int,typename Generator>
    void generate_n(Rng const & rng,Int size,Generator gen)
    {
        std::generate_n(detail::adl_begin(rng),size,gen);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_remove<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        remove_impl(Rng & rng,Val const & val)
        {
            rng.remove(val);
            return detail::adl_end(rng);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_remove<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        remove_impl(Rng & rng,Val const & val)
        {
            return std::remove(detail::adl_begin(rng),detail::adl_end(rng),val);
        }
    }

    // template function remove
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    remove(Rng & rng,Val const & val)
    {
        return detail::remove_impl(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    remove(Rng const & rng,Val const & val)
    {
        return detail::remove_impl(rng,val);
    }

    namespace detail
    {
        template<typename Rng,typename Pred>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_remove_if<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        remove_if_impl(Rng & rng,Pred pred)
        {
            rng.remove_if(pred);
            return detail::adl_end(rng);
        }

        template<typename Rng,typename Pred>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_remove_if<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        remove_if_impl(Rng & rng,Pred pred)
        {
            return std::remove_if(detail::adl_begin(rng),detail::adl_end(rng),pred);
        }
    }

    // template function remove_if
    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    remove_if(Rng & rng,Pred pred)
    {
        return detail::remove_if_impl(rng,pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    remove_if(Rng const & rng,Pred pred)
    {
        return detail::remove_if_impl(rng,pred);
    }

    // template function remove_copy
    template<typename Rng,typename OutIter,typename Val>
    inline OutIter remove_copy(Rng const & rng,OutIter out,Val const & val)
    {
        return std::remove_copy(detail::adl_begin(rng),detail::adl_end(rng),out,val);
    }

    // template function remove_copy_if
    template<typename Rng,typename OutIter,typename Pred>
    inline OutIter remove_copy_if(Rng const & rng,OutIter out,Pred pred)
    {
        return std::remove_copy_if(detail::adl_begin(rng),detail::adl_end(rng),out,pred);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_unique<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        unique_impl(Rng & rng)
        {
            rng.unique();
            return detail::adl_end(rng);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_unique<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        unique_impl(Rng & rng)
        {
            return std::unique(detail::adl_begin(rng),detail::adl_end(rng));
        }
    }

    // template function unique
    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    unique(Rng & rng)
    {
        return detail::unique_impl(rng);
    }

    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    unique(Rng const & rng)
    {
        return detail::unique_impl(rng);
    }

    namespace detail
    {
        template<typename Rng,typename Pred>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_unique<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        unique_impl(Rng & rng,Pred pred)
        {
            rng.unique(pred);
            return detail::adl_end(rng);
        }

        template<typename Rng,typename Pred>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_unique<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        unique_impl(Rng & rng,Pred pred)
        {
            return std::unique(detail::adl_begin(rng),detail::adl_end(rng),pred);
        }
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    unique(Rng & rng,Pred pred)
    {
        return detail::unique_impl(rng,pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    unique(Rng const & rng,Pred pred)
    {
        return detail::unique_impl(rng,pred);
    }

    // template function unique_copy
    template<typename Rng,typename OutIter>
    inline OutIter unique_copy(Rng const & rng,OutIter out)
    {
        return std::unique_copy(detail::adl_begin(rng),detail::adl_end(rng),out);
    }

    template<typename Rng,typename OutIter,typename Pred>
    inline OutIter unique_copy(Rng const & rng,OutIter out,Pred pred)
    {
        return std::unique_copy(detail::adl_begin(rng),detail::adl_end(rng),out,pred);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::enable_if<
            has_reverse<Rng>
        >::type
        reverse_impl(Rng & rng)
        {
            rng.reverse();
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::disable_if<
            has_reverse<Rng>
        >::type
        reverse_impl(Rng & rng)
        {
            std::reverse(detail::adl_begin(rng),detail::adl_end(rng));
        }
    }

    // template function reverse
    template<typename Rng>
    inline void reverse(Rng & rng)
    {
        detail::reverse_impl(rng);
    }

    template<typename Rng>
    inline void reverse(Rng const & rng)
    {
        detail::reverse_impl(rng);
    }

    // template function reverse_copy
    template<typename Rng,typename OutIter>
    inline OutIter reverse_copy(Rng const & rng,OutIter out)
    {
        return std::reverse_copy(detail::adl_begin(rng),detail::adl_end(rng),out);
    }

    // template function rotate
    template<typename Rng>
    inline void rotate(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle)
    {
        std::rotate(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    template<typename Rng>
    inline void rotate(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle)
    {
        std::rotate(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    // template function rotate_copy
    template<typename Rng,typename OutIter>
    inline OutIter rotate_copy(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle,
        OutIter out)
    {
        return std::rotate_copy(detail::adl_begin(rng),middle,detail::adl_end(rng),out);
    }

    template<typename Rng,typename OutIter>
    inline OutIter rotate_copy(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle,
        OutIter out)
    {
        return std::rotate_copy(detail::adl_begin(rng),middle,detail::adl_end(rng),out);
    }

    // template function random_shuffle
    template<typename Rng>
    inline void random_shuffle(Rng & rng)
    {
        std::random_shuffle(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void random_shuffle(Rng const & rng)
    {
        std::random_shuffle(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Generator>
    inline void random_shuffle(Rng & rng,Generator gen)
    {
        std::random_shuffle(detail::adl_begin(rng),detail::adl_end(rng),gen);
    }

    template<typename Rng,typename Generator>
    inline void random_shuffle(Rng const & rng,Generator gen)
    {
        std::random_shuffle(detail::adl_begin(rng),detail::adl_end(rng),gen);
    }

    // template function partition
    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    partition(Rng & rng,Pred pred)
    {
        return std::partition(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    partition(Rng const & rng,Pred pred)
    {
        return std::partition(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function stable_partition
    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    stable_partition(Rng & rng,Pred pred)
    {
        return std::stable_partition(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename Pred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    stable_partition(Rng const & rng,Pred pred)
    {
        return std::stable_partition(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::enable_if<
            has_sort<Rng>
        >::type
        sort_impl(Rng & rng)
        {
            rng.sort();
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::disable_if<
            has_sort<Rng>
        >::type
        sort_impl(Rng & rng)
        {
            std::sort(detail::adl_begin(rng),detail::adl_end(rng));
        }
    }

    // template function sort
    template<typename Rng>
    inline void sort(Rng & rng)
    {
        detail::sort_impl(rng);
    }

    template<typename Rng>
    inline void sort(Rng const & rng)
    {
        detail::sort_impl(rng);
    }

    namespace detail
    {
        template<typename Rng,typename Cmp>
        inline BOOST_DEDUCED_TYPENAME ::boost::enable_if<
            has_sort<Rng>
        >::type
        sort_impl(Rng & rng,Cmp cmp)
        {
            rng.sort(cmp);
        }

        template<typename Rng,typename Cmp>
        inline BOOST_DEDUCED_TYPENAME ::boost::disable_if<
            has_sort<Rng>
        >::type
        sort_impl(Rng & rng,Cmp cmp)
        {
            std::sort(detail::adl_begin(rng),detail::adl_end(rng),cmp);
        }
    }

    template<typename Rng,typename Cmp>
    inline void sort(Rng & rng,Cmp cmp)
    {
        detail::sort_impl(rng,cmp);
    }

    template<typename Rng,typename Cmp>
    inline void sort(Rng const & rng,Cmp cmp)
    {
        detail::sort_impl(rng,cmp);
    }

    // template function stable_sort
    template<typename Rng>
    inline void stable_sort(Rng & rng)
    {
        std::stable_sort(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void stable_sort(Rng const & rng)
    {
        std::stable_sort(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void stable_sort(Rng & rng,Cmp cmp)
    {
        std::stable_sort(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void stable_sort(Rng const & rng,Cmp cmp)
    {
        std::stable_sort(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    // template function partial_sort
    template<typename Rng>
    inline void partial_sort(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle)
    {
        std::partial_sort(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    template<typename Rng>
    inline void partial_sort(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle)
    {
        std::partial_sort(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void partial_sort(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle,
        Cmp cmp)
    {
        std::partial_sort(detail::adl_begin(rng),middle,detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void partial_sort(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle,
        Cmp cmp)
    {
        std::partial_sort(detail::adl_begin(rng),middle,detail::adl_end(rng),cmp);
    }

    // template function partial_sort_copy
    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng2>::type
    partial_sort_copy(Rng1 const & rng1,Rng2 & rng2)
    {
        return std::partial_sort_copy(detail::adl_begin(rng1),detail::adl_end(rng1),
                                      detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng2>::type
    partial_sort_copy(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::partial_sort_copy(detail::adl_begin(rng1),detail::adl_end(rng1),
                                      detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng2>::type
    partial_sort_copy(Rng1 const & rng1,Rng2 & rng2,Cmp cmp)
    {
        return std::partial_sort_copy(detail::adl_begin(rng1),detail::adl_end(rng1),
                                      detail::adl_begin(rng2),detail::adl_end(rng2),cmp);
    }

    template<typename Rng1,typename Rng2,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng2>::type
    partial_sort_copy(Rng1 const & rng1,Rng2 const & rng2,Cmp cmp)
    {
        return std::partial_sort_copy(detail::adl_begin(rng1),detail::adl_end(rng1),
                                      detail::adl_begin(rng2),detail::adl_end(rng2),cmp);
    }

    // template function nth_element
    template<typename Rng>
    inline void nth_element(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type nth)
    {
        std::nth_element(detail::adl_begin(rng),nth,detail::adl_end(rng));
    }

    template<typename Rng>
    inline void nth_element(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type nth)
    {
        std::nth_element(detail::adl_begin(rng),nth,detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void nth_element(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type nth,
        Cmp cmp)
    {
        std::nth_element(detail::adl_begin(rng),nth,detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void nth_element(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type nth,
        Cmp cmp)
    {
        std::nth_element(detail::adl_begin(rng),nth,detail::adl_end(rng),cmp);
    }


    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_lower_bound<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        lower_bound_impl(Rng & rng,Val const & val)
        {
            return rng.lower_bound(val);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_lower_bound<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        lower_bound_impl(Rng & rng,Val const & val)
        {
            return std::lower_bound(detail::adl_begin(rng),detail::adl_end(rng),val);
        }
    }

    // template function lower_bound
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    lower_bound(Rng & rng,Val const & val)
    {
        return detail::lower_bound_impl(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    lower_bound(Rng const & rng,Val const & val)
    {
        return detail::lower_bound_impl(rng,val);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    lower_bound(Rng & rng,Val const & val,Cmp cmp)
    {
        return std::lower_bound(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    lower_bound(Rng const & rng,Val const & val,Cmp cmp)
    {
        return std::lower_bound(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_upper_bound<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        upper_bound_impl(Rng & rng,Val const & val)
        {
            return rng.upper_bound(val);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_upper_bound<Rng>
          , ::boost::range_result_iterator<Rng>
        >::type
        upper_bound_impl(Rng & rng,Val const & val)
        {
            return std::upper_bound(detail::adl_begin(rng),detail::adl_end(rng),val);
        }
    }

    // template function upper_bound
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    upper_bound(Rng & rng,Val const & val)
    {
        return detail::upper_bound_impl(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    upper_bound(Rng const & rng,Val const & val)
    {
        return detail::upper_bound_impl(rng,val);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    upper_bound(Rng & rng,Val const & val,Cmp cmp)
    {
        return std::upper_bound(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    upper_bound(Rng const & rng,Val const & val,Cmp cmp)
    {
        return std::upper_bound(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    namespace detail
    {
        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_enable_if<
            has_equal_range<Rng>
          , iter_pair<Rng>
        >::type
        equal_range_impl(Rng & rng,Val const & val)
        {
            return rng.equal_range(val);
        }

        template<typename Rng,typename Val>
        inline BOOST_DEDUCED_TYPENAME ::boost::lazy_disable_if<
            has_equal_range<Rng>
          , iter_pair<Rng>
        >::type
        equal_range_impl(Rng & rng,Val const & val)
        {
            return std::equal_range(detail::adl_begin(rng),detail::adl_end(rng),val);
        }
    }

    // template function equal_range
    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME detail::iter_pair<Rng>::type
    equal_range(Rng & rng,Val const & val)
    {
        return detail::equal_range_impl(rng,val);
    }

    template<typename Rng,typename Val>
    inline BOOST_DEDUCED_TYPENAME detail::iter_pair<Rng const>::type
    equal_range(Rng const & rng,Val const & val)
    {
        return detail::equal_range_impl(rng,val);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    equal_range(Rng & rng,Val const & val,Cmp cmp)
    {
        return std::equal_range(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    equal_range(Rng const & rng,Val const & val,Cmp cmp)
    {
        return std::equal_range(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    // template function binary_search
    template<typename Rng,typename Val>
    inline bool binary_search(Rng const & rng,Val const & val)
    {
        return std::binary_search(detail::adl_begin(rng),detail::adl_end(rng),val);
    }

    template<typename Rng,typename Val,typename Cmp>
    inline bool binary_search(Rng const & rng,Val const & val,Cmp cmp)
    {
        return std::binary_search(detail::adl_begin(rng),detail::adl_end(rng),val,cmp);
    }

    // template function merge
    template<typename Rng1,typename Rng2,typename OutIter>
    inline OutIter merge(Rng1 const & rng1,Rng2 const & rng2,OutIter out)
    {
        return std::merge(detail::adl_begin(rng1),detail::adl_end(rng1),
                          detail::adl_begin(rng2),detail::adl_end(rng2),out);
    }

    template<typename Rng1,typename Rng2,typename OutIter,typename Cmp>
    inline OutIter merge(Rng1 const & rng1,Rng2 const & rng2,OutIter out,Cmp cmp)
    {
        return std::merge(detail::adl_begin(rng1),detail::adl_end(rng1),
                          detail::adl_begin(rng2),detail::adl_end(rng2),out,cmp);
    }

    // template function inplace_merge
    template<typename Rng>
    inline void inplace_merge(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle)
    {
        std::inplace_merge(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    template<typename Rng>
    inline void inplace_merge(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle)
    {
        std::inplace_merge(detail::adl_begin(rng),middle,detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void inplace_merge(Rng & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type middle,
        Cmp cmp)
    {
        std::inplace_merge(detail::adl_begin(rng),middle,detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void inplace_merge(Rng const & rng,
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type middle,
        Cmp cmp)
    {
        std::inplace_merge(detail::adl_begin(rng),middle,detail::adl_end(rng),cmp);
    }

    /////////////////////////////////////////////////////////////////////////
    // Set Algorithms
    /////////////////////////////////////////////////////////////////////////

    // template function includes
    template<typename Rng1,typename Rng2>
    inline bool includes(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::includes(detail::adl_begin(rng1),detail::adl_end(rng1),
                             detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename Cmp>
    inline bool includes(Rng1 const & rng1,Rng2 const & rng2,Cmp cmp)
    {
        return std::includes(detail::adl_begin(rng1),detail::adl_end(rng1),
                            detail::adl_begin(rng2),detail::adl_end(rng2),cmp);
    }

    // template function set_union
    template<typename Rng1,typename Rng2,typename OutIter>
    inline OutIter set_union(Rng1 const & rng1,Rng2 const & rng2,OutIter out)
    {
        return std::set_union(detail::adl_begin(rng1),detail::adl_end(rng1),
                              detail::adl_begin(rng2),detail::adl_end(rng2),out);
    }

    template<typename Rng1,typename Rng2,typename OutIter,typename Cmp>
    inline OutIter set_union(Rng1 const & rng1,Rng2 const & rng2,OutIter out,Cmp cmp)
    {
        return std::set_union(detail::adl_begin(rng1),detail::adl_end(rng1),
                              detail::adl_begin(rng2),detail::adl_end(rng2),out,cmp);
    }

    // template function set_intersection
    template<typename Rng1,typename Rng2,typename OutIter>
    inline OutIter set_intersection(Rng1 const & rng1,Rng2 const & rng2,OutIter out)
    {
        return std::set_intersection(detail::adl_begin(rng1),detail::adl_end(rng1),
                                     detail::adl_begin(rng1),detail::adl_end(rng1),out);
    }

    template<typename Rng1,typename Rng2,typename OutIter,typename Cmp>
    inline OutIter set_intersection(Rng1 const & rng1,Rng2 const & rng2,OutIter out,Cmp cmp)
    {
        return std::set_intersection(detail::adl_begin(rng1),detail::adl_end(rng1),
                                     detail::adl_begin(rng1),detail::adl_end(rng1),out,cmp);
    }

    // template function set_difference
    template<typename Rng1,typename Rng2,typename OutIter>
    inline OutIter set_difference(Rng1 const & rng1,Rng2 const & rng2,OutIter out)
    {
        return std::set_difference(detail::adl_begin(rng1),detail::adl_end(rng1),
                                   detail::adl_begin(rng2),detail::adl_end(rng2),out);
    }

    template<typename Rng1,typename Rng2,typename OutIter,typename Cmp>
    inline OutIter set_difference(Rng1 const & rng1,Rng2 const & rng2,OutIter out,Cmp cmp)
    {
        return std::set_difference(detail::adl_begin(rng1),detail::adl_end(rng1),
                                   detail::adl_begin(rng2),detail::adl_end(rng2),out,cmp);
    }

    // template function set_symmetric_difference
    template<typename Rng1,typename Rng2,typename OutIter>
    inline OutIter set_symmetric_difference(Rng1 const & rng1,Rng2 const & rng2,OutIter out)
    {
        return std::set_symmetric_difference(detail::adl_begin(rng1),detail::adl_end(rng1),
                                             detail::adl_begin(rng2),detail::adl_end(rng2),out);
    }

    template<typename Rng1,typename Rng2,typename OutIter,typename Cmp>
    inline OutIter set_symmetric_difference(Rng1 const & rng1,Rng2 const & rng2,OutIter out,Cmp cmp)
    {
        return std::set_symmetric_difference(detail::adl_begin(rng1),detail::adl_end(rng1),
                                             detail::adl_begin(rng2),detail::adl_end(rng2),out,cmp);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Heap Operations
    ///////////////////////////////////////////////////////////////////////////

    // template function push_heap
    template<typename Rng>
    inline void push_heap(Rng & rng)
    {
        std::push_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void push_heap(Rng const & rng)
    {
        std::push_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void push_heap(Rng & rng,Cmp cmp)
    {
        std::push_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void push_heap(Rng const & rng,Cmp cmp)
    {
        std::push_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    // template function pop_heap
    template<typename Rng>
    inline void pop_heap(Rng & rng)
    {
        std::pop_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void pop_heap(Rng const & rng)
    {
        std::pop_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void pop_heap(Rng & rng,Cmp cmp)
    {
        std::pop_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void pop_heap(Rng const & rng,Cmp cmp)
    {
        std::pop_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    // template function make_heap
    template<typename Rng>
    inline void make_heap(Rng & rng)
    {
        std::make_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void make_heap(Rng const & rng)
    {
        std::make_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void make_heap(Rng & rng,Cmp cmp)
    {
        std::make_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void make_heap(Rng const & rng,Cmp cmp)
    {
        std::make_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    // template function sort_heap
    template<typename Rng>
    inline void sort_heap(Rng & rng)
    {
        std::sort_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline void sort_heap(Rng const & rng)
    {
        std::sort_heap(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline void sort_heap(Rng & rng,Cmp cmp)
    {
        std::sort_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline void sort_heap(Rng const & rng,Cmp cmp)
    {
        std::sort_heap(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    /////////////////////////////////////////////////////////////////////////
    // Minimum and Maximum
    /////////////////////////////////////////////////////////////////////////

    // template function min_element
    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    min_element(Rng & rng)
    {
        return std::min_element(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    min_element(Rng const & rng)
    {
        return std::min_element(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    min_element(Rng & rng,BinPred pred)
    {
        return std::min_element(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    min_element(Rng const & rng,BinPred pred)
    {
        return std::min_element(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function max_element
    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    max_element(Rng & rng)
    {
        return std::max_element(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    max_element(Rng const & rng)
    {
        return std::max_element(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_iterator<Rng>::type
    max_element(Rng & rng,BinPred pred)
    {
        return std::max_element(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    template<typename Rng,typename BinPred>
    inline BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<Rng>::type
    max_element(Rng const & rng,BinPred pred)
    {
        return std::max_element(detail::adl_begin(rng),detail::adl_end(rng),pred);
    }

    // template function lexicographic_compare
    template<typename Rng1,typename Rng2>
    inline bool lexicographical_compare(Rng1 const & rng1,Rng2 const & rng2)
    {
        return std::lexicographical_compare(detail::adl_begin(rng1),detail::adl_end(rng1),
                                            detail::adl_begin(rng2),detail::adl_end(rng2));
    }

    template<typename Rng1,typename Rng2,typename BinPred>
    inline bool lexicographical_compare(Rng1 const & rng1,Rng2 const & rng2,BinPred pred)
    {
        return std::lexicographical_compare(detail::adl_begin(rng1),detail::adl_end(rng1),
                                            detail::adl_begin(rng2),detail::adl_end(rng2),pred);
    }

    /////////////////////////////////////////////////////////////////////////
    // Permutations
    /////////////////////////////////////////////////////////////////////////

    // template function next_permutation
    template<typename Rng>
    inline bool next_permutation(Rng & rng)
    {
        return std::next_permutation(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline bool next_permutation(Rng const & rng)
    {
        return std::next_permutation(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline bool next_permutation(Rng & rng,Cmp cmp)
    {
        return std::next_permutation(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline bool next_permutation(Rng const & rng,Cmp cmp)
    {
        return std::next_permutation(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    // template function prev_permutation
    template<typename Rng>
    inline bool prev_permutation(Rng & rng)
    {
        return std::prev_permutation(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng>
    inline bool prev_permutation(Rng const & rng)
    {
        return std::prev_permutation(detail::adl_begin(rng),detail::adl_end(rng));
    }

    template<typename Rng,typename Cmp>
    inline bool prev_permutation(Rng & rng,Cmp cmp)
    {
        return std::prev_permutation(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

    template<typename Rng,typename Cmp>
    inline bool prev_permutation(Rng const & rng,Cmp cmp)
    {
        return std::prev_permutation(detail::adl_begin(rng),detail::adl_end(rng),cmp);
    }

}

#endif

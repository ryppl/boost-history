/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_IO_DETAIL_IN_HPP
#define BOOST_FUSION_SEQUENCE_IO_DETAIL_IN_HPP

#include <istream>

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/sequence/io/detail/manip.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename Tag>
    struct delimiter_in
    {
        // read a delimiter
        template<typename IS>
        static void
        read(IS& is, char const* delim, mpl::false_ = mpl::false_())
        {
            detail::string_ios_manip<Tag, IS> manip(is);
            manip.read(delim);
        }

        template<typename IS>
        static void
        read(IS& is, char const* delim, mpl::true_)
        {
        }
    };

    struct read_sequence_loop
    {
        template<typename IS, typename Begin, typename End>
        static void
        call(IS& is, Begin const&, End const&, mpl::true_)
        {
        }

        template<typename IS, typename Begin, typename End>
        static void
        call(IS& is, Begin const& first, End const& last, mpl::false_)
        {
            typedef typename
                result_of::equal_to<
                    typename result_of::next<Begin>::type
                  , End
                >::type
            is_last;

            is >> fusion::deref(first);
            delimiter_in<tuple_delimiter_tag>::read(is, " ", is_last());
            call(is, fusion::next(first), last, is_last());
        }

        template<typename IS, typename Begin, typename End>
        static void
        call(IS& is, Begin const& first, End const& last)
        {
            call(is,
                    first,
                    last,
                    typename result_of::equal_to<
                        Begin const&
                      , End const&
                    >::type());
        }
    };

    template<typename IS, typename Seq>
    inline void
    read_sequence(IS& is, Seq& seq)
    {
        delimiter_in<tuple_open_tag>::read(is, "(");
        read_sequence_loop::call(is, fusion::begin(seq), fusion::end(seq));
        delimiter_in<tuple_close_tag>::read(is, ")");
    }
}}}

#endif

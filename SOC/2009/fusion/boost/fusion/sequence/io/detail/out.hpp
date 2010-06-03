/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_IO_DETAIL_OUT_HPP
#define BOOST_FUSION_SEQUENCE_IO_DETAIL_OUT_HPP

#include <ostream>
#include <boost/fusion/sequence/io/detail/manip.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename Tag>
    struct delimiter_out
    {
        // print a delimiter
        template<typename OS>
        static void
        print(OS& os, char const* delim, mpl::false_ = mpl::false_())
        {
            detail::string_ios_manip<Tag, OS> manip(os);
            manip.print(delim);
        }

        template<typename OS>
        static void
        print(OS& os, char const* delim, mpl::true_)
        {
        }
    };

    struct print_sequence_loop
    {
        template<typename OS, typename Begin, typename End>
        static void
        call(OS& os, Begin const&, End const&, mpl::true_)
        {
        }

        template<typename OS, typename Begin, typename End>
        static void
        call(OS& os, Begin const& first, End const& last, mpl::false_)
        {
            typedef typename
                result_of::equal_to<
                    typename result_of::next<Begin>::type
                  , End
                >::type
            is_last;

            os << fusion::deref(first);
            delimiter_out<tuple_delimiter_tag>::print(os, " ", is_last());
            call(os, fusion::next(first), last, is_last());
        }

        template<typename OS, typename Begin, typename End>
        static void
        call(OS& os, Begin const& first, End const& last)
        {
            call(os,
                    first,
                    last,
                    typename result_of::equal_to<
                        Begin const&
                      , End const&
                    >::type());
        }
    };

    template<typename OS, typename Seq>
    inline void
    print_sequence(OS& os, Seq const& seq)
    {
        delimiter_out<tuple_open_tag>::print(os, "(");
        print_sequence_loop::call(os, fusion::begin(seq), fusion::end(seq));
        delimiter_out<tuple_close_tag>::print(os, ")");
    }
}}}

#endif

/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_SLICE_VIEW_HPP)
#define BOOST_FUSION_SLICE_VIEW_HPP

#include <boost/spirit/fusion/iterator.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  slice_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BeginT, typename EndT>
    struct slice_view
    {
        typedef BeginT begin_type;
        typedef EndT end_type;

        slice_view(BeginT const& first, EndT const& last)
            : first(first), last(last) {}

        begin_type
        begin() const
        {
            return first;
        }

        end_type
        end() const
        {
            return last;
        }

        BeginT first;
        EndT last;
    };

}} // namespace boost::fusion

#endif



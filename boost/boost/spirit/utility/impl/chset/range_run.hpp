/*=============================================================================
    Spirit V1.5.2
    Copyright (c) 2001-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_RANGE_RUN_HPP
#define BOOST_SPIRIT_RANGE_RUN_HPP

///////////////////////////////////////////////////////////////////////////////
#include <vector>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace impl {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  range class
    //
    //      Implements a closed range of values. This class is used in
    //      the implementation of the range_run class.
    //
    //      { Low level implementation detail }
    //      { Not to be confused with boost::spirit::range }
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct range {

                        range(CharT first, CharT last);

        bool            is_valid() const;
        bool            includes(CharT v) const;
        bool            includes(range const& r) const;
        bool            is_adjacent(range const& r) const;
        void            merge(range const& r);

        CharT first;
        CharT last;
    };

    //////////////////////////////////
    template <typename CharT>
    struct range_char_compare {

        bool operator()(range<CharT> const& x, const CharT y) const
        { return x.first < y; }
    };

    //////////////////////////////////
    template <typename CharT>
    struct range_compare {

        bool operator()(range<CharT> const& x, range<CharT> const& y) const
        { return x.first < y.first; }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  range_run
    //
    //      An implementation of a sparse bit (boolean) set. The set uses
    //      a sorted vector of disjoint ranges. This class implements the
    //      bare minimum essentials from which the full range of set
    //      operators can be implemented. The set is constructed from
    //      ranges. Internally, adjacent or overlapping ranges are
    //      coalesced.
    //
    //      range_runs are very space-economical in situations where there
    //      are lots of ranges and a few individual disjoint values.
    //      Searching is O(log n) where n is the number of ranges.
    //
    //      { Low level implementation detail }
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    class range_run {

    public:

        typedef range<CharT> range_t;
        typedef std::vector<range_t> run_t;
        typedef typename run_t::iterator iterator;
        typedef typename run_t::const_iterator const_iterator;

        void            swap(range_run& rr);
        bool            test(CharT v) const;
        void            set(range_t const& r);
        void            clear(range_t const& r);
        void            clear();

        const_iterator  begin() const;
        const_iterator  end() const;

    private:

        void            merge(iterator iter, range_t const& r);

        run_t run;
    };

}}} // namespace boost::spirit::impl

#endif

#if !defined(BOOST_SPIRIT_RANGE_RUN_IPP)
#include "boost/spirit/utility/impl/chset/range_run.ipp"
#endif

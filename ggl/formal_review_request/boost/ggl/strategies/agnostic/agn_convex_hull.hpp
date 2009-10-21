// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_AGNOSTIC_CONVEX_HULL_HPP
#define GGL_STRATEGIES_AGNOSTIC_CONVEX_HULL_HPP


#include <cstddef>
#include <algorithm>
#include <vector>

#include <boost/range/functions.hpp>
#include <boost/concept_check.hpp> // for ignore-variable

#include <ggl/core/cs.hpp>
#include <ggl/strategies/strategy_traits.hpp>

#include <ggl/util/less.hpp>



// TODO: Temporary, comparing tests, this can be removed in the end
#if defined(GGL_USE_SMOOTH_SORT)
#  include "SmoothSort.hpp"
#elif defined(GGL_USE_MERGE_SORT)
#  include "MergeSort.hpp"
#else
#endif

namespace ggl
{

namespace strategy { namespace convex_hull {

#ifndef DOXYGEN_NO_DETAIL
namespace detail
{

template <typename Range, typename RangeIterator, typename Strategy>
static inline void get_extremes(const Range& range,
            RangeIterator& min_it, RangeIterator& max_it,
            const Strategy& strategy)
{
    min_it = boost::begin(range);
    max_it = boost::begin(range);

    for (RangeIterator it = boost::begin(range) + 1; it != boost::end(range); ++it)
    {
        if (strategy.smaller(*it, *min_it))
        {
            min_it = it;
        }

        if (strategy.larger(*it, *max_it))
        {
            max_it = it;
        }
    }
}

template <typename R>
static inline void sort(R& range)
{
    typedef typename boost::range_value<R>::type point_type;

#if defined(USE_SMOOTH_SORT)
    smoothsort::sort
#elif defined(USE_MERGE_SORT)
    comparing::merge_sort<thread_count>
#else
    std::sort
#endif

        (boost::begin(range), boost::end(range), ggl::less<point_type>());
}

} // namespace detail
#endif // DOXYGEN_NO_DETAIL


// Completely reworked version from source at:
// http://www.ddj.com/architect/201806315
// also available at http://marknelson.us/2007/08/22/convex
template <typename P>
class graham
{
private:

    typedef typename cs_tag<P>::type cs_tag;
    typedef typename std::vector<P> container;
    typedef typename std::vector<P>::const_iterator iterator;
    typedef typename std::vector<P>::const_reverse_iterator rev_iterator;

    container m_lower_hull;
    container m_upper_hull;
    container m_copied_input;


public:

    // Default constructor, ranges can be added using "add_range" but note they'll be copied
    inline graham()
    {
    }

    // Constructor with a range
    template <typename Range>
    inline graham(const Range& range)
    {
        handle_range(range);
    }


    template <typename OutputIterator>
    inline void get(OutputIterator out, bool clockwise)
    {
        if (clockwise)
        {
            get_range_forward(m_upper_hull, out);
            get_range_reverse(m_lower_hull, out);
        }
        else
        {
            get_range_forward(m_lower_hull, out);
            get_range_reverse(m_upper_hull, out);
        }
    }


    // TODO:
    // Consider if it is better to create an iterator over a multi, which is then used here,
    // instead of copying the range
    // It makes it slightly more complicated but avoids the copy, which is attractive because
    // multi-polygons (where it is used for) can be large.
    template <typename Range>
    inline void add_range(const Range& range)
    {
        std::copy(boost::begin(range), boost::end(range), std::back_inserter(m_copied_input));
    }

    inline void handle_input()
    {
        handle_range(m_copied_input);
    }


private:

    template <typename Range>
    inline void handle_range(const Range& range)
    {
        typedef typename boost::range_const_iterator<Range>::type range_iterator;

        // Polygons with three corners, or closed with 4 points, are always convex
        if (boost::size(range) <= 3)
        {
            for (range_iterator it = boost::begin(range);
                        it != boost::end(range);
                        ++it)
            {
                m_upper_hull.push_back(*it);
            }
            return;
        }

        // Get min/max (in most cases left / right) points
        range_iterator left_it, right_it;
        typename strategy_compare<cs_tag, P, 0>::type comparing;
        detail::get_extremes(range, left_it, right_it, comparing);

        // Bounding left/right points
        container lower_points, upper_points;

        assign_range(range, left_it, right_it, lower_points, upper_points);

        detail::sort(lower_points);
        detail::sort(upper_points);

        build_half_hull<-1>(lower_points, m_lower_hull, *left_it, *right_it);
        build_half_hull<1>(upper_points, m_upper_hull, *left_it, *right_it);
    }



    template <typename RangeIterator, typename Range>
    inline void assign_range(const Range& range,
            const RangeIterator& left_it,
            const RangeIterator& right_it,
            container& lower_points,
            container& upper_points)
    {
        typename strategy_side<cs_tag, P>::type side;
        boost::ignore_unused_variable_warning(side);

        // Put points in one of the two output sequences
        for (RangeIterator it = boost::begin(range);
            it != boost::end(range);
            ++it)
        {
            if (it != left_it && it != right_it)
            {
                int dir = side.side(*left_it, *right_it, *it);
                switch(dir)
                {
                    case 1 : // left
                        upper_points.push_back(*it);
                        break;
                    case -1 : // right
                        lower_points.push_back(*it);
                        break;
                    // zero: on line left-right, never part of hull
                }
            }
        }
    }


    template <int Factor>
    inline void build_half_hull(const container& input, container& output,
            const P& left, const P& right)
    {
        output.push_back(left);
        for(iterator it = input.begin(); it != input.end(); ++it)
        {
            add_to_hull<Factor>(*it, output);
        }
        add_to_hull<Factor>(right, output);
    }

    template <int Factor>
    inline void add_to_hull(const P& p, container& output)
    {
        typename strategy_side<cs_tag, P>::type side;
        boost::ignore_unused_variable_warning(side);

        output.push_back(p);
        register std::size_t output_size = output.size();
        while (output_size >= 3)
        {
            rev_iterator rit = output.rbegin();
            const P& last = *rit++;
            const P& last2 = *rit++;

            if (Factor * side.side(*rit, last, last2) <= 0)
            {
                // Remove last two points from stack, and add last again
                // This is much faster then erasing the one but last.
                output.pop_back();
                output.pop_back();
                output.push_back(last);
                output_size--;
            }
            else
            {
                return;
            }
        }
    }

    template <typename Range, typename OutputIterator>
    inline void get_range_forward(Range const& range, OutputIterator out)
    {
        for (iterator it = range.begin(); it != range.end(); ++it, ++out)
        {
            *out = *it;
        }
    }

    template <typename Range, typename OutputIterator>
    inline void get_range_reverse(Range const& range, OutputIterator out)
    {
        // STL Port does not accept iterating from rbegin+1 to rend
        std::size_t size = range.size();
        if (size > 0)
        {
            rev_iterator it = range.rbegin() + 1;
            for (std::size_t i = 1; i < size; ++i, ++it, ++out)
            {
                *out = *it;
            }
        }
    }

};

}} // namespace strategy::convex_hull


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
template <typename P>
struct strategy_convex_hull<cartesian_tag, P>
{
    typedef strategy::convex_hull::graham<P> type;
};
#endif

} // namespace ggl


#endif // GGL_STRATEGY_AGNOSTIC_CONVEX_HULL_HPP

//  Boost string_algo library benchmark_finder.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_BENCHMARK_FINDER_HPP
#define BOOST_STRING_BENCHMARK_FINDER_HPP

#include <boost/config.hpp>

#include <boost/algorithm/string/config.hpp>
#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/string_search/naive_search.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <boost/tuple/tuple.hpp>

#include <boost/mpl/void.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/distance.hpp>

#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>

#include <boost/spirit/home/phoenix/function.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>

//\todo pretty messy, Boost.Chrono?
#ifdef BOOST_WINDOWS
#include <windows.h>
#else
#include <boost/timer.hpp>
#endif

#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include <boost/throw_exception.hpp>


/*! \file
Defines a generic finder type with an iterface similar to that of \ref simplified_finder_t.
This finder type is useful for comparing the performance of various string search algorithms.
*/

namespace boost { namespace algorithm {

    //! A generic finder type which benchmarks string search algorithms
    /** Possesses a similar interface to \ref simplified_finder_t and allows to test
        the performance of various string search algorithms in order to allow easier
        choice of the right algorithm for a certain data set
        \tparam Range1T A range representing the type of the substring (the pattern)
        \tparam Range2T A range representing the type of the string (the text)
        \tparam AlgorithmSequenceT A MPL sequence containing algorithm types that are to be benchmarked
        \tparam ComparatorT The comparator type passed to the algorithms
    */
    template <class Range1T, class Range2T, class AlgorithmSequenceT,
    class ComparatorT = boost::algorithm::is_equal>
    class benchmark_finder_t
    {
        typedef std::allocator<std::size_t> allocator_type_;
    public:
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS(Range1T,Range2T);
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS2(ComparatorT, allocator_type_);
    public:

        //! \copydoc boost::algorithm::simplified_finder_t::set_substring
        //! \see boost::algorithm::simplified_finder_t::set_substring
        void set_substring (substring_type
            const *const substring)
        {
            boost::phoenix::function<finder_set_substring> f;
            boost::fusion::for_each(finders, 
                f(boost::phoenix::arg_names::arg1, substring)
            );
            trusted_finder.set_substring(substring);
        }

        //! \copydoc boost::algorithm::simplified_finder_t::set_string
        //! \see boost::algorithm::simplified_finder_t::set_string
        void set_string (string_type *const string)
        {
            boost::phoenix::function<finder_set_string> f;
            boost::fusion::for_each(
                finders,
                f(boost::phoenix::arg_names::arg1, string)
            );
            trusted_finder.set_string(string);
        }

        //! Clears all the benchmark data obtained from searching
        void clear ()
        { boost::fusion::for_each(finders, clear_stats()); }
        
        //! \copydoc boost::algorithm::simplified_finder_t::find_reset
        //! \see boost::algorithm::simplified_finder_t::find_reset
        void find_reset()
        {
            boost::fusion::for_each(finders, finder_reset());
            trusted_finder.find_reset();
        }

        //! \copydoc boost::algorithm::simplified_finder_t::find_next
        //! \see boost::algorithm::simplified_finder_t::find_next
        string_range_type find_next()
        {
            return boost::fusion::fold(finders, trusted_finder.find_next(),
                finder_benchmark_and_test());
        }

        //! \copydoc boost::algorithm::simplified_finder_t::find_first
        //! \sa boost::algorithm::simplified_finder_t::find_first
        string_range_type find_first()
        { find_reset(); return find_next(); }

        //! Output the benchmark data to a stream
        /*!
            \param output The stream to which the benchmark results are to be outputted
        */
        template <class CharT, class TraitsT>
        void output_stats (std::basic_ostream<CharT, TraitsT> &output)
        {
            boost::phoenix::function<output_stats_> f;
            boost::fusion::for_each(finders, f(boost::phoenix::ref(output), boost::phoenix::arg_names::arg1) );
        }
    private:
        typedef BOOST_STRING_TYPENAME boost::mpl::transform<AlgorithmSequenceT,
            std::pair<
                BOOST_STRING_TYPENAME boost::algorithm::simplified_finder_t<substring_type,string_type,
                    boost::mpl::_, ComparatorT>,
                std::deque<double>
            >
        >::type finders_sequence;
        BOOST_STRING_TYPENAME boost::simplified_finder_t<substring_type, string_type, boost::naive_search,
            ComparatorT> trusted_finder;

        BOOST_STRING_TYPENAME boost::fusion::result_of::as_vector< finders_sequence >::type finders;

        struct finder_set_substring
        {
            template <class,class> struct result { typedef void type; };

            template <class Finder>
            void operator() (Finder &finder, substring_type const *const substring) const
            { finder.first.set_substring(substring); }
        };

        struct finder_set_string
        {
            template <class,class> struct result { typedef void type; };

            template <class Finder>
            void operator() (Finder &finder, string_type *const string) const
            { finder.first.set_string(string); }
        };

        struct clear_stats
        {
            template <class Finder>
            void operator() (Finder &finder) const { finder.second.clear(); }
        };

        //template <class Finder>
        //static void finder_reset (Finder &finder) { finder.first.find_reset(); }

        struct finder_reset
        {
            template <class Finder>
            void operator() (Finder &finder) const { finder.first.find_reset(); }
        };

        //todo Make sure tests are performed properly (via returning etc.)
        struct finder_benchmark_and_test
        {
            template <class> struct result { typedef string_range_type type; };

            template <class Finder>
            string_range_type operator() (string_range_type correct, Finder &finder) const
            {
                string_range_type ret;
                //pray for Boost.Chrono, I don't like this.
                double elapsed;
                try {
#                   ifdef BOOST_WINDOWS
                    LARGE_INTEGER start, end, freq;
                    QueryPerformanceFrequency(&freq);
                    QueryPerformanceCounter(&start);
#                   else
                    boost::timer t;
#                   endif

                    ret = finder.first.find_next();

#                   ifdef BOOST_WINDOWS
                    QueryPerformanceCounter(&end);
                    elapsed = (double)(end.QuadPart - start.QuadPart)/freq.QuadPart;
#                   else
                    elapsed = t.elapsed();
#                   endif
                } catch (std::exception const &e) { BOOST_THROW_EXCEPTION(e); }
                bool is_correct = boost::equal(correct, ret);
                //assert(is_correct);
                if (!is_correct)
                {
                    std::ostringstream ss;

                    ss << "Match failed on " << finder.first.get_algorithm_name()
                        << " with:\n\tstr["<<boost::distance(finder.first.get_string_range())
                        << "]=";
                    boost::copy(finder.first.get_string_range(), std::ostream_iterator<char>(ss));

                    ss << "\n\tsubstr["<<boost::distance(finder.first.get_substring_range())
                        << "]=";
                    boost::copy(finder.first.get_substring_range(), std::ostream_iterator<char>(ss));

                    BOOST_THROW_EXCEPTION( std::runtime_error(ss.str()) );
                }
                finder.second.push_back(elapsed);
                return correct;
            }
        };

        struct output_stats_
        {
            template <class,class> struct result { typedef void type; };

            template <class CharT, class Finder, class TraitsT>
            void operator() (std::basic_ostream<CharT, TraitsT> &os, Finder &finder) const
            {
                std::deque<double> &t = finder.second;
                std::deque<double>::size_type size = finder.second.size();
                typedef std::deque<double>::const_iterator It;
                if (!size) return;
                double min = *t.begin(), max = *t.begin(), avg=0., stddev=0.;//, median;
                //std::sort(t.begin(), t.end());
                
                //if (size&1) median = t[size>>1];
                //else median = t[size>>1] + t[1 + (size>>1)];
                

                for (It it = t.begin(); it != t.end(); ++it)
                {
                    if (*it < min) min = *it;
                    if (*it > max) max = *it;
                    avg += *it;
                    //os << *it << ' ';
                }
                avg /= size;
                for (It it = t.begin(); it != t.end(); ++it)
                    stddev += (avg - *it) * (avg - *it); // square of absolute deviation of *it w.r.t. avg
                stddev /= size;
                stddev = std::sqrt(stddev); // stddev is square root of variance
                os << finder.first.get_algorithm_name() << "\n";
                os << "Min   : " << min << "\n"
                    << "Max   : " << max << "\n"
                    << "Avg   : " << avg << "\n"
                    << "Stddev: " << stddev << "\n"
                    << "Smples: " << size << "\n";
                os << "==========================================" << std::endl;
            }
        };

    };
} }

namespace boost { using algorithm::benchmark_finder_t; }

#endif

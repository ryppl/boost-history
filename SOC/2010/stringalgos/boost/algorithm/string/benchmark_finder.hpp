#ifndef BOOST_STRING_BENCHMARK_FINDER_HPP
#define BOOST_STRING_BENCHMARK_FINDER_HPP


//Our finder has 6 template params, which means we cannot use it in a placeholder expression
//unless MPL supports metafunctions with arity >= 6
#ifdef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
# if BOOST_MPL_LIMIT_METAFUNCTION_ARITY < 6 || !defined BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#  error "benchmark_finder.hpp requires BOOST_MPL_LIMIT_METAFUNCTION_ARITY to be at least 6. " \
    "Either configure MPL to have metafunction arity >= 6 or include benchmark_finder.hpp " \
    "before any other header."
# endif
#else
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 6
#endif

#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/detail/finder.hpp>
#include <boost/algorithm/string/string_search/naive_search.hpp>

#include <boost/tuple/tuple.hpp>

//#include <boost/mpl/transform.hpp>
//#include <boost/mpl/vector/vector0.hpp>
//#include <boost/mpl/push_back.hpp>
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


#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include <boost/throw_exception.hpp>

//!\todo use something more accurate
#include <boost/timer.hpp>

namespace boost { namespace algorithm {

    template <class Range1T, class Range2T, class AlgorithmSequenceT,
        class ComparatorT/*,
        template <class,class,class,class,class,class> class AdditionalBehaviorT*/>
    class benchmark_finder :
        public boost::algorithm::detail::finder_typedefs<Range1T, Range2T,
        ComparatorT, std::allocator<std::size_t> >
    {
    public:

        void set_substring (Range1T const *const substring)
        {
            boost::phoenix::function<finder_set_substring> f;
            boost::fusion::for_each(finders, 
                f(boost::phoenix::arg_names::arg1, substring)
            );
            trusted_finder.set_substring(substring);
        }

        void set_string (Range2T *const string)
        {
            boost::phoenix::function<finder_set_string> f;
            boost::fusion::for_each(
                finders,
                f(boost::phoenix::arg_names::arg1, string)
            );
            trusted_finder.set_string(string);
        }

        void clear ()
        { boost::fusion::for_each(finders, clear_stats()); }
        
        void find_reset()
        {
            boost::fusion::for_each(finders, finder_reset());
            trusted_finder.find_reset();
        }

        string_range_type find_next()
        {
            return boost::fusion::fold(finders, trusted_finder.find_next(),
                finder_benchmark_and_test());
        }

        string_range_type find_first()
        { find_reset(); return find_next(); }

        /*void refresh()
        {
            boost::fusion::for_each(finders, finder_refresh());
            trusted_finder.refresh();
        }

        void refresh_string()
        {
            boost::fusion::for_each(finders, finder_refresh_string());
            trusted_finder.refresh_string();
        }

        void refresh_substring()
        {
            boost::fusion::for_each(finders, finder_refresh_substring());
            trusted_finder.refresh_substring();
        }*/

        template <class CharT, class TraitsT>
        void output_stats (std::basic_ostream<CharT, TraitsT> &os)
        {
            boost::phoenix::function<output_stats_> f;
            boost::fusion::for_each(finders, f(boost::phoenix::ref(os), boost::phoenix::arg_names::arg1) );
        }
    private:
        //! \todo write a proxy type allowing us to have a simplified_finder_t3 taking only the first
        //!     3 template params. use that in boost::mpl::transform for eliminating the metafunction increased
        //!     arity requirement
        typedef typename boost::mpl::transform<AlgorithmSequenceT,
            std::pair<
                typename boost::algorithm::simplified_finder_t<Range1T,Range2T,
                    boost::mpl::_, ComparatorT>,
                std::deque<double>
            >
        >::type finders_sequence;
        typename boost::simplified_finder_t<Range1T, Range2T, boost::naive_search,
            ComparatorT> trusted_finder;

        typename boost::fusion::result_of::as_vector< finders_sequence >::type finders;

        struct finder_set_substring
        {
            template <class,class> struct result { typedef void type; };

            template <class Finder>
            void operator() (Finder &finder, Range1T const *const substring) const
            { finder.first.set_substring(substring); }
        };

        struct finder_set_string
        {
            template <class,class> struct result { typedef void type; };

            template <class Finder>
            void operator() (Finder &finder, Range2T *const string) const
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

        //!\todo Make sure tests are performed properly (via returning etc.)
        struct finder_benchmark_and_test
        {
            template <class> struct result { typedef string_range_type type; };

            template <class Finder>
            string_range_type operator() (string_range_type correct, Finder &finder) const
            {
                string_range_type ret;
                double time;
                try {
                    boost::timer t;
                    ret = finder.first.find_next();
                    time = t.elapsed();
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
                finder.second.push_back(time);
                return correct;
            }
        };

        /*struct finder_refresh
        {
            template <class> struct result { typedef void type; };

            template <class Finder>
            void operator () (Finder &finder) const
            { finder.first.refresh(); }
        };

        struct finder_refresh_string
        {
            template <class> struct result { typedef void type; };

            template <class Finder>
            void operator () (Finder &finder) const
            { finder.first.refresh_string(); }
        };

        struct finder_refresh_substring
        {
            template <class> struct result { typedef void type; };

            template <class Finder>
            void operator () (Finder &finder) const
            { finder.first.refresh_substring(); }
        };*/

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
                
                //!\todo start from t.begin()+1, now we just want to check the values.
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

namespace boost { using algorithm::benchmark_finder; }

#endif
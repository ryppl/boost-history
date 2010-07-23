#ifndef BOOST_ALGORITHM_KNUTH_MORRIS_PRATT_HPP
#define BOOST_ALGORITHM_KNUTH_MORRIS_PRATT_HPP

#include <iterator>
#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <string>
#include <vector>
#include <memory>
#include <boost/algorithm/string/detail/finder.hpp>

namespace boost { namespace algorithm {
    struct knuth_morris_pratt
    {

        template <class Finder,class RandomAccessRange1T,
            class RandomAccessRange2T,class ComparatorT,class AllocatorT>
        class algorithm
            : public boost::algorithm::detail::finder_typedefs<
                RandomAccessRange1T,RandomAccessRange2T,ComparatorT,AllocatorT>
        {
        public:
            /*typedef RandomAccessIterator1T substring_iterator_type;
            typedef RandomAccessIterator2T string_iterator_type;
            typedef boost::iterator_range<RandomAccessIterator1T> substring_range_type;
            typedef boost::iterator_range<RandomAccessIterator2T> string_range_type;
            typedef Comparator comparator_type;*/
            std::string get_algorithm_name () const { return "Knuth-Morris-Pratt"; }
        protected:
            string_range_type find(string_iterator_type start)
            {
                return find(start, string_iterator_category());
            }

            void on_substring_change()
            {
                on_substring_change(substring_iterator_category());
            }

            void on_string_change()
            {
            }
        private:

            std::vector<std::size_t,
                typename AllocatorT::template rebind<std::size_t>::other > failure_func;

            string_range_type find(string_iterator_type start, std::random_access_iterator_tag)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                std::size_t idx = start - boost::begin(str),
                    str_size = boost::end(str) - boost::begin(str), q = 0,
                    substr_size = boost::end(substr) - boost::begin(substr);

                if (boost::begin(substr) == boost::end(substr))
                    return boost::iterator_range<string_iterator_type>(
                        start, start);

                while (idx < str_size)
                {
                    // Invariant: substring[0..q-1] == string[..idx-1]
                    
                    //if (boost::begin(substr)[q] == boost::begin(str)[idx])
                    if (comp(boost::begin(substr)[q], boost::begin(str)[idx]))
                    {
                        ++idx; ++q;
                        if (q == substr_size)
                            return boost::iterator_range<string_iterator_type>(
                                boost::begin(str)+(idx-substr_size),
                                boost::begin(str)+idx
                            );
                    }
                    else
                    {
                        if (q == 0) ++idx;
                        else q = failure_func[q];
                    }
                }
                return boost::iterator_range<string_iterator_type>(
                    boost::end(str), boost::end(str));
            }
            
            void on_substring_change(std::random_access_iterator_tag)
            {
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                //!\todo clear the container first
                failure_func.clear();
                failure_func.reserve(boost::end(substr) - boost::begin(substr));
                std::size_t idx, q, substr_size = boost::end(substr) - boost::begin(substr);
                failure_func.push_back(0); failure_func.push_back(0);
                
                if (boost::begin(substr) == boost::end(substr)) return;
                
                for (idx = 2; idx < substr_size; ++idx)
                {
                    q = failure_func[idx-1];
                    for (;;)
                    {
                        //if (boost::begin(substr)[q] == boost::begin(substr)[idx-1])
                        if (comp(boost::begin(substr)[q], boost::begin(substr)[idx-1]))
                        {
                            failure_func.push_back(q+1);
                            break;
                        }
                        else if (q == 0) { failure_func.push_back(0); break; }
                        q = failure_func[q];
                    }
                }
            }

        };
    };
} }

namespace boost
{
    using boost::algorithm::knuth_morris_pratt;
    typedef boost::algorithm::finder_t<std::string, std::string,
        boost::algorithm::knuth_morris_pratt> knuth_morris_pratt_finder;
    typedef boost::algorithm::finder_t<std::wstring, std::wstring,
        boost::algorithm::knuth_morris_pratt> wknuth_morris_pratt_finder;
}

#endif
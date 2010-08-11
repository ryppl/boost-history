#ifndef BOOST_ALGORITHM_GENERATED_FINDERS_HPP
#define BOOST_ALGORITHM_GENERATED_FINDERS_HPP

#include <boost/algorithm/string/finder/detail/last_finder_impl.hpp>
#include <boost/algorithm/string/finder/detail/nth_finder_impl.hpp>
#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/as_literal.hpp>

#include <memory>

namespace boost { namespace algorithm {

    template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
    class first_finder_t
        : public boost::algorithm::detail::finder_typedefs<Range1T, Range2T, ComparatorT, std::allocator<std::size_t> >
    {
    public:
        first_finder_t (Range1T const *const substr, ComparatorT comparator=ComparatorT())
            : finder(comparator) { finder.set_substring(substr); }
        string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
        {
            finder.set_string(string_start, string_end);
            return finder.find_first();
        }
    private:
        typedef boost::algorithm::simplified_finder_t<Range1T, Range2T, AlgorithmT, ComparatorT> internal_finder_type;
        internal_finder_type finder;
    };

    template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
    class last_finder_t
        : public boost::algorithm::detail::last_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT>
    {
    private:
        typedef boost::algorithm::detail::last_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT> impl_type;
    public:
        last_finder_t (Range1T const *const substr, ComparatorT comparator=ComparatorT())
            : impl_type(boost::as_literal(*substr), comparator) { }
    };

    template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
    class nth_finder_t
        : public boost::algorithm::detail::nth_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT>
    {
    private:
        typedef boost::algorithm::detail::nth_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT> impl_type;
    public:
        nth_finder_t (Range1T const *const substr, ComparatorT comparator=ComparatorT(), int n = 0)
            : impl_type(boost::as_literal(*substr), comparator, n)
          //  : finder(comparator), n_(n) { finder.set_substring(substr); }
        {
        }
        /*void set_n(int n) { n_ = n; }
        string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
        {
            //IMPORTANT TODO: THIS ONLY TREATS THE CASE OF N>=0, MAKE IT SO IT WORKS WITH N<=0 too
            string_range_type ret;
            finder.set_string(string_start, string_end);
            for (int n = 0; n <= n_; ++n)
            {
                ret = finder.find_next();
                if (boost::begin(ret) == boost::end(finder.get_string_range())) return ret;
            }
            return ret;
        }
    private:
        typedef boost::algorithm::simplified_finder_t<Range1T, Range2T, AlgorithmT, ComparatorT> internal_finder_type;
        int n_; // TODO: better type?
        internal_finder_type finder;*/
    };
} }

namespace boost
{
    using algorithm::first_finder_t;
    using algorithm::last_finder_t;
    using algorithm::nth_finder_t;
}

#endif // BOOST_ALGORITHM_GENERATED_FINDERS_HPP
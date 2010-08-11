#ifndef BOOST_ALGORITHM_DETAIL_NTH_FINDER_IMPL_HPP
#define BOOST_ALGORITHM_DETAIL_NTH_FINDER_IMPL_HPP

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/reverse_iterator.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/utility/enable_if.hpp>

#include <memory>
#include <iterator>
#include <stdexcept>

#include <boost/range/category.hpp>

#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>

namespace boost { namespace algorithm { namespace detail {
	
	template <class Range1T, class Range2T, class AlgorithmT,
	class ComparatorT, class Enable = void>
	class nth_finder_impl_t;
	
	//Implementation of last_finder_t when both ranges are bidirectional
	template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
	class nth_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT, 
	    typename boost::enable_if<
	    typename boost::mpl::and_<
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range1T>::type>,
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range2T>::type>
	    >
	>::type>
	: public boost::algorithm::detail::finder_typedefs<Range1T, Range2T, ComparatorT, std::allocator<std::size_t> >
	{      
    public:
        nth_finder_impl_t (substring_range_type const &substr, ComparatorT comparator=ComparatorT(), int n = 0)
            : n_(n), finder(comparator), reverse_finder(comparator)
        {
            reverse_finder.set_substring(substring_reverse_iterator_type(boost::end(substr)),
                substring_reverse_iterator_type(boost::begin(substr)));
            finder.set_substring(boost::begin(substr), boost::end(substr));
        }
        string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
        {
            if (n_ >= 0)
            {
                finder.set_string(string_start, string_end);
                string_range_type ret;
                for (int n = 0; n <= n_; ++n)
                {
                    ret = finder.find_next();
                    if (boost::begin(ret) == string_end)
                        return make_iterator_range(string_end, string_end);
                }
                return ret;
            }
            else
            {
                reverse_finder.set_string(
                    string_reverse_iterator_type(string_end), string_reverse_iterator_type(string_start));
                string_reverse_range_type ret;
                int n_2 = -n_ - 1;
                for (int n = 0; n <= n_2; ++n)
                {
                    ret = reverse_finder.find_next();
                    if (boost::begin(ret) == boost::end(reverse_finder.get_string_range()))
                        return boost::make_iterator_range(string_end, string_end);
                }
                return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base() );
            }
        }
        void set_n(int n)
        { n_ = n; }
    private:
        typedef typename boost::algorithm::simplified_finder_t<
            substring_reverse_range_type, string_reverse_range_type, AlgorithmT, ComparatorT> reverse_finder_type;
        typedef typename boost::algorithm::simplified_finder_t<
            substring_type, string_type, AlgorithmT, ComparatorT> finder_type;
        finder_type finder;
        reverse_finder_type reverse_finder;
        int n_;
	};
	
	//Implementation of nth_finder_t when at least one range is not bidirectional
	template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
	class nth_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT, 
	    typename boost::enable_if<
	    typename boost::mpl::or_<
	    typename boost::mpl::not_<
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range1T>::type>
	    >,
	    typename boost::mpl::not_<
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range2T>::type>
	    >
	    >
	>::type>
	: public boost::algorithm::detail::finder_typedefs<Range1T, Range2T, ComparatorT, std::allocator<std::size_t> >
	{
    public:
        nth_finder_impl_t (substring_range_type const &substr, ComparatorT comparator=ComparatorT(), int n = 0)
            : finder(comparator)
        {
            assert (n > = 0); // we do not have bidirectional iterators
            finder.set_substring(boost::begin(substr), boost::end(substr));
        }

        string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
        {
            finder.set_string(string_start, string_end);
            string_range_type ret;
            for (int n = 0; n <= n_; ++n)
            {
                ret = finder.find_next();
                if (boost::begin(ret) == string_end)
                    return boost::make_iterator_range(string_end, string_end);
            }
            return ret;
        }

        void set_n(int n)
        { assert(n >= 0); n_ = n; }
    private:
        typedef typename boost::algorithm::simplified_finder_t<
            substring_type, string_type, AlgorithmT, ComparatorT> finder_type;
        finder_type finder;
        int n_;
	};

} } }

#endif // BOOST_ALGORITHM_DETAIL_NTH_FINDER_IMPL_HPP
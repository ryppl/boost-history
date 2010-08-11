#ifndef BOOST_ALGORITHM_DETAIL_LAST_FINDER_IMPL_HPP
#define BOOST_ALGORITHM_DETAIL_LAST_FINDER_IMPL_HPP

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/reverse_iterator.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/utility/enable_if.hpp>

#include <memory>
#include <iterator>

#include <boost/range/category.hpp>

#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>


namespace boost { namespace algorithm { namespace detail {
	
	template <class Range1T, class Range2T, class AlgorithmT,
	class ComparatorT, class Enable = void>
	class last_finder_impl_t;
	
	//Implementation of last_finder_t when both ranges are bidirectional
	template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
	class last_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT, 
	    typename boost::enable_if<
	    typename boost::mpl::and_<
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range1T>::type>,
	    typename boost::is_base_of<std::bidirectional_iterator_tag, typename boost::range_category<Range2T>::type>
	    >
	>::type>
	: public boost::algorithm::detail::finder_typedefs<Range1T, Range2T, ComparatorT, std::allocator<std::size_t> >
	{
	public:
        last_finder_impl_t (substring_range_type const &substr, ComparatorT comparator=ComparatorT())
            : finder(comparator)
	    { finder.set_substring(substring_reverse_iterator_type(boost::end(substr)),
            substring_reverse_iterator_type(boost::begin(substr))
            ); }
	    string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
	    {
	        finder.set_string(string_reverse_iterator_type(string_end),
                string_reverse_iterator_type(string_start));
	        string_reverse_range_type &ret = finder.find_first();
            //no match
            if (boost::begin(ret) == boost::end(finder.get_string_range()))
                return boost::make_iterator_range(string_end, string_end);
	        return boost::make_iterator_range(boost::end(ret).base(), boost::begin(ret).base());
	    }
	private:
        typedef typename boost::algorithm::simplified_finder_t<
            substring_reverse_range_type, string_reverse_range_type, AlgorithmT, ComparatorT> reverse_finder_type;
        reverse_finder_type finder;
	};
	
	//Implementation of last_finder_t when at least one range is not bidirectional
	template <class Range1T, class Range2T, class AlgorithmT, class ComparatorT>
	class last_finder_impl_t<Range1T, Range2T, AlgorithmT, ComparatorT, 
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
	    last_finder_impl_t (substring_range_type const &substr, ComparatorT comparator=ComparatorT())
	        : finder(comparator)
	    { finder.set_substring(boost::begin(substr), boost::end(substr)); }
	    string_range_type operator()(string_iterator_type const &string_start, string_iterator_type const &string_end)
	    {
	        finder.set_string(string_start, string_end);
	        string_iterator_type prev, crt = make_iterator_range(string_start, string_end);
	        for (;;)
	        {
	            prev = crt;
	            crt = finder.find_next();
	            if (boost::begin(crt) == string_end) break;
	        }
	        return prev;
	    }
	private:
	    typedef boost::algorithm::simplified_finder_t<Range1T, Range2T, AlgorithmT, ComparatorT> internal_finder_type;
	    internal_finder_type finder;
	};

} } }
#endif // BOOST_ALGORITHM_DETAIL_LAST_FINDER_IMPL_HPP
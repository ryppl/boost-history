#ifndef BOOST_ALGORITHM_NAIVE_SEARCH_HPP
#define BOOST_ALGORITHM_NAIVE_SEARCH_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/call_traits.hpp>

namespace boost
{
	namespace algorithm
	{

		struct naive_search
		{

			template <class ForwardIterator1T,class ForwardIterator2T,class Comparator,class Allocator = std::allocator<char> >
            struct algorithm : boost::noncopyable
			{
			public:
				typedef ForwardIterator1T substring_iterator_type;
				typedef ForwardIterator2T string_iterator_type;
				typedef Comparator comparator_type;
				typedef Allocator allocator_type;
				typedef typename Allocator::value_type allocator_value_type;
			protected:
                //construct the algorithm given iterator ranges for the substring and the string
				algorithm (typename boost::call_traits<Comparator>::param_type comparator,
					typename boost::call_traits<Allocator>::param_type allocator,
					typename boost::call_traits<typename boost::iterator_range<substring_iterator_type> >::param_type substring,
					typename boost::call_traits<typename boost::iterator_range<string_iterator_type> >::param_type string)
					: comparator_(comparator), allocator_(allocator), substring_(substring), string_(string) { }


				boost::iterator_range<string_iterator_type> find(string_iterator_type start)
				{
					for (;
						start != boost::end(string_); ++start)
					{
						string_iterator_type str_iter(start);
						substring_iterator_type substr_iter;
						for (substr_iter = boost::begin(substring_);
							substr_iter != boost::end(substring_) && str_iter != boost::end(string_); ++substr_iter, ++str_iter)
						{
							if (!comparator_(*str_iter, *substr_iter)) break;
						}
						if (substr_iter == boost::end(substring_))
							return boost::iterator_range<string_iterator_type>(start, str_iter);
					}
					return boost::iterator_range<string_iterator_type>(boost::end(string_),boost::end(string_));
				}
                //No precomputation to be done on the substring
                inline void on_substring_change()
                {
                }
                //No precomputation to be done on the string
                inline void on_string_change()
                {
                }
			private:
				typename boost::call_traits<comparator_type>::param_type comparator_;
				typename boost::call_traits<allocator_type>::param_type allocator_;
				typename boost::call_traits<typename boost::iterator_range<substring_iterator_type> >::param_type substring_;
				typename boost::call_traits<typename boost::iterator_range<string_iterator_type> >::param_type string_;
			};

		};
		
	}
	
	using algorithm::naive_search;

}

#endif
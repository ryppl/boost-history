#ifndef BOOST_ALGORITHM_BOYER_MOORE_HPP
#define BOOST_ALGORITHM_BOYER_MOORE_HPP

#include <iterator>
#include <allocators>
#include <vector>

namespace boost { namespace algorithm {
    struct boyer_moore
    {
        typedef std::allocator<std::size_t> default_allocator_type;

        template <class Finder, class ForwardIterator1T,class ForwardIterator2T,
            class Comparator,class Allocator>
        class algorithm
        {
        public:
            typedef ForwardIterator1T substring_iterator_type;
	        typedef ForwardIterator2T string_iterator_type;
            typedef typename std::iterator_traits<substring_iterator_type>::value_type substring_char_type;
            typedef typename std::iterator_traits<string_iterator_type>::value_type string_char_type;
            typedef typename boost::iterator_range<substring_iterator_type> substring_range_type;
            typedef typename boost::iterator_range<string_iterator_type> string_range_type;
        protected:
            string_range_type find(string_iterator_type start)
            {
                return find(start, std::iterator_traits<ForwardIterator2T>::iterator_category());
            }

            //Compute the two tables
            void on_substring_change()
            {
                on_substring_change(std::iterator_traits<ForwardIterator1T>::iterator_category());
            }
            //No precomputation to be done on the string
            inline void on_string_change()
            {

            }
        private:
            void on_substring_change(std::random_access_iterator_tag)
            {
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

            }

            string_range_type find(string_iterator_type start, std::random_access_iterator_tag)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();
            }
        };
    };
} }

#endif
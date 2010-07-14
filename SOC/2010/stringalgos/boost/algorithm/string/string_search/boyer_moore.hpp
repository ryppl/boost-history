#ifndef BOOST_ALGORITHM_BOYER_MOORE_HPP
#define BOOST_ALGORITHM_BOYER_MOORE_HPP

#include <iterator>
#include <allocators>
#include <utility>
#include <vector>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/distance.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <map>

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
            typedef typename
                std::iterator_traits<substring_iterator_type>::value_type substring_char_type;
            typedef typename std::iterator_traits<string_iterator_type>::value_type string_char_type;
            typedef typename boost::iterator_range<substring_iterator_type> substring_range_type;
            typedef typename boost::iterator_range<string_iterator_type> string_range_type;
            typedef Comparator comparator_type;
            typedef Allocator allocator_type;
        protected:
            algorithm () {
                BOOST_STATIC_ASSERT((boost::is_same<substring_char_type,string_char_type>::value));
            }

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
            { }
        private:
            struct compute_first_table_functor {
                void operator()(typename boost::call_traits<substring_char_type>::param_type c)
                {
                    //alg_.table1.insert( std::make_pair(c, alg_.substr_size_ - 1 - idx_) );
                    if (idx_ != 0) alg_.table1.insert(std::make_pair(c, idx_));

                    ++idx_;
                }
                compute_first_table_functor (algorithm &alg) : idx_(0), alg_(alg)
                { alg_.table1.clear(); }
            private:
                std::size_t idx_;
                algorithm &alg_;
            };

            struct compute_second_table_functor {
                void operator()(typename boost::call_traits<substring_char_type>::param_type c)
                {

                }
                compute_second_table_functor (algorithm &alg) : idx_(0), alg_(alg)
                { /*alg_.table2.clear();*/ }
            private:
                std::size_t idx_;
                algorithm &alg_;
            };

            //precomputation on pattern=bidirectional range
            void on_substring_change(std::bidirectional_iterator_tag)
            {
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();
                
                substr_size_ = boost::distance(substr);

                //Compute the first table
                boost::for_each(substr | boost::adaptors::reversed,
                    compute_first_table_functor(*this));
                
                //Compute the second table
                boost::for_each(substr | boost::adaptors::reversed,
                    compute_second_table_functor(*this));
            }

            //finding in text=random access range
            string_range_type find(string_iterator_type start, std::random_access_iterator_tag)
            {
                string_range_type const &str = static_cast<Finder*>(this)->get_string_range();
                substring_range_type const &substr = static_cast<Finder*>(this)->get_substring_range();
                comparator_type const &comp = static_cast<Finder*>(this)->get_comparator();

                std::size_t str_idx, substr_idx, str_size;

                if (substr_size_ == 0)
                    return string_range_type(start, start);

                str_size = boost::end(str) - start;
                for (str_idx = substr_size_ - 1, substr_idx = substr_size_ - 1; str_idx < str_size;)
                {
                    if (comp(start[str_idx],
                        boost::begin(substr)[substr_idx]))
                    {
                        if (substr_idx == 0)
                        {
                            return string_range_type(start+str_idx,start+str_idx+substr_size_);
                        }
                        else { assert(str_idx > 0); --substr_idx; --str_idx; }
                    }
                    else
                    {
                        table1_type::const_iterator iter = table1.find(start[str_idx]);
                        if (iter == table1.end())
                        {
                            str_idx += substr_size_ - substr_idx;
                            substr_idx = substr_size_ - 1;
                        }
                        else if (iter->second > substr_idx)
                        {
                            str_idx += iter->second;
                            substr_idx = substr_size_ - 1;
                            //str_idx += iter->second - substr_idx + substr_size_ - 1 - substr_idx;
                            //substr_idx = substr_size_ - 1;
                            //substr_idx = substr_size_ - 1 - iter->second;
                            //str_idx += substr_size_ - 1 - substr_idx;
                            //substr_idx = substr_size_ - 1;
                        }
                        else
                        {
                            assert(substr_size_ >= substr_idx);
                            str_idx += substr_size_-substr_idx;
                            substr_idx = substr_size_ - 1;
                        }
                    }
                }
                return string_range_type(boost::end(str), boost::end(str));
            }

            //!\todo Get a better data structure here (hash table?)
            //!\todo Find a better way for custom allocators here
            typedef typename std::map<substring_char_type, std::size_t,
                std::less<substring_char_type>,
                typename std::allocator<std::pair<const substring_char_type, std::size_t> >
            > table1_type;

            table1_type table1;

            std::size_t substr_size_;

        };
    };
} }

namespace boost { using boost::algorithm::boyer_moore; }

#endif
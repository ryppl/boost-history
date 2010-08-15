#ifndef BOOST_ALGORITHM_RABIN_KARP_DETAIL_HPP
#define BOOST_ALGORITHM_RABIN_KARP_DETAIL_HPP

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <boost/mpl/void.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/range/iterator.hpp>
#include <boost/range/category.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/call_traits.hpp>

#include <iterator>
#include <cassert>
#include <limits>
#include <memory>

#include <boost/algorithm/string/config.hpp>
#include <boost/algorithm/string/compare.hpp>

//todo add proper overflow assertions here. also try to find if these aren't already in boost
#define BOOST_ALGORITHM_DETAIL_ASSERTED_ADD(a, b, T) \
    assert((boost::uintmax_t)(a)+(boost::uintmax_t)(b) <= std::numeric_limits<T>::max()),((a)+(b))
#define BOOST_ALGORITHM_DETAIL_ASSERTED_SUBSTRACT(a, b, T) \
    assert((boost::uintmax_t)(a)-(boost::uintmax_t)(b) <= std::numeric_limits<T>::max()),((a)-(b))
#define BOOST_ALGORITHM_DETAIL_ASSERTED_MULTIPLY(a, b, T) \
    assert((boost::uintmax_t)(a)*(boost::uintmax_t)(b) <= std::numeric_limits<T>::max()),((a)*(b))


namespace boost { namespace algorithm { namespace detail {

    /*template <class HashType, HashType Modulo, HashType Base> struct rabin_karp_multiplicative_inverse;

    template <class HashType> struct rabin_karp_multiplicative_inverse<unsigned int,1,2>
    {
        static const unsigned int value = 3;
    };*/

    //Note: for 32bit integers, modulo, base and exp must all be at most 16bit.
    template <class IntT>
    inline IntT logarithmic_exponentiation (IntT base, IntT exp, IntT modulo)
    {
        IntT ret = static_cast<IntT>(1);
        for (; exp; exp >>= 1)
        {
            if (exp&1) ret = (ret*base) % modulo;
            base = (base*base) % modulo;
        }
        return ret;
    }

    template <class Range1CharT, class Range2CharT, class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo, class StringIteratorCategory, class Enable = void>
    class rabin_karp_algorithm;

    // Implementation of Rabin Karp for text supporting Input Iterators
    template <class Range1CharT, class Range2CharT, class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo, class StringIteratorCategory>
    class rabin_karp_algorithm<Range1CharT, Range2CharT, HashType,
        FirstBase, FirstModulo, SecondBase, SecondModulo, StringIteratorCategory,
        typename boost::enable_if<
        typename boost::mpl::and_<
        typename boost::is_base_of<std::input_iterator_tag, StringIteratorCategory>,
        typename boost::mpl::not_<typename boost::is_base_of<std::forward_iterator_tag, StringIteratorCategory> >
        >
        >::type
    >
    ;

    // Implementation of Rabin Karp for text supporting Forward Iterators
    template <class Range1CharT, class Range2CharT, class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo, class StringIteratorCategory>
    class rabin_karp_algorithm<Range1CharT, Range2CharT, HashType,
        FirstBase, FirstModulo, SecondBase, SecondModulo, StringIteratorCategory,
        typename boost::enable_if<
        typename boost::mpl::and_<
        typename boost::is_base_of<std::forward_iterator_tag, StringIteratorCategory>,
        typename boost::mpl::not_<typename boost::is_base_of<std::random_access_iterator_tag, StringIteratorCategory> >
        >
        >::type
    >
    ;

    //Implementation of Rabin Karp for text supporting Random Access Iterators
    template <class Range1CharT, class Range2CharT, class HashType,
        HashType FirstBase, HashType FirstModulo,
        HashType SecondBase, HashType SecondModulo, class StringIteratorCategory>
    class rabin_karp_algorithm<Range1CharT, Range2CharT, HashType,
        FirstBase, FirstModulo, SecondBase, SecondModulo, StringIteratorCategory,
        typename boost::enable_if<
            typename boost::is_base_of<
                std::random_access_iterator_tag,
                StringIteratorCategory
            >
        >::type
    >
    {
    private:
        typedef Range1CharT substring_char_type;
        typedef Range2CharT string_char_type;
    protected:

        rabin_karp_algorithm() :
                 first_substring_hash_(0), second_substring_hash_(0),
                 first_string_hash_current_(0), second_string_hash_current_(0),
                 first_string_hash_beginning_(0), second_string_hash_beginning_(0),
                 first_inverse_(0), second_inverse_(0), string_size_(0), substring_size_(0), string_computed_upto_(0)
             { }

        //\todo this the right name? the right way to do it?
        template <class T>
        BOOST_STRING_FORCE_INLINE HashType integer_promotion(T i)
        { return static_cast<HashType>(static_cast<typename boost::make_unsigned<T>::type>(i)); }

    public:

        template <class Range1T, class Range2T>
        inline void on_substring_change(
            typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
        {
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            substring_range_type const &substr = ranges.substr;

            HashType first = static_cast<HashType>(0), second = static_cast<HashType>(0);
            
            std::size_t old_substring_size = substring_size_;

            substring_size_ = 0;
            for (substring_iterator_type it = boost::begin(substr);
                it != boost::end(substr); ++it, ++substring_size_)
            {
                first = (first * FirstBase + integer_promotion(*it) ) % FirstModulo;
                second = (second * SecondBase + integer_promotion(*it) ) % SecondModulo;
            }
            // \todo Optimize here? (by precomputing all powers FirstBase^(2^k) and SecondBase^(2^k))
            //compute (-(FirstBase)^substring_size_) % FirstModulo

            first_inverse_ = FirstModulo - ::boost::algorithm::detail::logarithmic_exponentiation(
                FirstBase, static_cast<HashType>(substring_size_), FirstModulo);
            //compute (-(SecondBase)^substring_size_) % SecondModulo
            second_inverse_ = SecondModulo - ::boost::algorithm::detail::logarithmic_exponentiation(
                SecondBase, static_cast<HashType>(substring_size_), SecondModulo);
            first_substring_hash_ = first;
            second_substring_hash_ = second;
            
            if (substring_size_ != old_substring_size)
                on_string_change(ranges);
        }

        template <class Range1T, class Range2T>
        inline void on_string_change(
            typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
        {
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            string_range_type const &str = ranges.str;

            HashType first = static_cast<HashType>(0), second = static_cast<HashType>(0);
            std::size_t computed = 0;
            for (string_iterator_type it = boost::begin(str);
                it != boost::end(str) && computed < substring_size_;
                ++it, ++computed)
            {
                first = (first * FirstBase + integer_promotion(*it) ) % FirstModulo;
                second = (second * SecondBase + integer_promotion(*it) ) % SecondModulo;
            }
            //note: use the loop above to calculate part of the string size and then only finish the rest
            //(when having a forward iterator)
            //todo figure out how to avoid necessity of string_size_ when you have an input iterator only
            string_size_ = boost::end(str) - boost::begin(str);

            first_string_hash_beginning_ = first;
            second_string_hash_beginning_ = second;
            string_computed_upto_ = computed;
            first_string_hash_current_ = first;
            second_string_hash_current_ = second;
        }

        template <class Range1T, class Range2T>
        inline typename boost::iterator_range<typename boost::range_iterator<Range2T>::type>
            find(typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
        {
            BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

            string_range_type const &str = ranges.str;
            substring_range_type const &substr = ranges.substr;
            string_iterator_type start = ranges.offset;

            //substring bigger than string
            if (string_size_ < substring_size_)
                return boost::iterator_range<string_iterator_type>(boost::end(str), boost::end(str));

            std::size_t offset = start - boost::begin(str);

            //not enough space left in the string to match the full substring
            if (offset + substring_size_ > string_size_)
                return boost::iterator_range<string_iterator_type>(boost::end(str), boost::end(str));

            //the string hashes have been computed past the current offset, reset them to the beginning
            if (string_computed_upto_ - substring_size_ > offset)
            {
                string_computed_upto_ = substring_size_;
                first_string_hash_current_ = first_string_hash_beginning_;
                second_string_hash_current_ = second_string_hash_beginning_;
            }
            //roll the hash until we reach the current offset
            while (offset > string_computed_upto_ - substring_size_)
                roll_string_hash(str);

            //a match found right at the current offset.
            if (equal())
                return boost::iterator_range<string_iterator_type>(start, start+substring_size_);
            //rolling the hash until we find a match
            while (string_computed_upto_ != string_size_)
            {
                roll_string_hash(str);
                //match found
                if (equal())
                    return boost::iterator_range<string_iterator_type>(
                        boost::begin(str) + (string_computed_upto_ - substring_size_),
                        boost::begin(str) + string_computed_upto_
                        );
            }
            //no match found
            return boost::iterator_range<string_iterator_type>(
                boost::end(str), boost::end(str)
            );
        }
        
    private:
        bool equal () const
        {
            return first_substring_hash_ == first_string_hash_current_ &&
                second_substring_hash_ == second_string_hash_current_;
        }
        /*template <class IteratorT>
        inline boost::tuple<HashType,HashType> compute_hashes (IteratorT const &start, IteratorT const &end)
        {
            HashType first = static_cast<HashType>(0), second = static_cast<HashType>(0);
            for (IteratorT it = start; it != end; ++it)
            {
                first = (first*FirstBase + *it) % FirstModulo;
                second= (second*SecondBase + *it) % SecondModulo;
            }
            return boost::make_tuple(first, second);
        }*/

        //todo compatible force inline? __attribute__((force_inline)) in GCC
        //inline void roll_string_hash()
        template <class StrT>
        BOOST_STRING_FORCE_INLINE void roll_string_hash(StrT const &str)
        {

            HashType remove = static_cast<HashType>(
                static_cast<typename boost::make_unsigned<string_char_type>::type>(
                    boost::begin(str)[string_computed_upto_-substring_size_]
            ));
            HashType add    = static_cast<HashType>(
                static_cast<typename boost::make_unsigned<string_char_type>::type>(
                    boost::begin(str)[string_computed_upto_]
            ));
            
            /*first_string_hash_current_ = mod(
                    mod(FirstBase * first_string_hash_current_ + add,FirstModulo) +
                    mod(first_inverse_ * remove,FirstModulo),
                FirstModulo);*/

            //In order to not overflow: (M1-1)*b1 + X + (M1-1)*X <= MAX(HashType)
            first_string_hash_current_ = mod(
                    FirstBase * first_string_hash_current_ + add +
                    first_inverse_ * remove,
                FirstModulo);

            /*second_string_hash_current_ = mod(
                mod(SecondBase * second_string_hash_current_ + add,SecondModulo) +
                mod(second_inverse_ * remove,SecondModulo),
                SecondModulo);
                */
            //In order to not overflow: (M2-1)*b2 + X + (M2-1)*X <= MAX(HashType)
            second_string_hash_current_ = mod(
                SecondBase * second_string_hash_current_ + add +
                second_inverse_ * remove,
                SecondModulo);

            ++string_computed_upto_;
        }

        inline HashType mod (HashType a, HashType b)
        {
            return a%b;
        }

        HashType first_substring_hash_, second_substring_hash_;
        HashType first_string_hash_current_, second_string_hash_current_;
        HashType first_string_hash_beginning_, second_string_hash_beginning_;
        HashType first_inverse_, second_inverse_;
        std::size_t string_size_, substring_size_, string_computed_upto_;
    };

} } }
#endif

#ifndef BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP
#define BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP

#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <boost/range/as_literal.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/call_traits.hpp>

#include <memory>

namespace boost { namespace algorithm {

    //! A generic finder type
    /*!
        Allows simple use of various string search algorithms.
        It has a similar interface to that of \ref boost::algorithm::finder_t, except the constructors and setters
        of this type can only take pointers to strings (thus avoiding any data copying).
        \tparam Range1T A range representing the type of the substring (pattern)
        \tparam Range2T A range representing the type of the string (text)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm.
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
        \tparam AdditionalBehaviorT Optional template parameter used to extend the functionality of
            the finder.
    */
    template <class Range1T, class Range2T, class AlgorithmT,
        class ComparatorT = ::boost::algorithm::is_equal,
        class AllocatorT = std::allocator<std::size_t>
    >
    class simplified_finder_t :
        public AlgorithmT::template algorithm<
            simplified_finder_t<Range1T, Range2T, AlgorithmT, ComparatorT, AllocatorT>,
            Range1T, Range2T, ComparatorT,AllocatorT>
    {
        // TODO Add concept assertions here.
    private:
        typedef typename AlgorithmT::template algorithm<simplified_finder_t, Range1T,
            Range2T, ComparatorT, AllocatorT> algorithm_type;
    public:
        //! The type of the pattern (substring)
        typedef typename algorithm_type::substring_type substring_type;
        //! The type of the text (string)
        typedef typename algorithm_type::string_type string_type;
        //! The type of the comparator
        typedef typename algorithm_type::comparator_type comparator_type;
        //! The type of the allocator
        typedef typename algorithm_type::allocator_type allocator_type;
        //! The type of the substring's iterator
        typedef typename algorithm_type::substring_iterator_type substring_iterator_type;
        //! The type of the text's iterator
        typedef typename algorithm_type::string_iterator_type string_iterator_type;
        //! The character type of the substring
        typedef typename algorithm_type::substring_char_type substring_char_type;
        //! The character type of the text
        typedef typename algorithm_type::string_char_type string_char_type;
        //! The range type of the substring (pattern)
        typedef typename algorithm_type::substring_range_type substring_range_type;
        //! The range type of the text
        typedef typename algorithm_type::string_range_type string_range_type;
        //! A type capable of holding the difference between two iterators of the text
        typedef typename algorithm_type::string_difference_type string_difference_type;

        //! Constructs a finder.
        /*!
            \param comparator ComparatorT instance used to compare individual characters
            \param allocator AllocatorT instance used to allocate memory
                for storing precomputed data if necessary
        */
        explicit simplified_finder_t(ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT())
            : substring_range_(), string_range_(), substring_has_changed_(false),
            string_has_changed_(false), comparator_(comparator), allocator_(allocator),
            start_offset_()
        { }
        /*!
            Constructs a finder given a pattern and a text
            \param substr A pointer to a range (or a character array) representing the sought string
            \param str A pointer to a range (or a character array) representing the text
                in which to search
            \param comparator ComparatorT instance used to compare individual characters
            \param allocator AllocatorT instance used to allocate memory
                for storing precomputed data if necessary
            \warning The ranges that substr and str point to must not change during the lifetime of the finder,
                otherwise one would have to re-set the ranges:
                <example> <code>
                string substr("a"), str("b");
                simplified_finder_t<string,string,..> finder(&substr, &str);
                substr = "b"; // substr has changed, invalidating the internally stored range
                finder.set_substring(&substr); // re-set the substring so the finder now uses the new, valid range
                </code> </example>
        */
        simplified_finder_t(Range1T const *const substr, Range2T *str,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT())
            : substring_range_(boost::as_literal(*substr)),
            string_range_(boost::as_literal(*str)),
            comparator_(comparator), allocator_(allocator),
            substring_has_changed_(true), string_has_changed_(true),
            algorithm_type()
        { }


        simplified_finder_t (const simplified_finder_t &other)
            : substring_range_(other.substring_range_), string_range_(other.string_range_),
            substring_has_changed_(other.substring_has_changed_), string_has_changed_(other.string_has_changed_),
            comparator_(other.comparator_), allocator_(other.allocator_), start_offset_(other.start_offset_),
            algorithm_type(other)
        {
        }
        simplified_finder_t &operator=(const simplified_finder_t &rhs)
        {
            substring_range_ = rhs.substring_range_;
            string_range_ = rhs.string_range_;
            substring_has_changed_ = rhs.substring_has_changed_;
            string_has_changed_ = rhs.string_has_changed_;
            comparator_ = rhs.comparator_;
            allocator_ = rhs.allocator_;
            start_offset_ = rhs.start_offset_;
            return *this;
        }

        //! Change the pattern (substring) to be searched.
        /*!
            \param substr A pointer to a range (or a character array) representing the sought string
        */
        void set_substring (substring_type const *const substr)
        { substring_range_ = boost::as_literal(*substr); substring_has_changed_ = true; }

        void set_substring (substring_iterator_type const &substring_begin, substring_iterator_type const &substring_end)
        {
            substring_range_ = boost::make_iterator_range(substring_begin, substring_end);
            substring_has_changed_ = true;
        }

        //! Change the text in which to search.
        /*!
            \param str A pointer to a range (or a character array) representing the text
                in which to search
        */
        void set_string (string_type *const str)
        { string_range_ = boost::as_literal(*str); string_has_changed_ = true; }

        void set_string (string_iterator_type const &string_begin, string_iterator_type const &string_end)
        {
            string_range_ = boost::make_iterator_range(string_begin, string_end);
            string_has_changed_ = true;
        }

        //! Reset the internal offset to the beginning of the text.
        void find_reset ()
        { start_offset_ = boost::begin(string_range_); }

        //! Finds the first occurrence of the pattern in the text (substring in the string)
        /*!
            Equivalent to:
            \code
            find_reset(); return find_next();
            \endcode
        */
        string_range_type find_first ()
        {
            find_reset();
            return find_next();
        }

        //! Perform a search
        /*!
            \deprecated Implemented to preserve compatibility
                with the previous Finder concept
            \param string_start An iterator at the start of the text in which to perform the search.
            \param string_end An iterator one past the end of the text in which to perform the search.
            \return An iterator range indicating the place where the match has occurred.
            \warning This call changes the text used by the finder, but <b>DOES NOT</b> make a
                copy of the text. That is, the finder will still use the text given to this call in future searches.
                Please note that once the passed iterators become invalid, trying to perform searches
                would cause undefined behavior. Make sure you call set_string() if you want to use this finder
                after the passed iterators become invalid.
            \note This is equivalent to:
                <code>
                finder.set_string(string_start, string_end);
                return finder.find_first();
                </code>
            */
        string_range_type operator()(string_iterator_type &string_start,
            string_iterator_type &string_end)
        {
            set_string(string_start, string_end);
            return find_first();
        }


        // TODO: Assert in case this was called after an empty ctor
        
        //! Finds the next occurrence of the pattern in the text (starts searching at the current internal offset)
        //! \sa \ref find_next(), \ref find_first()
        string_range_type find_next()
        {
            apply_changes();
            if (start_offset_ == boost::end(string_range_))
                return string_range_type(start_offset_, start_offset_);
            string_range_type ret =
                algorithm_type::find(start_offset_);
            if (boost::begin(ret) == boost::end(string_range_))
            {
                start_offset_ = boost::end(string_range_);
                return ret;
            }
            else
            {
                start_offset_ = boost::begin(ret);
                ++start_offset_;
                return ret;
            }
        }

        //! Get an iterator range of the current pattern (substring)
        /*!
            \return Iterator range of the current pattern
            \warning If you modify the contents in this range, you must manually call
                \ref use_internal_substring(), so that the finder can perform any required precomputation on the pattern
        */
        substring_range_type get_substring_range() const { return substring_range_; }

        //! Get an iterator range of the current text
        /*!
            \return Range of the current text
            \warning If you modify the contents in this range, you must manually call
                \ref use_internal_string(), so that the finder can perform any required precomputation on the text
        */
        string_range_type get_string_range() const { return string_range_; }
            
        //! Gets a reference to the current comparator
        typename boost::call_traits<comparator_type>::const_reference get_comparator() const
        { return comparator_;  }
       
        //! Gets a reference to the current allocator
        //! \return A reference to the current allocator
        typename boost::call_traits<allocator_type>::reference get_allocator()
        { return allocator_; }
            
        //! Gets a reference to the current allocator
        //! \return A reference to the current allocator
        typename boost::call_traits<allocator_type>::const_reference get_allocator() const
        { return allocator_; }

    private:
        inline void apply_changes()
        {
            if (substring_has_changed_ || string_has_changed_) {
                find_reset();
                if (substring_has_changed_) {
                    on_substring_change();
                    substring_has_changed_ = false;
                }
                if (string_has_changed_) {
                    on_string_change();
                    string_has_changed_ = false;
                }
            }
        }


    protected:
        substring_range_type substring_range_;
        string_range_type string_range_;
        bool substring_has_changed_, string_has_changed_;

        comparator_type comparator_;
        allocator_type allocator_;

        string_iterator_type start_offset_;
    };

} }

namespace boost
{
    using algorithm::simplified_finder_t;
}

#endif // BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP
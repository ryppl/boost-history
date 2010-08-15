//  Boost string_algo library simplified_finder.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP
#define BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP

#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>
#include <boost/algorithm/string/finder/default_search_algorithm.hpp>

#include <boost/range/as_literal.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/call_traits.hpp>

#include <memory>

/*! \file
    Defines \ref simplified_finder_t, a finder type uesd for finding occurrences of a pattern in a string.
    This finder is a more limited version of \ref finder_t, because it only allows passing pointers to ranges,
    which means no internal copies of the strings are made (\see finder_t for details). While this offers additional
    efficiency, it requires the caller to guarantee that the lifetime of a passed string is at least as long
    as the lifetime of the finder.
*/

namespace boost { namespace algorithm {

    //! A generic finder type
    /*!
        Allows simple use of various string search algorithms.
        It has a similar interface to that of \ref boost::algorithm::finder_t, except the constructors and setters
        of this type can only take pointers to strings (thus avoiding any data copying).
        \tparam Range1T A range representing the type of the substring (pattern)
        \tparam Range2T A range representing the type of the string (text)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm;
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
    */
    template <class Range1T, class Range2T, class AlgorithmT = boost::algorithm::default_finder_algorithm,
        class ComparatorT = ::boost::algorithm::is_equal,
        class AllocatorT = std::allocator<std::size_t>
    >
    class simplified_finder_t
    {
        // TODO Add concept assertions here.
    public:
        BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS2(ComparatorT, AllocatorT);
    private:
        typedef BOOST_STRING_TYPENAME AlgorithmT::template algorithm<substring_char_type, string_char_type,
            comparator_type, allocator_type> algorithm_type;
    public:
        //! Constructs the finder.
        /*!
            \param comparator ComparatorT instance used to compare individual characters
            \param allocator AllocatorT instance used to allocate memory
                for storing precomputed data if necessary
        */
        explicit simplified_finder_t(ComparatorT const &comparator = ComparatorT(),
            AllocatorT const &allocator = AllocatorT())
            : ranges_(), algorithm_(comparator, allocator),
            substring_has_changed_(false), string_has_changed_(false)            
        { }
        /*!
            Constructs the finder given a pattern and a text
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
        simplified_finder_t(Range1T const *const substr, Range2T *const str,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT())
            : ranges_(boost::as_literal(*substr), boost::as_literal(*str)),
            algorithm_(comparator, allocator),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! Copies the finder
        simplified_finder_t (const simplified_finder_t &other)
            : ranges_(other.ranges_), algorithm_(other.algorithm_),
            substring_has_changed_(other.substring_has_changed_),
            string_has_changed_(other.string_has_changed_)
        {
        }

        simplified_finder_t &operator=(const simplified_finder_t &rhs)
        {
            ranges_ = rhs.ranges_;
            algorithm_ = rhs.algorithm_;
            substring_has_changed_ = rhs.substring_has_changed_;
            string_has_changed_ = rhs.string_has_changed_;
            return *this;
        }

        //! Change the pattern (substring) to be searched.
        /*!
            \param substr A pointer to a range (or a character array) representing the sought string
        */
        void set_substring (substring_type const *const substr)
        { ranges_.substr = boost::as_literal(*substr); substring_has_changed_ = true; }

        //! \overload
        void set_substring (substring_iterator_type const &substring_begin, substring_iterator_type const &substring_end)
        {
            ranges_.substr = boost::make_iterator_range(substring_begin, substring_end);
            substring_has_changed_ = true;
        }

        //! Change the text in which to search.
        /*!
            \param str A pointer to a range (or a character array) representing the text
                in which to search
        */
        void set_string (string_type *const str)
        { ranges_.str = boost::as_literal(*str); string_has_changed_ = true; }

        //!\overload
        void set_string (string_iterator_type const &string_begin, string_iterator_type const &string_end)
        {
            ranges_.str = boost::make_iterator_range(string_begin, string_end);
            string_has_changed_ = true;
        }

        //! Reset the internal offset to the beginning of the text.
        void find_reset ()
        { ranges_.offset = boost::begin(ranges_.str); }

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
            \param string_begin An iterator at the start of the text in which to perform the search.
            \param string_end An iterator one past the end of the text in which to perform the search.
            \return An iterator range indicating the place where the match has occurred.
            \warning This call changes the text used by the finder, but <b>DOES NOT</b> make a
                copy of the text. That is, the finder will still use the text given to this call in future searches.
                Please note that once the passed iterators become invalid, trying to perform searches
                would cause undefined behavior. Make sure you call set_string() if you want to use this finder
                after the passed iterators become invalid.
            \note This is equivalent to:
                <code>
                finder.set_string(string_begin, string_end);
                return finder.find_first();
                </code>
            */
        string_range_type operator()(string_iterator_type const &string_begin,
            string_iterator_type const &string_end)
        {
            set_string(string_begin, string_end);
            return find_first();
        }


        // TODO: Assert in case this was called after an empty ctor
        
        //! Finds the next occurrence of the pattern in the text (starts searching at the current internal offset)
        //! \sa \ref find_next(), \ref find_first()
        string_range_type find_next()
        {
            apply_changes();
            if (ranges_.offset == boost::end(ranges_.str))
                return string_range_type(ranges_.offset, ranges_.offset);
            string_range_type ret =
                algorithm_.find(ranges_);
            if (boost::begin(ret) == boost::end(ranges_.str))
            {
                ranges_.offset = boost::end(ranges_.str);
                return ret;
            }
            else
            {
                ranges_.offset = boost::begin(ret);
                ++ranges_.offset;
                return ret;
            }
        }

        //! Get an iterator range of the current pattern (substring)
        /*!
            \return Iterator range of the current pattern
            \warning If you modify the contents in this range externally, you must manually call
                <code>set_substring(&substr);</code> afterwards
        */
        substring_range_type get_substring_range() const { return ranges_.substr; }

        //! Get an iterator range of the current text
        /*!
            \return Range of the current text
            \warning If you modify the contents in this range externally, you must manually call
                <code>set_string(&str);</code> afterwards
        */
        string_range_type get_string_range() const { return ranges_.str; }
           

        /*
        //! Gets a reference to the current comparator
        BOOST_STRING_TYPENAME boost::call_traits<comparator_type>::const_reference get_comparator() const
        { return comparator_;  }
       
        //! Gets a reference to the current allocator
        //! \return A reference to the current allocator
        BOOST_STRING_TYPENAME boost::call_traits<allocator_type>::reference get_allocator()
        { return allocator_; }
            
        //! Gets a reference to the current allocator
        //! \return A reference to the current allocator
        BOOST_STRING_TYPENAME boost::call_traits<allocator_type>::const_reference get_allocator() const
        { return allocator_; }
        */

        std::string get_algorithm_name() const { return algorithm_.get_algorithm_name(); }

    private:
        inline void apply_changes()
        {
            if (substring_has_changed_ || string_has_changed_) {
                find_reset();
                if (substring_has_changed_) {
                    algorithm_.on_substring_change(ranges_);
                    substring_has_changed_ = false;
                }
                if (string_has_changed_) {
                    algorithm_.on_string_change(ranges_);
                    string_has_changed_ = false;
                }
            }
        }

        boost::algorithm::detail::string_search_ranges<substring_type, string_type> ranges_;
        algorithm_type algorithm_;
        bool substring_has_changed_, string_has_changed_;
    };

} }

namespace boost
{
    using algorithm::simplified_finder_t;
}

#endif // BOOST_ALGORITHM_SIMPLIFIED_FINDER_HPP

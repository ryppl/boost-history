//  Boost string_algo library finder.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_FINDER_T_HPP
#define BOOST_ALGORITHM_FINDER_T_HPP

#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/finder/detail/is_pointer_to.hpp>

#include <memory>

#include <boost/concept_check.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/call_traits.hpp>

#include <boost/algorithm/string/config.hpp>
#include <boost/algorithm/string/finder/default_search_algorithm.hpp>
#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>

/*! \file
    Defines \ref finder_t, a finder type used for finding occurrences of a pattern in a string.
    This finder allows passing both references and pointers to strings. The overloads taking references
    will make an internal copy of the passed string, whereas the pointer overloads will rely on the lifetime
    of the passed string being longer than that of the finder.
    If you are not planning on passing temporaries as strings and you can always guarantee on the lifetime of your
    strings, you might want to use \ref simplified_finder_t instead, which only has the pointer overloads (and
    cannot possess any internal copies). Likewise, if efficiency is very important, \ref simplified_finder_t is preferred.
*/

namespace boost { namespace algorithm {


    //TODO: make optional copies of the text/pattern use the allocator too

    //TODO: copyable finder type below?

    //! A generic finder type
    /*!
        Allows simple use of various string search algorithms.
        \tparam Sequence1T A sequence representing the type of the substring (pattern)
        \tparam Sequence2T A sequence representing the type of the string (text)
        \tparam AlgorithmT The algorithm used to perform the searches
        \tparam ComparatorT Optional template parameter passed to the algorithm.
            Used for comparing individual characters for equality.
        \tparam AllocatorT Optional template parameter passed to the algorithm
            in the event that additional computation on the data has to be stored.
        \tparam AdditionalBehaviorT Optional template parameter used to extend the functionality of
            the finder.
        \note This finder holds an internal sequence of type Sequence1T and Sequence2T.
            They are used to store copies of the pattern and/or the text, when they are passed by
            reference to the finder.
        */
    template <
        class Sequence1T, class Sequence2T,
        class AlgorithmT = ::boost::algorithm::default_finder_algorithm,
        class ComparatorT = ::boost::algorithm::is_equal,
        class AllocatorT = std::allocator<std::size_t>
    >
    class finder_t
    {
        //TODO:: Maybe write a String concept?
        //TODO:: Currently, there's a SGI Sequence Concept implemented by Boost.ConceptCheck,
        //!         but std::string does not obey this concept, which means that even calling these template
        //!         parameters sequences is wrong.
        BOOST_CONCEPT_ASSERT((boost::Container<Sequence1T>));
        BOOST_CONCEPT_ASSERT((boost::Container<Sequence2T>));
    public:
        BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Sequence1T, Sequence2T);
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS2(ComparatorT, AllocatorT);
    private:
        typedef BOOST_STRING_TYPENAME AlgorithmT::template algorithm<substring_char_type, string_char_type,
            comparator_type, allocator_type> algorithm_type;
    public:
       

        //! Constructs a finder given a pattern and a text
        /*!
            \param substring Either a range (or character array)
                of the pattern to be searched, or a pointer to a sequence of type \ref substring_type.
            \param string Either a range (or character array)
                of the string to be searched, or a pointer to a sequence of type \ref string_type..
            \param comparator ComparatorT instance used to compare individual characters
            \param allocator AllocatorT instance used to allocate memory
                for storing precomputed data if necessary
            \note Both the pattern and the text can be passed either as references of ranges,
                or as pointers to sequences. In the former case, the pattern and/or text is copied
                internally. In the latter class, the pointer is used by the finder and no copy is made.
            \warning Whereas the overloads taking pointers are faster (because no strings are copied around),
                you have to guarantee that the lifetime of your pointee is at least as long as the lifetime
                of the finder. If you cannot guarantee on the lifetime, use a reference instead, which will
                force a copy.
            \note If a rvalue reference is passed as the string or substring, and your compiler supports rvalue
                references, then a move is performed as opposed to a copy.
            */
        explicit finder_t (const Sequence1T *const substring = 0, Sequence2T *const string = 0,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT()) 
            : algorithm_(comparator, allocator), substring_optional_copy_(), string_optional_copy_(),
            ranges_(substring?*substring:substring_optional_copy_, string?*string:string_optional_copy_),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        template <class Range2T>
        finder_t (const Sequence1T *const substring, Range2T &string,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT(),
            BOOST_STRING_TYPENAME boost::disable_if<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0)
            : algorithm_(comparator, allocator), substring_optional_copy_(), string_optional_copy_(), ranges_(),
            substring_has_changed_(true)
        {
            set_string(string);
            ranges_.substr = substring?*substring:substring_optional_copy_;
        }

        //! \overload
        template <class Range1T>
        explicit finder_t (const Range1T &substring, Sequence2T *const string = 0,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT(),
            BOOST_STRING_TYPENAME boost::disable_if<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0)
            : algorithm_(comparator, allocator),
            substring_optional_copy_(), string_optional_copy_(), ranges_(),
            string_has_changed_(true)
        {
            set_substring(substring);
            ranges_.str = string?boost::as_literal(*string):string_optional_copy_;
        }

        //! \overload
        template <class Range1T, class Range2T>
        finder_t (const Range1T &substring, Range2T &string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            BOOST_STRING_TYPENAME boost::disable_if<boost::mpl::or_<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T>,
                    BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> > >::type* = 0) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(), string_optional_copy_(), ranges_()
        {
            set_substring(substring);
            set_string(string);
        }

#       ifdef BOOST_HAS_RVALUE_REFS
        //! \overload
        template <class Range2T>
        explicit finder_t (
            Sequence1T const &&substring,
            Sequence2T *const string = 0,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT()) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(std::move(substring)), string_optional_copy_(),
            ranges_(substring_optional_copy_, string?*string:string_optional_copy_),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        template <class Range2T>
        explicit finder_t (
            Sequence1T const &&substring,
            Range2T &string,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT(),
            BOOST_STRING_TYPENAME boost::disable_if<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(std::move(substring)),
            string_optional_copy_(), ranges_(),
            substring_has_changed_(true)
        { set_string(string); ranges_.substr = substring_optional_copy_; }

        //! \overload
        finder_t (
            Sequence1T const &&substring,
            Sequence2T &&string,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT()) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(std::move(substring)), string_optional_copy_(std::move(string)),
            ranges_(substring_optional_copy_, string_optional_copy_),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        finder_t (const Sequence1T *const substring,
            Sequence2T &&string,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT()) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(), string_optional_copy_(std::move(string)),
            ranges_(substring?*substring:substring_optional_copy_, string_optional_copy_),
            substring_has_changed_(true), string_has_changed_(true)
        { }
            
        //! \overload
        template <class Range1T>
        finder_t (const Range1T &substring,
            Sequence2T &&string,
            ComparatorT const &comparator = ComparatorT(), AllocatorT const &allocator = AllocatorT(),
            BOOST_STRING_TYPENAME boost::disable_if<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0) 
            : algorithm_(comparator, allocator),
            substring_optional_copy_(), string_optional_copy_(std::move(string)),
            ranges_(), string_has_changed_(true)
        { set_substring(substring); ranges_.str = string_optional_copy_; }

#       endif


        //! Get an iterator range of the current pattern (substring)
        /*!
            \return Iterator range of the current pattern
            \warning If you modify the contents in the returned range, you must manually call
                \ref use_internal_substring() (if the used pattern was the internal pattern)
                or .set_substring(&your_pattern) otherwise
        */
        substring_range_type get_substring_range() const
        { return ranges_.substr; }

        //! Get an iterator range of the current text
        /*!
            \return Range of the current text
            \see get_substring_range()
        */
        string_range_type get_string_range() const
        { return ranges_.str; }

        /*
        //! Gets a reference to an instance of the comparator in use
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

        //! Returns a string indicating the name of the string search algorithm used for performing the searches.
        std::string get_algorithm_name() const { return algorithm_.get_algorithm_name(); }

        //! Change the pattern (substring) to be searched.
        /*!
            \param substring Either a range (or character array) corresponding to the new substring,
                or a pointer to a sequence of type \c substring_type
            \warning The same rule on the lifetime of the pointee applies. (see the constructor for details)
        */
        template <typename RangeT>
        void set_substring(RangeT const &substring,
            BOOST_STRING_TYPENAME boost::disable_if<
                BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<RangeT,Sequence1T> >::type* = 0)
        {
            boost::iterator_range<BOOST_STRING_TYPENAME boost::range_const_iterator<RangeT>::type> substring_range = 
                boost::as_literal(substring);
            //substring_optional_copy_.assign(boost::begin(substring_range), boost::end(substring_range));
            substring_optional_copy_.clear();
            substring_optional_copy_.insert(substring_optional_copy_.end(),
                boost::begin(substring_range), boost::end(substring_range));
            ranges_.substr = substring_optional_copy_;
            substring_has_changed_ = true;
        }

        //! \overload            
        void set_substring (Sequence1T const *const substring = 0)
        {
            substring_optional_copy_.clear();
            if (substring)
                ranges_.substr = *substring;
            else
                ranges_.substr = substring_optional_copy_;
            substring_has_changed_ = true;
        }

#       ifdef BOOST_HAS_RVALUE_REFS
        //!\overload
        void set_substring (
            Sequence1T const &&substring)
        {
            substring_optional_copy_ = std::move(substring);
            ranges_.substr = substring_optional_copy_;
            substring_has_changed_ = true;
        }
#       endif

        //! Change the text in which to search.
        /*!
            \param string Either a range (or character array) corresponding to the new substring,
                or a pointer to a sequence of type \ref string_type
        */
        template <typename RangeT>
        void set_string(RangeT const &string,
            BOOST_STRING_TYPENAME boost::disable_if<BOOST_STRING_TYPENAME ::boost::algorithm::detail::is_pointer_to<RangeT,Sequence2T> >::type* = 0)
        {
            boost::iterator_range<BOOST_STRING_TYPENAME boost::range_const_iterator<RangeT>::type> string_range = 
                boost::as_literal(string);
            //string_optional_copy_.assign(boost::begin(string_range), boost::end(string_range));
            string_optional_copy_.clear();
            string_optional_copy_.insert(string_optional_copy_.end(),
                boost::begin(string_range), boost::end(string_range));
            ranges_.str = string_optional_copy_;
            string_has_changed_ = true;
        }
            
        //!\overload
        void set_string (Sequence2T *const string = 0)
        {
            string_optional_copy_.clear();
            if (string)
                ranges_.str = *string;
            else
                ranges_.str = string_optional_copy_;
            string_has_changed_ = true;
        }

#       ifdef BOOST_HAS_RVALUE_REFS
        //!overload
        void set_string (Sequence2T &&string)
        {
            string_optional_copy_ = std::move(string);
            ranges_.str = string_optional_copy_;
            string_has_changed_ = true;
        }
#       endif

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
                <code>finder::string_range_type range(string_begin,string_end);
                finder.set_string(&range);
                return finder.find_first();
                </code>
                */
        string_range_type operator()(string_iterator_type &string_begin,
            string_iterator_type &string_end)
        {
            ranges_.str = boost::make_iterator_range(string_begin, string_end);
            string_has_changed_ = true;
            return find_first();
        }

        //! Forces the finder to use its internal text for searching
        /*!
            \warning This is especially useful, and \b MUST be called when the internal text has been changed externally.
            Example:
            \code
            finder.set_string("hello"); // the internal text is now "hello"
            finder.get_string_range().begin()[0] = 'x'; // the first character of the internal text is now 'x' => "xello"
            finder.use_internal_string(); // forces the finder to recompute the information on the text
            \endcode
        */
        void use_internal_string()
        {
            ranges_.str = string_optional_copy_;
            string_has_changed_ = true;
        }

        //! Forces the finder to use its internal pattern for searching
        /*!
            \warning The warning from \ref use_internal_string() applies here too.
        */
        void use_internal_substring()
        {
            ranges_.substr = substring_optional_copy_;
            substring_has_changed_ = true;
        }

        //! Gets a reference to its internal pattern sequence
        /*!
            \note Because the finder might need to make copies of the passed data (when it gets strings
                passed by reference), it stores an internal sequence of type Sequence1T,
                which is used if a copy is necessary and left empty otherwise
            \return The internal pattern sequence
        */
        BOOST_STRING_TYPENAME boost::call_traits<substring_type>::const_reference get_internal_substring() const
        {
            return substring_optional_copy_;
        }

        //!\overload
        /*!
            \warning If you change the internal pattern, you must call \ref use_internal_substring()
                so that the finder can ask the string search algorithm to recompute its information on the new pattern
        */
        BOOST_STRING_TYPENAME boost::call_traits<substring_type>::reference get_internal_substring()
        {
            return substring_optional_copy_;
        }

        //! Gets a reference to its internal text sequence.
        //! \sa get_internal_substring()
        BOOST_STRING_TYPENAME boost::call_traits<substring_type>::const_reference get_internal_string() const
        {
            return substring_optional_copy_;
        }

        //! Gets a reference to its internal text sequence.
        //! \sa get_internal_substring()
        BOOST_STRING_TYPENAME boost::call_traits<substring_type>::reference get_internal_string()
        {
            return substring_optional_copy_;
        }

        //! Performs a search using the chosen algorithm, starting from the beginning.
        /*!
            Looks for the first match of the pattern in the text.
            \return An iterator range indicating where in the text
                a match has occurred. If there is no match, an iterator range with
                begin()==end()==string.end() is returned.
            \pre The iterator ranges for the string and substring must be set.
            \post The internal find offset is set immediately after the current match starts.
            \note This is semantically equivalent to \code find_reset(); match=find_next(); \endcode
            \sa find_next()
        */
        string_range_type find_first()
        {
            find_reset();
            return find_next();
        }

        //! Performs a search using the chosen algorithm, starting from the beginning.
        /*!
            Same as \ref find_first(), except it returns an index as opposed to an iterator range
            \return An index indicating the position in the text where the match starts
            \sa find_first()
        */
        string_difference_type find_first_index()
        {
            find_reset();
            return find_next_index();
        }

        //! Performs a string using the chosen algorithm, starting from the internal find offset.
        /*!
            Looks for the next occurrence of the pattern in the text.
            \return An iterator range indicating where in the text the next occurrence of the pattern has been found, 
                or an empty range otherwise.
            \pre The iterator ranges for the string and substring must be set.
            \post The internal find offset is set immediately after the current match starts.
            \warning If the match is performed against the internal text, then an iterator range in the
                internal text will be returned. Modifying the contents of that range will modify the internal text,
                making it necessary to call \ref use_internal_string() afterwards. This call will switch to using the internal
                text (in our case, it already does that, so it will just "refresh") and will force the string search algorithm
                to recompute its information on the new text.
        */
        string_range_type find_next()
        {
            apply_changes();
            if (ranges_.offset == boost::end(ranges_.str))
                return boost::iterator_range<string_iterator_type>(
                    ranges_.offset, ranges_.offset
                );
            string_range_type ret = algorithm_.find(ranges_);
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

        //! Performs a search using the chosen algorithm, starting from the internal find offset.
        /*!
            Same as \ref find_next(), except it returns an index as opposed to an iterator range
            \return An index indicating the position in the text where the match starts
            \sa find_next()
        */
        string_difference_type find_next_index()
        {
            apply_changes();
                
            //empty substring
            if (boost::begin(ranges_.substr) == boost::end(ranges_.substr))
            {
                //empty string, empty substring
                // TODO if this gets called more times, it always returns 0
                //     i.e. the pointer is not moved. what would be a good solution for that?
                //     maybe a special dummy value for the range?
                if (boost::begin(ranges_.str) == boost::end(ranges_.str))
                    return static_cast<string_difference_type>(0);
                //empty substring, offset at the end of the range
                if (ranges_.offset == boost::end(ranges_.str))
                    return static_cast<string_difference_type>(-1);
                //empty substring, offset not at the end of range
                return std::distance(boost::begin(ranges_.str),ranges_.offset++);
            }
            else if (boost::begin(ranges_.str) == boost::end(ranges_.str))
            {
                //empty string, nonempty substring
                return static_cast<string_difference_type>(-1);
            }

            //perform an actual search
            string_range_type const &match = find_next();
            if (boost::begin(match) == boost::end(ranges_.str))
                return static_cast<string_difference_type>(-1);
            return std::distance(boost::begin(ranges_.str), boost::begin(match));
        }

        //! Reset the internal offset to the beginning of the text.
        void find_reset()
        {
            ranges_.offset = boost::begin(ranges_.str);
        }

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

        algorithm_type algorithm_;
        substring_type substring_optional_copy_;
        string_type string_optional_copy_;
        // TODO should we use the allocator here too?
        boost::algorithm::detail::string_search_ranges<Sequence1T, Sequence2T> ranges_;
        bool substring_has_changed_, string_has_changed_;
    };


} }

namespace boost
{
    using algorithm::finder_t;
}
#endif // BOOST_ALGORITHM_FINDER_T_HPP

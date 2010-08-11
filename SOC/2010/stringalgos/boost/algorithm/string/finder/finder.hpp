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

namespace boost { namespace algorithm {

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
        class AlgorithmT,
        typename ComparatorT = ::boost::algorithm::is_equal,
        class AllocatorT = std::allocator<std::size_t>
    >
    class finder_t :
        public AlgorithmT::template algorithm<
            typename finder_t<Sequence1T, Sequence2T, AlgorithmT, ComparatorT, AllocatorT>,
            Sequence1T, Sequence2T, ComparatorT, AllocatorT>
    {
        //TODO:: Maybe write a String concept?
        //TODO:: Currently, there's a SGI Sequence Concept implemented by Boost.ConceptCheck,
        //!         but std::string does not obey this concept, which means that even calling these template
        //!         parameters sequences is wrong.
        BOOST_CONCEPT_ASSERT((boost::Container<Sequence1T>));
        BOOST_CONCEPT_ASSERT((boost::Container<Sequence2T>));
    private:
        typedef typename AlgorithmT::template algorithm<finder_t, Sequence1T,
            Sequence2T, ComparatorT, AllocatorT> algorithm_type;
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
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT()) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), substring_range_(substring?*substring:substring_optional_copy_),
            string_optional_copy_(), string_range_(string?*string:string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        template <class Range2T>
        finder_t (const Sequence1T *const substring, const Range2T &string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0)
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), substring_range_(substring?*substring:substring_optional_copy_),
            string_optional_copy_(), string_range_(),
            start_offset_(),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        {
            set_string(string);
        }

        //! \overload
        template <class Range1T>
        explicit finder_t (const Range1T &substring, Sequence2T *const string = 0,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0)
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), substring_range_(),
            string_optional_copy_(), string_range_(string?boost::as_literal(*string):string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        {
            set_substring(substring);
        }

        //! \overload
        template <class Range1T, class Range2T>
        finder_t (const Range1T &substring, const Range2T &string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            typename boost::disable_if<boost::mpl::or_<typename ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T>,
                    typename ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> > >::type* = 0) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), substring_range_(),
            string_optional_copy_(), string_range_(),
            start_offset_(),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
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
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT()) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(std::move(substring)), string_optional_copy_(),
            substring_range_(substring_optional_copy_), string_range_(string?*string:string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        template <class Range2T>
        explicit finder_t (
            Sequence1T const &&substring,
            const Range2T &string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(std::move(substring)), substring_range_(substring_optional_copy_),
            string_optional_copy_(), string_range_(),
            start_offset_(),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { set_string(string); }

        //! \overload
        finder_t (
            Sequence1T const &&substring,
            Sequence2T &&string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT()) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(std::move(substring)), string_optional_copy_(std::move(string)),
            substring_range_(substring_optional_copy_), string_range_(string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { }

        //! \overload
        finder_t (const Sequence1T *const substring,
            Sequence2T &&string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT()) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), string_optional_copy_(std::move(string)),
            substring_range_(substring?*substring:substring_optional_copy_), string_range_(string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { }
            
        //! \overload
        template <class Range1T>
        finder_t (const Range1T &substring,
            Sequence2T &&string,
            ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT(),
            typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0) 
            : comparator_(comparator), allocator_(allocator),
            substring_optional_copy_(), substring_range_(),
            string_optional_copy_(std::move(string)), string_range_(string_optional_copy_),
            start_offset_(boost::begin(string_range_)),
            algorithm_type(),
            substring_has_changed_(true), string_has_changed_(true)
        { set_substring(substring); }

#       endif


        //! Get an iterator range of the current pattern (substring)
        /*!
            \return Iterator range of the current pattern
            \warning If you modify the contents in the returned range, you must manually call
                \ref use_internal_substring() (if the used pattern was the internal pattern)
                or .set_substring(&your_pattern) otherwise
        */
        typename substring_range_type get_substring_range() const
        { return substring_range_; }

        //! Get an iterator range of the current text
        /*!
            \return Range of the current text
            \see get_substring_range()
        */
        typename string_range_type get_string_range() const
        { return string_range_; }

        //! Gets a reference to an instance of the comparator in use
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

        //! Change the pattern (substring) to be searched.
        /*!
            \param substring Either a range (or character array) corresponding to the new substring,
                or a pointer to a sequence of type \c substring_type
            \warning The same rule on the lifetime of the pointee applies. (see the constructor for details)
        */
        template <typename RangeT>
        void set_substring(RangeT const &substring,
            typename boost::disable_if<
                typename ::boost::algorithm::detail::is_pointer_to<RangeT,Sequence1T> >::type* = 0)
        {
            boost::iterator_range<typename boost::range_const_iterator<RangeT>::type> substring_range = 
                boost::as_literal(substring);
            //substring_optional_copy_.assign(boost::begin(substring_range), boost::end(substring_range));
            substring_optional_copy_.clear();
            substring_optional_copy_.insert(substring_optional_copy_.end(),
                boost::begin(substring_range), boost::end(substring_range));
            substring_range_ = substring_optional_copy_;
            substring_has_changed_ = true;
        }

        //! \overload            
        void set_substring (Sequence1T const *const substring = 0)
        {
            substring_optional_copy_.clear();
            if (substring)
                substring_range_ = *substring;
            else
                substring_range_ = substring_optional_copy_;
            substring_has_changed_ = true;
        }

#       ifdef BOOST_HAS_RVALUE_REFS
        //!\overload
        void set_substring (
            Sequence1T const &&substring)
        {
            substring_optional_copy_ = std::move(substring);
            substring_range_ = substring_optional_copy_;
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
            typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<RangeT,Sequence2T> >::type* = 0)
        {
            boost::iterator_range<typename boost::range_const_iterator<RangeT>::type> string_range = 
                boost::as_literal(string);
            //string_optional_copy_.assign(boost::begin(string_range), boost::end(string_range));
            string_optional_copy_.clear();
            string_optional_copy_.insert(string_optional_copy_.end(),
                boost::begin(string_range), boost::end(string_range));
            string_range_ = string_optional_copy_;
            string_has_changed_ = true;
        }
            
        //!\overload
        void set_string (Sequence2T *const string = 0)
        {
            string_optional_copy_.clear();
            if (string)
                string_range_ = *string;
            else
                string_range_ = string_optional_copy_;
            string_has_changed_ = true;
        }

#       ifdef BOOST_HAS_RVALUE_REFS
        //!overload
        void set_string (
            Sequence2T &&string)
        {
            string_optional_copy_ = std::move(string);
            string_range_ = string_optional_copy_;
            string_has_changed_ = true;
        }
#       endif

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
                <code>finder::string_range_type range(string_start,string_end);
                finder.set_string(&range);
                return finder.find_first();
                </code>
                */
        string_range_type operator()(string_iterator_type &string_start,
            string_iterator_type &string_end)
        {
            string_range_ = boost::make_iterator_range(string_start, string_end);
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
            set_string();
        }

        //! Forces the finder to use its internal pattern for searching
        /*!
            \warning The warning from \ref use_internal_string() applies here too.
        */
        void use_internal_substring()
        {
            set_substring();
        }

        //! Gets a reference to its internal pattern sequence
        /*!
            \note Because the finder might need to make copies of the passed data (when it gets strings
                passed by reference), it stores an internal sequence of type Sequence1T,
                which is used if a copy is necessary and left empty otherwise
            \return The internal pattern sequence
        */
        typename boost::call_traits<substring_type>::const_reference get_internal_substring() const
        {
            return substring_optional_copy_;
        }

        //!\overload
        /*!
            \warning If you change the internal pattern, you must call \ref use_internal_substring()
                so that the finder can ask the string search algorithm to recompute its information on the new pattern
        */
        typename boost::call_traits<substring_type>::reference get_internal_substring()
        {
            return substring_optional_copy_;
        }

        //! Gets a reference to its internal text sequence.
        //! \sa get_internal_substring()
        typename boost::call_traits<substring_type>::const_reference get_internal_string() const
        {
            return substring_optional_copy_;
        }

        //! Gets a reference to its internal text sequence.
        //! \sa get_internal_substring()
        typename boost::call_traits<substring_type>::reference get_internal_string()
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
            if (start_offset_ == boost::end(string_range_))
                return boost::iterator_range<string_iterator_type>(
                    start_offset_, start_offset_
                );
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
            if (boost::begin(substring_range_) == boost::end(substring_range_))
            {
                //empty string, empty substring
                // TODO if this gets called more times, it always returns 0
                //     i.e. the pointer is not moved. what would be a good solution for that?
                //     maybe a special dummy value for the range?
                if (boost::begin(string_range_) == boost::end(string_range_))
                    return static_cast<string_difference_type>(0);
                //empty substring, offset at the end of the range
                if (start_offset_ == boost::end(string_range_))
                    return static_cast<string_difference_type>(-1);
                //empty substring, offset not at the end of range
                return std::distance(boost::begin(string_range_),start_offset_++);
            }
            else if (boost::begin(string_range_) == boost::end(string_range_))
            {
                //empty string, nonempty substring
                return static_cast<string_difference_type>(-1);
            }

            //perform an actual search
            string_range_type const &match = find_next();
            if (boost::begin(match) == boost::end(string_range_))
                return static_cast<string_difference_type>(-1);
            return std::distance(boost::begin(string_range_), boost::begin(match));
        }

        //! Reset the internal offset to the beginning of the text.
        void find_reset()
        {
            start_offset_ = boost::begin(string_range_);
        }

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

        substring_type substring_optional_copy_;
        string_type string_optional_copy_;
        comparator_type comparator_;
        allocator_type allocator_;
        substring_range_type substring_range_;
        string_range_type string_range_;
        string_iterator_type start_offset_;
        bool substring_has_changed_, string_has_changed_;
    };


} }

namespace boost
{
    using algorithm::finder_t;
}
#endif // BOOST_ALGORITHM_FINDER_T_HPP
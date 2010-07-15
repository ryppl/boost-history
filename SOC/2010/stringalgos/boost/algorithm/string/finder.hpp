//  Boost string_algo library finder.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_FINDER_HPP
#define BOOST_STRING_FINDER_HPP

#include <boost/algorithm/string/config.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/const_iterator.hpp>

#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/detail/finder.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <boost/call_traits.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/concept_check.hpp>

//!\todo modify this
/*! \file
    Defines Finder types and Finder generators. Finder object is a functor which is able to 
    find a substring matching a specific criteria in the input.
    Finders are used as a pluggable components for replace, find 
    and split facilities. This header contains generator functions 
    for finders provided in this library.
*/

namespace boost { namespace algorithm {


//  Finder types ---------------------------------------------//

        template <class,class,class,class,class,class> struct finder_no_additional_behavior;
        template <class,class,class,class,class,class> class finder_profiling_behavior;
        template <class,class,class,class,class,class> class finder_functor_first_finder;
        template <class,class,class,class,class,class> class finder_functor_last_finder;
        template <class,class,class,class,class,class> class finder_functor_nth_finder;


        template <class Range1T, class Range2T, class AlgorithmT,
            class ComparatorT = ::boost::algorithm_is_equal,
            class AllocatorT = typename AlgorithmT::default_allocator_type,
            template <class,class,class,class,class,class> class AdditionalBehaviorT =
                boost::algorithm::finder_no_additional_behavior
        >
        class simplified_finder_t :
            public boost::algorithm::detail::finder_typedefs<Range1T,Range2T,ComparatorT,AllocatorT>,
            private AlgorithmT::algorithm<
                simplified_finder_t<Range1T, Range2T, AlgorithmT, ComparatorT, AllocatorT, AdditionalBehaviorT>,
                typename boost::range_const_iterator<Range1T>::type,
                typename boost::range_iterator<Range2T>::type,
                ComparatorT,AllocatorT>
        {
            //! \todo Add concept assertions here.

        public:
            simplified_finder_t(Range1T const *const substr, Range2T *str,
                ComparatorT comparator = ComparatorT(), AllocatorT allocator = AllocatorT())
                : substring_range_(boost::as_literal(*substr)),
                string_range_(boost::as_literal(*str)),
                comparator_(comparator), allocator_(allocator),
                substring_has_changed_(true), string_has_changed_(true),
                algorithm()
            { }

            void set_substring (Range1T const *substr)
            { substring_range_ = boost::as_literal(*substr); substring_has_changed_ = true; }

            void set_string (Range2T *str)
            { string_range_ = boost::as_literal(*str); string_has_changed_ = true; }

            void find_reset ()
            { start_offset_ = boost::begin(string_range_); }
            
            string_range_type find_first ()
            {
                find_reset();
                return find_next();
            }

            void refresh()
            {
                string_has_changed_ = true;
                find_reset();
            }

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

            substring_range_type get_substring_range() const { return substring_range_; }
            string_range_type get_string_range() const { return string_range_; }
            
            typename boost::call_traits<comparator_type>::reference get_comparator()
            { return comparator_; }

            typename boost::call_traits<comparator_type>::const_reference get_comparator() const
            { return comparator_;  }
       
            //! Gets a reference to the current allocator
            typename boost::call_traits<allocator_type>::reference get_allocator()
            { return allocator_; }
            
            /*!
                \overload
            */
            typename boost::call_traits<allocator_type>::const_reference get_allocator() const
            { return allocator_; }

        protected:
            substring_range_type substring_range_;
            string_range_type string_range_;
            bool substring_has_changed_, string_has_changed_;

            comparator_type comparator_;
            allocator_type allocator_;

            string_iterator_type start_offset_;
        };

        //! \todo copyable finder types

        //! A generic finder type
        /*!
            Allows simple use of various string searching algorithms.
            \tparam Sequence1T The type of the substring
            \tparam Sequence2T The type of the string
            \tparam Algorithm An algorithm class
                which will be used for performing the searches. \see string_search.hpp
            \tparam Comparator Optional template parameter passed to the algorithm.
                Used for comparing individual characters for equality.
            \tparam Allocator Optional template parameter passed to the algorithm
                in the event that additional computation on the data has to be stored.
         */
        template <
            class Sequence1T, class Sequence2T,
            class Algorithm,
            typename Comparator = ::boost::algorithm::is_equal,
            class Allocator = typename Algorithm::default_allocator_type,
            template <class,class,class,class,class,class> class AdditionalBehavior =
                boost::algorithm::finder_no_additional_behavior
        >
        class finder_t :
            private Algorithm::algorithm<
                typename finder_t<Sequence1T, Sequence2T, Algorithm, Comparator, Allocator, AdditionalBehavior>,
                typename boost::range_const_iterator<Sequence1T>::type,
                typename boost::range_iterator<Sequence2T>::type,
                Comparator, Allocator>,
            private AdditionalBehavior<
                typename finder_t<Sequence1T, Sequence2T, Algorithm, Comparator, Allocator, AdditionalBehavior>,
                Sequence1T,Sequence2T,Algorithm,Comparator,Allocator>
        {
            //! \todo: Maybe write a String concept?
            //! \todo: Currently, there's a SGI Sequence Concept implemented by Boost.ConceptCheck,
            //!         but std::string does not obey this concept, which means that even calling these template
            //!         paramters sequences is wrong.
            BOOST_CONCEPT_ASSERT((boost::Container<Sequence1T>));
            BOOST_CONCEPT_ASSERT((boost::Container<Sequence2T>));
        public:
            //! The type of the substring
            typedef Sequence1T substring_type;
            //! The type of the string
            typedef Sequence2T string_type;
            //! The type of the comparator
            typedef Comparator comparator_type;
            //! The type of the allocator
            typedef Allocator allocator_type;
            //! The type of the substring's iterator
            typedef typename boost::range_const_iterator<Sequence1T>::type substring_iterator_type;
            //! The type of the string's iterator
            typedef typename boost::range_iterator<Sequence2T>::type string_iterator_type;
            //! The character type of the substring
            typedef typename boost::iterator_value<substring_iterator_type>::type substring_char_type;
            //! The character type of the string
            typedef typename boost::iterator_value<string_iterator_type>::type string_char_type;
            //! The type of the algorithm
            typedef typename Algorithm::algorithm<finder_t,
                substring_iterator_type,
                string_iterator_type,
                Comparator, Allocator> algorithm_type;
            typedef typename boost::iterator_range<substring_iterator_type> substring_range_type;
            typedef typename boost::iterator_range<string_iterator_type> string_range_type;
            typedef typename boost::iterator_difference<string_iterator_type>::type string_difference_type;
            
            //! Constructs a finder given a string and a substring
            /*!
                \param substring Either a range (or character array)
                    of the substring to be searched, or a pointer to a sequence of type \c substring_type.
                \param string Either a range (or character array)
                    of the string to be searched, or a pointer to a sequence of type \c string_type.
                \param comparator A comparator object passed on to the algorithm
                \param allocator An allocator object passed on to the algorithm
                \note Both the substring and string can be passed either as references of ranges,
                    or as pointers to sequences. In the former case, the substring and/or string is copied
                    inside the class. In the latter class, the pointer is used and no copy is made.
                \warning Whereas the overloads taking pointers are faster (because no strings are copied around),
                    you have to guarantee that the lifetime of your pointee is at least as long as the lifetime
                    of the finder. If you cannot guarantee on the lifetime, use a reference instead, which will
                    force a copy.
                \note If a rvalue reference is passed as the string or substring, and your compiler supports rvalue
                    references, then a move is performed as opposed to a copy.
             */
            explicit finder_t (const Sequence1T *const substring = 0, Sequence2T *const string = 0,
                Comparator comparator = Comparator(), Allocator allocator = Allocator()) 
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
                Comparator comparator = Comparator(), Allocator allocator = Allocator(),
                typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0)
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(), substring_range_(substring?*substring:substring_optional_copy_),
                string_optional_copy_(), string_range_(),
                start_offset_(),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            {
                set_string(string);
                //on_substring_change();
            }

            //! \overload
            template <class Range1T>
            explicit finder_t (const Range1T &substring, Sequence2T *const string = 0,
                Comparator comparator = Comparator(), Allocator allocator = Allocator(),
                typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0)
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(), substring_range_(),
                string_optional_copy_(), string_range_(string?boost::as_literal(*string):string_optional_copy_),
                start_offset_(boost::begin(string_range_)),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            {
                set_substring(substring);
                //on_string_change();
            }

            //! \overload
            template <class Range1T, class Range2T>
            finder_t (const Range1T &substring, const Range2T &string,
                Comparator comparator = Comparator(), Allocator allocator = Allocator(),
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

#           ifdef BOOST_HAS_RVALUE_REFS
            //! \overload
            template <class Range2T>
            explicit finder_t (
                Sequence1T const &&substring,
                Sequence2T *const string = 0,
                Comparator comparator = Comparator(), Allocator allocator = Allocator()) 
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(std::move(substring)), string_optional_copy_(),
                substring_range_(substring_optional_copy_), string_range_(string?*string:string_optional_copy_),
                start_offset_(boost::begin(string_range_)),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            { /*on_substring_change(); on_string_change();*/ }

            //! \overload
            template <class Range2T>
            explicit finder_t (
                Sequence1T const &&substring,
                const Range2T &string,
                Comparator comparator = Comparator(), Allocator allocator = Allocator(),
                typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range2T,Sequence2T> >::type* = 0) 
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(std::move(substring)), substring_range_(substring_optional_copy_),
                string_optional_copy_(), string_range_(),
                start_offset_(),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            { set_string(string); /*on_substring_change();*/ }

            //! \overload
            finder_t (
                Sequence1T const &&substring,
                Sequence2T &&string,
                Comparator comparator = Comparator(), Allocator allocator = Allocator()) 
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(std::move(substring)), string_optional_copy_(std::move(string)),
                substring_range_(substring_optional_copy_), string_range_(string_optional_copy_),
                start_offset_(boost::begin(string_range_)),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            { /*on_substring_change(); on_string_change();*/ }

            //! \overload
            finder_t (const Sequence1T *const substring,
                Sequence2T &&string,
                Comparator comparator = Comparator(), Allocator allocator = Allocator()) 
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(), string_optional_copy_(std::move(string)),
                substring_range_(substring?*substring:substring_optional_copy_), string_range_(string_optional_copy_),
                start_offset_(boost::begin(string_range_)),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            { /*on_substring_change(); on_string_change();*/ }
            
            //! \overload
            template <class Range1T>
            finder_t (const Range1T &substring,
                Sequence2T &&string,
                Comparator comparator = Comparator(), Allocator allocator = Allocator(),
                typename boost::disable_if<typename ::boost::algorithm::detail::is_pointer_to<Range1T,Sequence1T> >::type* = 0) 
                : comparator_(comparator), allocator_(allocator),
                substring_optional_copy_(), substring_range_(),
                string_optional_copy_(std::move(string)), string_range_(string_optional_copy_),
                start_offset_(boost::begin(string_range_)),
                algorithm_type(),
                substring_has_changed_(true), string_has_changed_(true)
            { set_substring(substring); /*on_string_change();*/ }

#           endif


            //! Get an iterator range of the currently stored substring
            /*!
                \return An iterator range of the currently stored substring
             */
            //! \todo This may return iterators for copies of the string, properly deal with it.
            typename substring_range_type get_substring_range() const
            { return substring_range_; }

            //! Get an iterator range of the currently stored string
            /*!
                \return An iterator range of the currently stored string
             */
            typename string_range_type get_string_range() const
            { return string_range_; }

            typename boost::call_traits<Comparator>::reference get_comparator()
            { return comparator_; }

            typename boost::call_traits<Comparator>::const_reference get_comparator() const
            { return comparator_;  }

            //! \todo: any reason why stdlib get_allocator()s return value types?

           
            //! Gets a reference to the current allocator
            typename boost::call_traits<Allocator>::reference get_allocator()
            { return allocator_; }
            
            /*!
                \overload
            */
            typename boost::call_traits<Allocator>::const_reference get_allocator() const
            { return allocator_; }

            //! Changes the substring to be searched for.
            /*!
                \param substring Either a range (or character array) corresponding to the new substring,
                    or a pointer to a sequence of type \c substring_type
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
            
            void set_substring (Sequence1T const *const substring = 0)
            {
                substring_optional_copy_.clear();
                if (substring)
                    substring_range_ = *substring;
                else
                    substring_range_ = substring_optional_copy_;
                substring_has_changed_ = true;
            }

#           ifdef BOOST_HAS_RVALUE_REFS
            void set_substring (
                Sequence1T const &&substring)
            {
                substring_optional_copy_ = std::move(substring);
                substring_range_ = substring_optional_copy_;
                substring_has_changed_ = true;
            }
#           endif

            //! Changes the string to be searched for.
            /*!
                \param string Either a range (or character array) corresponding to the new substring,
                    or a pointer to a sequence of type \c string_type
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
            
            void set_string (Sequence2T *const string = 0)
            {
                string_optional_copy_.clear();
                if (string)
                    string_range_ = *string;
                else
                    string_range_ = string_optional_copy_;
                string_has_changed_ = true;
            }

#           ifdef BOOST_HAS_RVALUE_REFS
            void set_string (
                Sequence2T &&string)
            {
                string_optional_copy_ = std::move(string);
                string_range_ = string_optional_copy_;
                string_has_changed_ = true;
            }
#           endif

            //! \todo Change the object's substring or just use a temporary one?
            //! \todo Maybe this shouldn't be a part of finder_t, but a part of a certain AdditionalBehaviorT
            //! Perform a search...
            /*!
                \deprecated Only implemented to preserve compatibility
                    with the previous Finder concept
                \todo This should probably only exist to classes that derive from finder_t (such as first_finder_t etc.)
             */
            string_range_type operator()(string_iterator_type const &string_start,
                string_iterator_type const &string_end)
            {
                set_string( boost::make_iterator_range(string_start, string_end) );
                return find_first();
            }
            
            //! Performs a search using the chosen algorithm.
            /*!
                Looks for the first match of the substring in the string.
                \return An iterator range indicating where in the string
                    a match has occured. If there is no match, an iterator range with
                    begin()==end()==string.end() is returned.
                \pre The iterator ranges for the string and substring must be set.
                \post The internal find offset is set immediately after the current match starts.
                \note This is semantically equivalent to \c find_reset(); match=find_next();
             */

            void refresh()
            {
                string_has_changed_ = true;
                find_reset();
            }

            //!\todo Must return a range of const iterators, otherwise one could modify
            //!         the range's contents, range which may actually
            //!         be data of our private member
            string_range_type find_first()
            {
                //assert(substring_ && string_);
                //return algorithm_type::find(boost::begin(string_));
                find_reset();
                return find_next();
            }

            string_difference_type find_first_index()
            {
                find_reset();
                return find_next_index();
            }

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

            string_difference_type find_next_index()
            {
                apply_changes();
                
                //empty substring
                if (boost::begin(substring_range_) == boost::end(substring_range_))
                {
                    if (start_offset_ == boost::end(string_range_))
                        return static_cast<string_difference_type>(-1);
                    return std::distance(boost::begin(string_range_),start_offset_++);
                }

                string_range_type const &match = find_next();
                if (boost::begin(match) == boost::end(string_range_))
                    return static_cast<string_difference_type>(-1);
                return std::distance(boost::begin(string_range_), boost::begin(match));
            }

            void find_reset()
            {
                start_offset_ = boost::begin(string_range_);
            }

            //! \todo: Figure out whether you want to make finder iterators or not. find_iterator can be used otherwise.
            //const_iterator begin() const { }
            //const_iterator end() const { }
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

        template <class,class,class,class,class,class> struct finder_no_additional_behavior { };


//  Finder generators ------------------------------------------//
        
        //! "First" finder generator
        /*!
            Construct the \c first_finder_t. The finder searches for the first
            occurrence of the string in a given input.
            The result is given as an \c iterator_range delimiting the match.

            \param Search A substring to be searched for.
            \param Comp An element comparison predicate
            \return An instance of the \c first_finder object
        */
        template<typename RangeT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            is_equal>
        first_finder( const RangeT& Search )
        {
            return 
                detail::first_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                        is_equal>( ::boost::as_literal(Search), is_equal() ) ;
        }

        //! "First" finder
        /*!
            \overload
        */
        template<typename RangeT,typename PredicateT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            PredicateT>
        first_finder( 
            const RangeT& Search, PredicateT Comp )
        {
            return 
                detail::first_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                    PredicateT>( ::boost::as_literal(Search), Comp );
        }

        //! "Last" finder
        /*!
            Construct the \c last_finder. The finder searches for the last
            occurrence of the string in a given input.
            The result is given as an \c iterator_range delimiting the match.

            \param Search A substring to be searched for.
            \param Comp An element comparison predicate
            \return An instance of the \c last_finder object
        */
        template<typename RangeT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            is_equal>
        last_finder( const RangeT& Search )
        {
            return 
                detail::last_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                    is_equal>( ::boost::as_literal(Search), is_equal() );
        }
        //! "Last" finder
        /*!
            \overload
        */
        template<typename RangeT, typename PredicateT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            PredicateT>
        last_finder( const RangeT& Search, PredicateT Comp )
        {
            return 
                detail::last_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                    PredicateT>( ::boost::as_literal(Search), Comp ) ;
        }

        //! "Nth" finder
        /*!
            Construct the \c nth_finder. The finder searches for the n-th (zero-indexed)
            occurrence of the string in a given input.
            The result is given as an \c iterator_range delimiting the match.

            \param Search A substring to be searched for.
            \param Nth An index of the match to be find
            \param Comp An element comparison predicate
            \return An instance of the \c nth_finder object
        */
        template<typename RangeT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            is_equal>
        nth_finder( 
            const RangeT& Search, 
            int Nth)
        {
            return 
                detail::nth_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                    is_equal>( ::boost::as_literal(Search), Nth, is_equal() ) ;
        }
        //! "Nth" finder
        /*!
            \overload
        */
        template<typename RangeT, typename PredicateT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME range_const_iterator<RangeT>::type,
            PredicateT>
        nth_finder( 
            const RangeT& Search, 
            int Nth, 
            PredicateT Comp )
        {
            return 
                detail::nth_finderF<
                    BOOST_STRING_TYPENAME 
                        range_const_iterator<RangeT>::type,
                    PredicateT>( ::boost::as_literal(Search), Nth, Comp );
        }

        //! "Head" finder
        /*!
            Construct the \c head_finder. The finder returns a head of a given
            input. The head is a prefix of a string up to n elements in
            size. If an input has less then n elements, whole input is 
            considered a head.
            The result is given as an \c iterator_range delimiting the match.

            \param N The size of the head
            \return An instance of the \c head_finder object
        */
        inline detail::head_finderF
        head_finder( int N )
        {
            return detail::head_finderF(N);
        }
        
        //! "Tail" finder
        /*!
            Construct the \c tail_finder. The finder returns a tail of a given
            input. The tail is a suffix of a string up to n elements in
            size. If an input has less then n elements, whole input is 
            considered a head.
            The result is given as an \c iterator_range delimiting the match.

            \param N The size of the head
            \return An instance of the \c tail_finder object
        */
        inline detail::tail_finderF
        tail_finder( int N )
        {
            return detail::tail_finderF(N);
        }

        //! "Token" finder
        /*!
            Construct the \c token_finder. The finder searches for a token 
            specified by a predicate. It is similar to std::find_if 
            algorithm, with an exception that it return a range of
            instead of a single iterator.

            If "compress token mode" is enabled, adjacent matching tokens are 
            concatenated into one match. Thus the finder can be used to 
            search for continuous segments of characters satisfying the 
            given predicate.

            The result is given as an \c iterator_range delimiting the match.

            \param Pred An element selection predicate
            \param eCompress Compress flag
            \return An instance of the \c token_finder object
        */
        template< typename PredicateT >
        inline detail::token_finderF<PredicateT>
        token_finder( 
            PredicateT Pred, 
            token_compress_mode_type eCompress=token_compress_off )
        {
            return detail::token_finderF<PredicateT>( Pred, eCompress );
        }

        //! "Range" finder
        /*!
            Construct the \c range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

            \param Begin Beginning of the range
            \param End End of the range
            \param Range The range.
            \return An instance of the \c range_finger object
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder(
            ForwardIteratorT Begin,
            ForwardIteratorT End )
        {
            return detail::range_finderF<ForwardIteratorT>( Begin, End );
        }

        //! "Range" finder
        /*!       
            \overload
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder( iterator_range<ForwardIteratorT> Range )
        {
            return detail::range_finderF<ForwardIteratorT>( Range );
        }

    } // namespace algorithm

    // pull the names to the boost namespace
    using algorithm::first_finder;
    using algorithm::last_finder;
    using algorithm::nth_finder;
    using algorithm::head_finder;
    using algorithm::tail_finder;
    using algorithm::token_finder;
    using algorithm::range_finder;


    //! \TODO: any other finder types?
    using algorithm::finder_t;

} // namespace boost


#endif  // BOOST_STRING_FINDER_HPP

//  Boost string_algo library find.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/algorithm/string/config.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/as_literal.hpp>

#include <boost/algorithm/string/finder/finder_generators.hpp>
#include <boost/algorithm/string/finder/generated_finders.hpp>
#include <boost/algorithm/string/finder/default_search_algorithm.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/constants.hpp>

/*! \file
    Defines a set of find algorithms. The algorithms are searching
    for a substring of the input. The result is given as an \c iterator_range
    delimiting the substring.
*/

//todo update doc here

namespace boost {
    namespace algorithm {

//  Generic find -----------------------------------------------//

        //! Generic find algorithm
        /*!
            Search the input using the given finder.

            \param Input A string which will be searched.
            \param Finder Finder object used for searching.
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c RangeT::iterator or 
                \c RangeT::const_iterator, depending on the constness of 
                the input parameter.
            \deprecated
        */
        template<typename RangeT, typename FinderT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<RangeT>::type>
        find( 
            RangeT& Input, 
            FinderT& Finder)
        {

            iterator_range<BOOST_STRING_TYPENAME range_iterator<RangeT>::type> lit_input(
                ::boost::as_literal(Input));
            return Finder(::boost::begin(lit_input), ::boost::end(lit_input));
        }

//  find_first  -----------------------------------------------//

        //! Find first algorithm (not deprecated)
        /*!
            Search for the first occurrence of the substring in the input. 
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \return 
            An \c iterator_range delimiting the match. 
            Returned iterator is either \c RangeT::iterator or 
            \c RangeT::const_iterator, depending on the constness of 
            the input parameter.

            \note This function provides the strong exception-safety guarantee
            */
        template<typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        find_first( 
            Range1T& Input, 
            const Range2T& Search,
            AlgorithmTagT const &algorithm)
        {
            //we want to generate finders taking const ranges for text (thus the const Range1T)
            //this only allows the finder to return matches as const ranges,
            //but find() can convert those to mutable ranges if they are provided by the user
            return ::boost::algorithm::find(Input,
                ::boost::algorithm::first_finder_t<Range2T,
                    typename AlgorithmTagT::type,::boost::algorithm::is_equal>(&Search)
                );
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        find_first( 
            Range1T& Input, 
            const Range2T& Search)
        {
            return boost::algorithm::find_first(Input, Search, boost::algorithm::default_finder_algorithm_tag());
        }


        //! Find first algorithm ( case insensitive )
        /*!
            Search for the first occurence of the substring in the input. 
            Searching is case insensitive.
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \param Loc A locale used for case insensitive comparison
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.

            \note This function provides the strong exception-safety guarantee
        */
        template<typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_first( 
            Range1T& Input, 
            const Range2T& Search,
            AlgorithmTagT const &,
            const std::locale& Loc=std::locale())
        {
            //boost::algorithm::simplified_finder_t<Range2T, Range1T, typename AlgorithmTagT::type,
            //    ::boost::algorithm::is_iequal> finder(&Search, &Input, ::boost::algorithm::is_iequal(Loc));
            //return finder.find_first();

            return ::boost::algorithm::find(Input,
                ::boost::algorithm::first_finder_t<Range2T,
                    typename AlgorithmTagT::type,::boost::algorithm::is_iequal>(&Search, ::boost::algorithm::is_iequal(Loc))
                );
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_first(
            Range1T& Input, 
            const Range2T& Search,
            const std::locale& Loc=std::locale())
        {
            return boost::algorithm::ifind_first(Input, Search,
                boost::algorithm::default_finder_algorithm_tag(), Loc);
        }

//  find_last  -----------------------------------------------//

        //! Find last algorithm
        /*!
            Search for the last occurrence of the substring in the input. 
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.

            \note This function provides the strong exception-safety guarantee
        */
        template<typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        find_last( 
            Range1T& Input, 
            const Range2T& Search,
            AlgorithmTagT const &)
        {
            /*
            simplified_finder_t<Range2T, Range1T, typename AlgorithmTagT::type>
                finder(&Search, &Input);
            boost::iterator_range<typename boost::range_iterator<Range1T>::type>
                crt = make_iterator_range(boost::end(Input), boost::end(Input)),
                prev;
            for (;;)
            {
                prev=crt;
                crt=finder.find_next();
                //note: we don't use boost::end(Input) on the rhs, because Input can be a character
                //array, which means we would have to apply boost::as_literal to Input first.
                //however, the finder does that for us before saving the range, so we use the internal
                //range instead.
                if (boost::begin(crt) == boost::end(finder.get_string_range())) break;
            }
            return prev;
            */
            return ::boost::algorithm::find(Input,
                ::boost::algorithm::last_finder_t<Range2T,
                    typename AlgorithmTagT::type,::boost::algorithm::is_equal>(&Search)
                );
            //return ::boost::algorithm::find(Input, ::boost::algorithm::last_finder(Search));
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        find_last( 
            Range1T& Input, 
            const Range2T& Search)
        {
            return boost::algorithm::find_last(Input, Search,
                boost::algorithm::default_finder_algorithm_tag());
        }

        //! Find last algorithm ( case insensitive )
        /*!
            Search for the last match a string in the input. 
            Searching is case insensitive.
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \param Loc A locale used for case insensitive comparison
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.
        
            \note This function provides the strong exception-safety guarantee    
        */
        template<typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_last( 
            Range1T& Input, 
            const Range2T& Search,
            AlgorithmTagT const &,
            const std::locale& Loc=std::locale())
        {
            return ::boost::algorithm::find(Input,
                ::boost::algorithm::last_finder_t<Range2T,
                    typename AlgorithmTagT::type,::boost::algorithm::is_iequal>(&Search, ::boost::algorithm::is_iequal(Loc)));
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_last( 
            Range1T& Input, 
            const Range2T& Search,
            const std::locale& Loc=std::locale())
        {
            return ::boost::algorithm::ifind_last(Input, Search,
                boost::algorithm::default_finder_algorithm_tag(), Loc);
        }

//  find_nth ----------------------------------------------------------------------//

        //! Find n-th algorithm 
        /*!
            Search for the n-th (zero-indexed) occurrence of the substring in the 
            input.         
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \param Nth An index (zero-indexed) of the match to be found.
                For negative N, the matches are counted from the end of string.
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.
        */


        template<typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
            find_nth( 
            Range1T& Input, 
            const Range2T& Search,
            int Nth,
            AlgorithmTagT const &)
        {
            return ::boost::algorithm::find(Input, 
                ::boost::algorithm::nth_finder_t<Range2T,
                    typename AlgorithmTagT::type, ::boost::algorithm::is_equal>(&Search, ::boost::algorithm::is_equal(), Nth)
                    );
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        find_nth( 
            Range1T& Input, 
            const Range2T& Search,
            int Nth)
        {
            return ::boost::algorithm::find_nth(Input, Search, Nth,
                ::boost::algorithm::default_finder_algorithm_tag());
        }

        //! Find n-th algorithm ( case insensitive ).
        /*!
            Search for the n-th (zero-indexed) occurrence of the substring in the 
            input. Searching is case insensitive.
            
            \param Input A string which will be searched.
            \param Search A substring to be searched for.
            \param Nth An index (zero-indexed) of the match to be found. 
                For negative N, the matches are counted from the end of string.
            \param Loc A locale used for case insensitive comparison
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.


            \note This function provides the strong exception-safety guarantee
        */
        template <typename Range1T, typename Range2T, typename AlgorithmTagT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_nth (
            Range1T& Input,
            const Range2T &Search,
            int Nth,
            AlgorithmTagT const&,
            const std::locale& Loc=std::locale())
        {
            return ::boost::algorithm::find(Input, 
                ::boost::algorithm::nth_finder_t<Range2T,
                    typename AlgorithmTagT::type, ::boost::algorithm::is_iequal>(&Search, ::boost::algorithm::is_iequal(Loc), Nth));
        }

        template<typename Range1T, typename Range2T>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<Range1T>::type>
        ifind_nth( 
            Range1T& Input, 
            const Range2T& Search,
            int Nth,
            const std::locale& Loc=std::locale())
        {
            return ::boost::algorithm::ifind_nth(Input, Search, Nth,
                ::boost::algorithm::default_finder_algorithm_tag(), Loc);
        }

//  find_head ----------------------------------------------------------------------//

        //! Find head algorithm
        /*!
            Get the head of the input. Head is a prefix of the string of the 
            given size. If the input is shorter then required, whole input if considered 
            to be the head.

            \param Input An input string
            \param N Length of the head
                For N>=0, at most N characters are extracted.
                For N<0, size(Input)-|N| characters are extracted.
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c Range1T::iterator or 
                \c Range1T::const_iterator, depending on the constness of 
                the input parameter.

            \note This function provides the strong exception-safety guarantee
        */
        template<typename RangeT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<RangeT>::type>
        find_head( 
            RangeT& Input, 
            int N)
        {
            return ::boost::algorithm::find(Input, ::boost::algorithm::head_finder(N));
        }

//  find_tail ----------------------------------------------------------------------//

        //! Find tail algorithm
        /*!
            Get the tail of the input. Tail is a suffix of the string of the 
            given size. If the input is shorter then required, whole input if considered 
            to be the tail.

            \param Input An input string
            \param N Length of the tail. 
                For N>=0, at most N characters are extracted.
                For N<0, size(Input)-|N| characters are extracted.
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c RangeT::iterator or 
                \c RangeT::const_iterator, depending on the constness of 
                the input parameter.


            \note This function provides the strong exception-safety guarantee
        */
        template<typename RangeT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<RangeT>::type>
        find_tail( 
            RangeT& Input, 
            int N)
        {
            return ::boost::algorithm::find(Input, ::boost::algorithm::tail_finder(N));
        }

//  find_token --------------------------------------------------------------------//

        //! Find token algorithm
        /*!
            Look for a given token in the string. Token is a character that matches the
            given predicate.
            If the "token compress mode" is enabled, adjacent tokens are considered to be one match.
            
            \param Input A input string.
            \param Pred An unary predicate to identify a token
            \param eCompress Enable/Disable compressing of adjacent tokens
            \return 
                An \c iterator_range delimiting the match. 
                Returned iterator is either \c RangeT::iterator or 
                \c RangeT::const_iterator, depending on the constness of 
                the input parameter.
        
            \note This function provides the strong exception-safety guarantee    
        */
        template<typename RangeT, typename PredicateT>
        inline iterator_range< 
            BOOST_STRING_TYPENAME range_iterator<RangeT>::type>
        find_token( 
            RangeT& Input,
            PredicateT Pred,
            token_compress_mode_type eCompress=token_compress_off)
        {
            return ::boost::algorithm::find(Input, ::boost::algorithm::token_finder(Pred, eCompress));
        }

    } // namespace algorithm

    // pull names to the boost namespace
    using algorithm::find;
    using algorithm::find_first;
    using algorithm::ifind_first;
    using algorithm::find_last;
    using algorithm::ifind_last;
    using algorithm::find_nth;
    using algorithm::ifind_nth;
    using algorithm::find_head;
    using algorithm::find_tail;
    using algorithm::find_token;

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP

//  Boost string_algo library predicate2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE2_HPP
#define BOOST_STRING_PREDICATE2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/find.hpp>
#include <boost/string_algo/detail/predicate.hpp>

/*! \file boost/string_algo/predicate2.hpp
    Defines string-related predicates. 
    The predicates are resolving if a substring is contained in the input string 
    under various conditions. If a string starts with the substring, ends with the 
    substring, simply contains the substring or if both strings are equal.

    In addtion to header predicate.hpp, algorithms defined here allows
    to use custom element comparison policy.
*/

namespace boost {
    namespace string_algo {

//  starts_with predicate  -----------------------------------------------//

        //! 'Starts with' predicate
        /*!
            This predicate holds when the test container is a prefix of the Input.
            In other word, if the input starts with the test.

            \param Input An input container
            \param Test A test container
            \param Comp An element comparison predicate
            \return A result of the test
        */
        template< typename InputContainerT, typename TestContainerT, typename PredicateT >
        inline bool starts_with( 
            const InputContainerT& Input, 
            const TestContainerT& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputContainerT>::type Iterator1T;
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<TestContainerT>::type Iterator2T;
                
            Iterator1T InputEnd=string_algo::end(Input);
            Iterator2T TestEnd=string_algo::end(Test);

            Iterator1T it=string_algo::begin(Input);
            Iterator2T pit=string_algo::begin(Test);
            for(;
                it!=InputEnd && pit!=TestEnd;
                ++it,++pit)
            {
                if( !( Comp(*it,*pit) ) )
                    return false;
            }

            return pit==TestEnd;
        }

//  ends_with predicate  -----------------------------------------------//

        //! 'Ends with' predicate
        /*!
            This predicate holds when the test container is a suffix of the Input.
            In other word, if the input ends with the test.

            \param Input An input container
            \param Test A test container
            \param Comp An element comparison predicate
            \return A result of the test
        */
        template< typename InputContainerT, typename TestContainerT, typename PredicateT >
        inline bool ends_with( 
            const InputContainerT& Input, 
            const TestContainerT& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputContainerT>::type Iterator1T;
            typedef BOOST_STRING_TYPENAME boost::detail::
                iterator_traits<Iterator1T>::iterator_category category;

            return string_algo::detail::
                ends_with_iter_select( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    string_algo::begin(Test), 
                    string_algo::end(Test), 
                    Comp,
                    category() );
        }

//  contains predicate  -----------------------------------------------//

        //! 'Contains' predicate
        /*!
            This predicate holds when the test container is contained in the Input.
            
            \param Input An input container
            \param Test A test container
            \param Comp An element comparison predicate
            \return A result of the test
        */
        template< typename InputContainerT, typename TestContainerT, typename PredicateT >
        inline bool contains( 
            const InputContainerT& Input, 
            const TestContainerT& Test,
            PredicateT Comp )
        {
            if ( string_algo::empty(Test) )
            {
                // Empty range is contained always
                return true;
            }
            
            return ( ! (first_finder(Test,Comp)(begin(Input), end(Input))).empty() );
        }


//  equal predicate  -----------------------------------------------//

        //! 'Equals' predicate
        /*!
            This predicate holds when the test container is equal to the
            input container i.e. all elements in both containers are same.
            
            \param Input An input container
            \param Test A test container
            \param Comp An element comparison predicate
            \return A result of the test
        */
        template< typename InputContainerT, typename TestContainerT, typename PredicateT >
        inline bool equals( 
            const InputContainerT& Input, 
            const TestContainerT& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<InputContainerT>::type Iterator1T;
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_const_iterator<TestContainerT>::type Iterator2T;
                
            Iterator1T InputEnd=string_algo::end(Input);
            Iterator2T TestEnd=string_algo::end(Test);

            Iterator1T it=string_algo::begin(Input);
            Iterator2T pit=string_algo::begin(Test);
            for(;
                it!=InputEnd && pit!=TestEnd;
                ++it,++pit)
            {
                if( !( Comp(*it,*pit) ) )
                    return false;
            }

            return ( pit==TestEnd ) && ( it==InputEnd );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_PREDICATE2_HPP

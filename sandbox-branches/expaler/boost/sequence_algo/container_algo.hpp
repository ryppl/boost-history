// (C) Copyright Thorsten Ottosen 2002-3. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Mutating algorithms originally written by Vladimir Prus' 
// <ghost@cs.msu.su> code from Boost Wiki

// Problem: should member functions be called automatically? Or should the user 
// know that it is better to call map::find() than find( map )?

#ifndef BOOST_SEQUENCE_ALGO_CONTAINER_ALGORITHM_HPP
#define BOOST_SEQUENCE_ALGO_CONTAINER_ALGORITHM_HPP

//#include <boost/sequence_algo/algorithm.hpp>
#include "container_traits.hpp"
#include <boost/concept_check.hpp>
#include <algorithm>
#include <numeric>

namespace boost 
{

///////////////////////////////////////////////////////////////////////////
// typedef to ease returning of pairs
///////////////////////////////////////////////////////////////////////////

template< typename C1, typename C2 >
struct pair_return
{
    typedef BOOST_DEDUCED_TYPENAME std::pair
    < 
    typename container_traits<C1>::iterator,
    typename container_traits<C2>::iterator 
    > 
    mutable_pair;

    typedef BOOST_DEDUCED_TYPENAME std::pair
    <
    typename container_traits<C1>::iterator,
    typename container_traits<C2>::const_iterator 
    >                                          
    mutable_const_pair;

    typedef BOOST_DEDUCED_TYPENAME std::pair
    < 
    typename container_traits<C1>::const_iterator,
    typename container_traits<C2>::iterator 
    >                                         
    const_mutable_pair;

    typedef BOOST_DEDUCED_TYPENAME std::pair
    < 
    typename container_traits<C1>::const_iterator,
    typename container_traits<C2>::const_iterator 
    >                                       
    const_pair;

    typedef BOOST_DEDUCED_TYPENAME std::pair
    <
    typename container_traits<C1>::result_iterator,
    typename container_traits<C2>::result_iterator
    >
    result_pair;

}; // 'pair_return'

/////////////////////////////////////////////////////////////////////////
// Nonmodifying Sequence Operations
/////////////////////////////////////////////////////////////////////////

//
// @note: for compilers without ordering of template functions
//        we need to use a different approach which will remove
//        the possibility to pass r-values as arguments.
//

template< typename Container, typename UnaryOperation >
inline UnaryOperation 
for_each( Container& c, UnaryOperation fun )
{
    return std::for_each( begin( c ), end( c ), fun );
}

template< typename Container, typename UnaryOperation >
inline UnaryOperation 
for_each( const Container& c, UnaryOperation fun )
{
    return std::for_each( begin( c ), end( c ), fun );
}

namespace detail
{
    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    find_impl( C& c, const T& value, associative_container_tag )
    {
        return c.find( value );
    }

    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::const_iterator 
    find_impl( const C& c, const T& value, associative_container_tag )
    {
        return c.find( value );
    }

    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    find_impl( C& c, const T& value, ... )
    {
        return std::find( begin( c ), end( c ), value );
    }

    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::const_iterator 
    find_impl( const C& c, const T& value, ... )
    {
        return std::find( begin( c ), end( c ), value );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator 
find( Container& c, const T& value )
{
    return detail::find_impl( c, value, 
                              container_traits<Container>::container_category() ); 
}    

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator 
find( const Container& c, const T& value )
{
    return detail::find_impl( c, value, 
                              container_traits<Container>::container_category() ); 
}



template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator 
find_if( Container& c, Predicate pred )
{
    return std::find_if( begin( c ), end( c ), pred );
}

template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
find_if( const Container& c, Predicate pred )
{
    return std::find_if( begin( c ), end( c ), pred );
}   


namespace ext
{

    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    adjacent_find( Container& c )
    {
        return std::adjacent_find( begin( c ), end( c ) );
    }

    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    adjacent_find( const Container& c )
    {
        return std::adjacent_find( begin( c ), end( c ) );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator 
    adjacent_find( Container& c, BinaryPredicate pred )
    {
        return std::adjacent_find( begin( c ), end( c ), pred );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator 
    adjacent_find( const Container& c, BinaryPredicate pred )
    {
        return std::adjacent_find( begin( c ), end( c ), pred );
    }

} // namespace 'ext'




template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator 
find_first_of( Container1& c1, const Container2& c2 )
{
    return std::find_first_of( begin( c1 ), end( c1 ), 
                               begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator 
find_first_of( const Container1& c1, const Container2& c2 )
{
    return std::find_first_of( begin( c1 ), end( c1 ), 
                               begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2, 
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator 
find_first_of( Container1& c1, const Container2& c2, 
               BinaryPredicate pred )
{
    return std::find_first_of( begin( c1 ), end( c1 ), 
                               begin( c2 ), end( c2 ), pred );
}

template< typename Container1, typename Container2, 
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator 
find_first_of( const Container1& c1, const Container2& c2, 
               BinaryPredicate pred )
{
    return std::find_first_of( begin( c1 ), end( c1 ), 
                               begin( c2 ), end( c2 ), pred );
}


namespace detail
{
    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::difference_type
    count_impl( const Container& c, const T& value, associative_container_tag )
    {
        return c.count( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::difference_type
    count_impl( const Container& c, const T& value, ... )
    {
        return std::count( begin( c ), end( c ), value );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::difference_type
count( const Container& c, const T& value )
{
    return detail::count_impl( c, value, 
                       container_traits<Container>::container_category() ); 
}



template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::difference_type
count_if( const Container& c, Predicate pred )
{
    return std::count_if( begin( c ), end( c ), pred );
}


namespace ext
{
    template< typename Container, typename InputIterator >
    inline std::pair< typename container_traits<Container>::iterator,
    InputIterator >
    mismatch( Container& c, InputIterator first2 )
    {
        return std::mismatch( begin( c ), end( c ), first2 );
    }

    template< typename Container, typename InputIterator >
    inline std::pair< typename container_traits<Container>::const_iterator,
    InputIterator >
    mismatch( const Container& c, InputIterator first2 )
    {
        return std::mismatch( begin( c ), end( c ), first2 );
    }

    template< typename Container, typename InputIterator, 
    typename BinaryPredicate >
    inline std::pair< typename container_traits<Container>::iterator,
    InputIterator >
    mismatch( Container& c, InputIterator first2, BinaryPredicate pred )
    {
        return std::mismatch( begin( c ), end( c ), first2, pred );
    }

    template< typename Container, typename InputIterator, 
    typename BinaryPredicate >
    inline std::pair< typename container_traits<Container>::const_iterator, 
    InputIterator >
    mismatch( const Container& c, InputIterator first2, 
              BinaryPredicate pred )
    {
        return std::mismatch( begin( c ), end( c ), first2, pred );
    }




    template< typename Container, typename InputIterator >
    inline bool 
    equal( const Container& c, InputIterator first2 )
    {
        return std::equal( begin( c ), end( c ), first2 );
    }

    template< typename Container, typename InputIterator, 
    typename BinaryPredicate > 
    inline bool 
    equal( const Container& c, InputIterator first2, 
           BinaryPredicate pred )
    {
        return std::equal( begin( c ), end( c ), first2, pred );
    }

} // namespace 'ext'


template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator 
search( Container1& c1, const Container2& c2 )
{
    return std::search( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator 
search( const Container1& c1, const Container2& c2 )
{
    return std::search( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator
search( Container1& c1, const Container2& c2, BinaryPredicate pred )
{
    return std::search( begin( c1 ), end( c1 ), 
                        begin( c2 ), end( c2 ), pred );
}

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator
search( const Container1& c1, const Container2& c2, BinaryPredicate pred )
{
    return std::search( begin( c1 ), end( c1 ), 
                        begin( c2 ), end( c2 ), pred );
}



namespace ext
{

    template< typename Container, typename Integer, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator 
    search_n( Container& c, Integer count, const T& value )
    {
        return std::search_n( begin( c ), end( c ), count, value );
    }

    template< typename Container, typename Integer, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator 
    search_n( const Container& c, Integer count, const T& value )
    {
        return std::search_n( begin( c ), end( c ), count, value );
    }

    template< typename Container, typename Integer, 
    typename T, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    search_n( Container& c, Integer count, const T& value,
              BinaryPredicate pred )
    {
        return std::search_n( begin( c ), end( c ), count, value, pred );
    }

    template< typename Container, typename Integer, 
    typename T, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    search_n( const Container& c, Integer count, const T& value,
              BinaryPredicate pred )
    {
        return std::search_n( begin( c ), end( c ), count, value, pred );
    }

} // namespace 'ext'


template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator 
find_end( Container1& c1, const Container2& c2 )
{
    return std::find_end( begin( c1 ), end( c1 ),
                          begin( c2 ), end( c2 ) ); 
}

template< typename Container1, typename Container2 >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator 
find_end( const Container1& c1, const Container2& c2 )
{
    return std::find_end( begin( c1 ), end( c1 ),
                          begin( c2 ), end( c2 ) ); 
}

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::iterator 
find_end( Container1& c1, const Container2& c2, BinaryPredicate pred )
{
    return std::find_end( begin( c1 ), end( c1 ), 
                          begin( c2 ), end( c2 ), pred );
}

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container1>::const_iterator 
find_end( const Container1& c1, const Container2& c2, 
          BinaryPredicate pred )
{
    return std::find_end( begin( c1 ), end( c1 ), 
                          begin( c2 ), end( c2 ), pred );
}

/////////////////////////////////////////////////////////////////////////
// Modifying Sequance Operations
/////////////////////////////////////////////////////////////////////////

template< typename Container, typename OutputIterator >
inline OutputIterator
copy( const Container& c, OutputIterator result )
{
    return std::copy( begin( c ), end( c ), result );
}



template< typename Container, typename BidirectionalIterator > 
inline BidirectionalIterator
copy_backward( const Container& c, BidirectionalIterator result ) 
{
    return std::copy_backward( begin( c ), end( c ), result ); 
}



template< typename Container, typename ForwardIterator >
inline ForwardIterator 
swap_ranges( Container& c, ForwardIterator first2 ) 
{
    return std::swap_ranges( begin( c ), end( c ), first2 );
}


namespace ext
{

    template< typename Container, typename OutputIterator, 
    typename UnaryOperation > 
    inline OutputIterator 
    transform( const Container& c, OutputIterator result, 
               UnaryOperation fun )
    {
        return std::transform( begin( c ), end( c ), result, fun );
    }

    template< typename Container, typename InputIterator,
    typename OutputIterator, typename BinaryOperation >
    inline OutputIterator
    transform( const Container& c, InputIterator first2,
               OutputIterator result, BinaryOperation fun )
    {
        return std::transform( begin( c ), end( c ), first2, 
                               result, fun ); 
    }
}



template< typename Container, typename T >
inline void
replace( Container& c, const T& what, const T& with_what )
{
    return std::replace( begin( c ), end( c ), what, with_what );
}



template< typename Container, typename Predicate , typename T >
inline void
replace_if( Container& c, Predicate pred, const T& value )
{
    return std::replace_if( begin( c ), end( c ), pred, value );
}



template< typename Container, typename OutputIterator, typename T > 
inline OutputIterator
replace_copy( const Container& c, OutputIterator result, 
              const T& what, const T& with_what )
{
    return std::replace_copy( begin( c ), end( c ), result,
                              what, with_what );
}



template< typename Container, typename OutputIterator, 
typename Predicate, typename T >
inline OutputIterator
replace_copy_if( const Container& c, OutputIterator result,
                 Predicate pred, const T& value)
{
    return std::replace_copy_if( begin( c ), end( c ), 
                                 result, pred, value );     
}



template< typename Container, typename T >
inline void
fill( Container& c, const T& value )
{
    std::fill( begin( c ), end( c ), value );
}


namespace ext
{
    template< typename Container, typename Integer, typename T >
    inline void
    fill_n( Container& c, Integer size, const T& value )
    {
        std::fill_n( begin( c ), size, value );
    }
}



template< typename Container, typename Generator >
inline void
generate( Container& c, Generator gen )
{
    std::generate( begin( c ), end( c ), gen );
}



namespace ext
{
    template< typename Container, typename Integer, typename Generator >
    void
    generate_n( Container& c, Integer size, Generator gen )
    {
        std::generate_n( begin( c ), size, gen );
    }
} // namespace 'ext'



namespace detail
{
    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    remove_impl( C& c, const T& t, list_tag )
    {
        c.remove( t );
        return c.end();
    }

    template< typename C, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    remove_impl( C& c, const T& t, ... )
    {
        return std::remove( begin( c ), end( c ), t );
    }

    template< typename C, typename BP >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    remove_if_impl( C& c, BP bp, list_tag )
    {
        c.remove_if( bp );
        return c.end();
    }

    template< typename C, typename BP >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    remove_if_impl( C& c, BP bp, ... )
    {
        return std::remove_if( begin( c ), end( c ), bp );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
remove( Container& c, const T& what )
{
    return detail::remove_impl( c, what, 
                                container_traits<Container>::container_id() ); 
}



template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
remove_if( Container& c, Predicate pred )
{
    return detail::remove_if_impl( c, pred, 
                                   container_traits<Container>::container_id() ); 
}



template< typename Container, typename OutputIterator, typename T >
inline OutputIterator
remove_copy( const Container& c, OutputIterator result, const T& value )
{
    return std::remove_copy( begin( c ), end( c ), result, value );
}



template< typename Container, typename OutputIterator, typename Predicate >
inline OutputIterator
remove_copy_if( const Container& c, OutputIterator result, Predicate pred )
{
    return std::remove_copy_if( begin( c ), end( c ), 
                                result, pred );
}


namespace detail
{
    template< typename C >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    unique_impl( C& c, list_tag )
    {
        c.unique();
        return c.end();
    }

    template< typename C >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    unique_impl( C& c, ... )
    {
        return std::unique( begin( c ), end( c ) );
    }

    template< typename C, typename BP >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    unique_impl2( C& c, BP bp , list_tag )
    {
        c.unique( bp );
        return c.end();
    }

    template< typename C, typename BP >
    inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator 
    unique_impl2( C& c, BP bp , ... )
    {
        return std::unique( begin( c ), end( c ), bp );
    }
}

namespace ext
{
    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    unique( Container& c )
    {
        return detail::unique_impl( c, container_traits<Container>::container_id() );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    unique( Container& c, BinaryPredicate pred )
    {
        return detail::unique_impl2( c, pred, 
                                    container_traits<Container>::container_id() ); 
    }



    template< typename Container, typename OutputIterator >
    inline OutputIterator
    unique_copy( const Container& c, OutputIterator result )
    {
        return std::unique_copy( begin( c ), end( c ), result );
    }

    template< typename Container, typename OutputIterator, 
    typename BinaryPredicate >
    inline OutputIterator
    unique_copy( const Container& c, OutputIterator result, 
                 BinaryPredicate pred )
    {
        return std::unique_copy( begin( c ), end( c ), result, pred );
    }
}



namespace detail
{
    template< typename C >
    inline void
    reverse_impl( C& c, list_tag )
    {
        c.reverse();
    }

    template< typename C >
    inline void
    reverse_impl( C& c, ... )
    {   
        std::reverse( begin( c ), end( c ) );
    }
}

template< typename Container >
inline void
reverse( Container& c )
{
    detail::reverse_impl( c, container_traits<Container>::container_id() );
}



template< typename Container, typename OutputIterator >
inline OutputIterator
reverse_copy( const Container& c, OutputIterator result )
{
    return std::reverse_copy( begin( c ), end( c ), result );
}



template< typename Container > 
inline void
rotate( Container& c, 
        typename container_traits<Container>::iterator middle )
{
    std::rotate( begin( c ), middle, end( c ) );
}



template< typename Container, typename ForwardIterator, 
typename OutputIterator >
inline OutputIterator
rotate_copy( Container& c, ForwardIterator middle, OutputIterator result ) 
{
    return std::rotate_copy( begin( c ), middle, end( c ), result );
}

template< typename Container, typename ForwardIterator, 
typename OutputIterator >
inline OutputIterator
rotate_copy( const Container& c, ForwardIterator middle, 
             OutputIterator result ) 
{
    return std::rotate_copy( begin( c ), middle, end( c ), result );
}



template< typename Container >
inline void
random_shuffle( Container& c )
{
    std::random_shuffle( begin( c ), end( c ) );
}

template< typename Container, typename Generator >
inline void
random_shuffle( Container& c, Generator gen )
{
    std::random_shuffle( begin( c ), end( c ), gen );
}

/////////////////////////////////////////////////////////////////////////
// Sorted Sequences
/////////////////////////////////////////////////////////////////////////

namespace detail
{
    template< typename C >
    inline void sort_impl( C& c, list_tag )
    {
        c.sort();
    }

    template< typename C, typename Comp >
    inline void sort_impl2( C& c, Comp comp, list_tag )
    {
        c.sort( comp );
    }

    template< typename Container >
    inline void
    sort_impl( Container& c, ... )
    {
        std::sort( begin( c ), end( c ) );
    }

    template< typename C, typename Comp >
    inline void sort_impl2( C& c, Comp comp, ... )
    {
        std::sort( begin( c ), end( c ), comp );
    }
}

namespace ext
{
    template< typename Container >
    inline void
    sort( Container& c )
    {
        detail::sort_impl( c, container_traits<Container>::container_id() );
    }

    template< typename Container, typename Compare >
    inline void
    sort( Container& c, Compare comp )
    {
        detail::sort_impl2( c, comp, container_traits<Container>::container_id() 
); }



    template< typename Container >
    inline void
    stable_sort( Container& c )
    {
        std::stable_sort( begin( c ), end( c) );
    }

    template< typename Container, typename Compare >
    inline void
    stable_sort( Container& c, Compare comp )
    {
        std::stable_sort( begin( c ), end( c ), comp );
    }

} // namespace 'ext'


namespace ext
{

    template< typename Container, typename RandomAccessIterator >
    inline void
    partial_sort( Container& c, RandomAccessIterator middle )
    {
        std::partial_sort( begin( c ), middle, end( c ) );
    }

    template< typename Container, typename RandomAccessIterator, 
    typename Compare >
    inline void
    partial_sort( Container& c, RandomAccessIterator middle,
                  Compare comp )
    {
        std::partial_sort( begin( c ), middle, end( c ), comp );
    }
}


template< typename Container1, typename InputIterator >
inline BOOST_DEDUCED_TYPENAME container_traits<InputIterator>::iterator
partial_sort_copy( const Container1& c1, InputIterator& c2 )
{
    return std::partial_sort_copy( begin( c1 ), end( c1 ), 
                                   begin( c2 ), end( c2 ) );
}

template< typename Container1, typename InputIterator, typename Compare >
inline BOOST_DEDUCED_TYPENAME container_traits<InputIterator>::iterator
partial_sort_copy( const Container1& c1, InputIterator& c2, Compare comp )
{
    return std::partial_sort_copy( begin( c1 ), end( c1 ), 
                                   begin( c2 ), end( c2 ), comp ); 
}



namespace ext
{
    template< typename Container >
    inline void
    nth_element( Container& c, 
                 typename container_traits<Container>::iterator nth )
    {
        std::nth_element( begin( c ), nth, end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    nth_element( Container& c, 
                 typename container_traits<Container>::iterator nth, 
                 Compare comp )
    {
        std::nth_element( begin( c ), nth, end( c ), comp );
    }
}



namespace detail
{
    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    lower_bound( Container& c, const T& value, associative_container_tag ) 
    {
        return c.lower_bound( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    lower_bound( const Container& c, const T& value, associative_container_tag ) 
    {
        return c.lower_bound( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    lower_bound( Container& c, const T& value, ... )
    {
        return std::lower_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    lower_bound( const Container& c, const T& value, ... )
    {
        return std::lower_bound( begin( c ), end( c ), value );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
lower_bound( Container& c, const T& value )
{
    return detail::lower_bound( c, value, 
                                container_traits<Container>::container_category() ); 
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
lower_bound( const Container& c, const T& value )
{
    return detail::lower_bound( c, value, 
                                container_traits<Container>::container_category() ); 
}

namespace ext
{
    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    lower_bound( Container& c, const T& value, Compare comp )
    {
        return std::lower_bound( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    lower_bound( const Container& c, const T& value, Compare comp )
    {
        return std::lower_bound( begin( c ), end( c ), value, comp );
    }
}



namespace detail
{
    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    upper_bound( Container& c, const T& value, associative_container_tag ) 
    {
        return c.upper_bound( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    upper_bound( const Container& c, const T& value, associative_container_tag ) 
    {
        return c.upper_bound( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    upper_bound( Container& c, const T& value, ... )
    {
        return std::upper_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    upper_bound( const Container& c, const T& value, ... )
    {
        return std::upper_bound( begin( c ), end( c ), value );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
upper_bound( Container& c, const T& value )
{
    return detail::upper_bound( c, value,                                 
                                container_traits<Container>::container_category() );  
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
upper_bound( const Container& c, const T& value )
{
    return detail::upper_bound( c, value,                               
                                container_traits<Container>::container_category() );
}

namespace ext
{
    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    upper_bound( Container& c, const T& value, Compare comp )
    {
        return std::upper_bound( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    upper_bound( const Container& c, const T& value, Compare comp )
    {
        return std::upper_bound( begin( c ), end( c ), value, comp );
    }
}



namespace detail
{

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::mutable_pair 
    equal_range( Container& c, const T& value, associative_container_tag )
    {
        return c.equal_range( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::const_pair 
    equal_range( const Container& c, const T& value, associative_container_tag )
    {
        return c.equal_range( value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::mutable_pair 
    equal_range( Container& c, const T& value, ... )
    {
        return std::equal_range( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::const_pair 
    equal_range( const Container& c, const T& value, ... )
    {
        return std::equal_range( begin( c ), end( c ), value );
    }
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::mutable_pair 
equal_range( Container& c, const T& value )
{
    return detail::equal_range( c, value, 
                                container_traits<Container>::container_category() ); 
}

template< typename Container, typename T >
inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::const_pair 
equal_range( const Container& c, const T& value )
{
    return detail::equal_range( c, value, 
                                container_traits<Container>::container_category() ); 
}

namespace ext
{

    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::mutable_pair
    equal_range( Container& c, const T& value, Compare comp )
    {
        return std::equal_range( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline BOOST_DEDUCED_TYPENAME pair_return<Container,Container>::const_pair
    equal_range( const Container& c, const T& value, Compare comp )
    {
        return std::equal_range( begin( c ), end( c ), value, comp );
    }

}


template< typename Container, typename T >
inline bool
binary_search( const Container& c, const T& value )
{
    return std::binary_search( begin( c ), end( c ), value );
}

template< typename Container, typename T, typename Compare >
inline bool
binary_search_( const Container& c, const T& value, Compare comp )
{
    return std::binary_search( begin( c ), end( c ), value, comp );
}



template< typename Container1, typename Container2, typename Container3 > 
inline BOOST_DEDUCED_TYPENAME container_traits<Container3>::iterator
merge( const Container1& c1, const Container2& c2, Container3& c3 )
{
    return std::merge( begin( c1 ), end( c1 ), begin( c2 ), 
                       end( c2 ), begin( c3 ) );  
}

template< typename Container1, typename Container2, typename Container3, 
typename Compare >
inline BOOST_DEDUCED_TYPENAME container_traits<Container3>::iterator
merge( const Container1& c1, const Container2& c2, Container3& c3,
       Compare comp )
{
    return std::merge( begin( c1 ), end( c1 ), begin( c2 ), 
                       end( c2 ), begin( c3 ), comp ); 
}



template< typename Container >
inline void
inplace_merge( Container& c, 
               typename container_traits<Container>::iterator middle )
{
    std::inplace_merge( begin( c ), middle, end( c ) );
}

template< typename Container, typename Compare >
inline void
inplace_merge_( Container& c,
                typename container_traits<Container>::iterator middle, 
                Compare comp )
{
    std::inplace_merge( begin( c ), middle, end( c ), comp );
}



template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
partition( Container& c, Predicate pred )
{
    return std::partition( begin( c ), end( c ), pred );
}



template< typename Container, typename Predicate >
inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
stable_partition( Container& c, Predicate pred )
{
    return std::stable_partition( begin( c ), end( c ), pred );
}

/////////////////////////////////////////////////////////////////////////
// Set Algorithms
/////////////////////////////////////////////////////////////////////////

template< typename Container1, typename Container2 >
inline bool 
includes( const Container1& c1, const Container2& c2 )
{
    return std::includes( begin( c1 ), end( c1 ),
                          begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2, typename Compare >
inline bool 
includes( const Container1& c1, const Container2& c2, Compare comp )
{
    return std::includes( begin( c1 ), end( c1 ),
                          begin( c2 ), end( c2 ), comp );
}



template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_union( const Container1& c1, const Container2& c2, 
           OutputIterator result )
{
    return std::set_union( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ),
                           result );
}

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_union( const Container1& c1, const Container2& c2,
           OutputIterator result, Compare comp )
{
    return std::set_union( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ),
                           result, comp );
}



template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_intersection( const Container1& c1, const Container2& c2, 
                  OutputIterator result )
{
    return std::set_intersection( begin( c1 ), end( c1 ), 
                                  begin( c1 ), end( c1 ),
                                  result ); 
}

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare > 
inline OutputIterator
set_intersection( const Container1& c1, const Container2& c2, 
                  OutputIterator result, Compare comp )
{
    return std::set_intersection( begin( c1 ), end( c1 ), 
                                  begin( c1 ), end( c1 ),
                                  result, comp ); 
}



template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_difference( const Container1& c1, const Container2& c2,  
                OutputIterator result )
{
    return std::set_difference( begin( c1 ), end( c1 ), begin( c2 ), 
                                end( c2 ), result );
}  

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_difference( const Container1& c1, const Container2& c2,  
                OutputIterator result, Compare comp )
{
    return std::set_difference( begin( c1 ), end( c1 ), begin( c2 ), 
                                end( c2 ), result, comp );
}




template< typename Container1, typename Container2, 
typename OutputIterator >
inline OutputIterator
set_symmetric_difference( const Container1& c1, const Container2& c2,
                          OutputIterator result )
{
    return std::set_symmetric_difference( begin( c1 ), end( c1 ),
                                          begin( c2 ), end( c2 ), 
                                          result );
}


template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_symmetric_difference( const Container1& c1, const Container2& c2,
                          OutputIterator result, Compare comp )
{
    return std::set_symmetric_difference( begin( c1 ), end( c1 ),
                                          begin( c2 ), end( c2 ), 
                                          result, comp );
}

///////////////////////////////////////////////////////////////////////////
// Heap Operations
///////////////////////////////////////////////////////////////////////////

namespace ext
{
    template< typename Container >
    inline void 
    push_heap( Container& c )
    {
        std::push_heap( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    push_heap( Container& c, Compare comp )
    {
        std::push_heap( begin( c ), end( c ), comp );
    }



    template< typename Container >
    inline void 
    pop_heap( Container& c )
    {
        std::pop_heap( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    pop_heap( Container& c, Compare comp )
    {
        std::pop_heap( begin( c ), end( c ), comp );
    }



    template< typename Container >
    inline void 
    make_heap( Container& c )
    {
        std::make_heap( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    make_heap( Container& c, Compare comp )
    {
        std::make_heap( begin( c ), end( c ), comp );
    }



    template< typename Container >
    inline void 
    sort_heap( Container& c )
    {
        std::sort_heap( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    sort_heap( Container& c, Compare comp )
    {
        std::sort_heap( begin( c ), end( c ), comp );
    }

} // namespace 'ext'

/////////////////////////////////////////////////////////////////////////
// Minimum and Maximum
/////////////////////////////////////////////////////////////////////////

namespace ext
{
    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    min_element( Container& c )
    {
        return std::min_element( begin( c ), end( c ) );
    }

    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    min_element( const Container& c )
    {
        return std::min_element( begin( c ),end( c ) );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    min_element( Container& c, BinaryPredicate pred )
    {
        return std::min_element( begin( c ), end( c ), pred );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    min_element( const Container& c, BinaryPredicate pred )
    {
        return std::min_element( begin( c ), end( c ), pred );
    }



    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    max_element( Container& c )
    {
        return std::max_element( begin( c ), end( c ) );
    }

    template< typename Container >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    max_element( const Container& c )
    {
        return std::max_element( begin( c ), end( c ) );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::iterator
    max_element( Container& c, BinaryPredicate pred )
    {
        return std::max_element( begin( c ), end( c ), pred );
    }

    template< typename Container, typename BinaryPredicate >
    inline BOOST_DEDUCED_TYPENAME container_traits<Container>::const_iterator
    max_element( const Container& c, BinaryPredicate pred )
    {
        return std::max_element( begin( c ), end( c ), pred );
    }
} // namepsce 'ext'

template< typename Container1, typename Container2 >
inline bool
lexicographical_compare( const Container1& c1, const Container2& c2 )
{
    return std::lexicographical_compare( begin( c1 ), end( c1 ), 
                                         begin( c2 ), end( c2 ) );
}

template< typename Container1, typename Container2, 
typename BinaryPredicate > 
inline bool
lexicographical_compare( const Container1& c1, const Container2& c2, 
                         BinaryPredicate pred )
{
    return std::lexicographical_compare( begin( c1 ), end( c1 ), 
                                         begin( c2 ), end( c2 ), pred );
}

/////////////////////////////////////////////////////////////////////////
// Permutations
/////////////////////////////////////////////////////////////////////////

namespace ext
{
    template< typename Container >
    inline bool
    next_permutation( Container& c )
    {
        return std::next_permutation( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline bool
    next_permutation( Container& c, Compare comp )
    {
        return std::next_permutation( begin( c ), end( c ), comp );
    }



    template< typename Container >
    inline bool
    prev_permutation( Container& c )
    {
        return std::prev_permutation( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    inline bool
    prev_permutation( Container& c, Compare comp )
    {
        return std::prev_permutation( begin( c ), end( c ), comp );
    }

} // namespace 'ext'

/////////////////////////////////////////////////////////////////////////
// Generalized Numeric Algorithms
/////////////////////////////////////////////////////////////////////////

namespace ext
{
    template< typename Container, typename T >
    inline T
    accumulate( const Container& c, T initial )
    {
        return std::accumulate( begin( c ), end( c ), initial ); 
    }

    template< typename Container, typename T, typename BinaryOperation >
    inline T
    accumulate( const Container& c, const T initial, BinaryOperation fun )
    {
        return std::accumulate( begin( c ), end( c ), initial, fun );
    }
}



template< typename Container, typename InputIterator, typename T >
inline T                            
inner_product( const Container& c, InputIterator first2,
               const T initial )
{
    return std::inner_product( begin( c ), end( c ), 
                               first2, initial );
}

template< typename Container, typename InputIterator, typename T,
typename BinaryOperation1, typename BinaryOperation2 >
inline T                            
inner_product( const Container& c, InputIterator first2,
               const T initial, BinaryOperation1 fun1, 
               BinaryOperation2 fun2 )
{
    return std::inner_product( begin( c ), end( c ), 
                               first2, initial, fun1, fun2 );
}



template< typename Container, typename OutputIterator >
inline OutputIterator
partial_sum( const Container& c, OutputIterator result )
{
    return std::partial_sum( begin( c ), end( c ), result );
}

template< typename Container, typename OutputIterator, 
typename BinaryOperation > 
inline OutputIterator
partial_sum_( const Container& c, OutputIterator result, 
              BinaryOperation fun )
{
    return std::partial_sum( begin( c ), end( c ), result, fun );
}



template< typename Container, typename OutputIterator >
inline OutputIterator
adjacent_difference( const Container& c, OutputIterator result )
{
    return std::adjacent_difference( begin( c ), end( c ), result );
}

namespace ext
{

    template<typename Container, typename OutputIterator, typename Predicate >
    inline OutputIterator
    adjacent_difference( const Container& c, OutputIterator result, 
                         Predicate pred )
    {
        return std::adjacent_difference( begin( c ), end( c ), 
                                         result, pred ); 
    }

}
/////////////////////////////////////////////////////////////////////////
// Boost algorithm extensions
/////////////////////////////////////////////////////////////////////////

template< typename Container, typename T >
void 
iota( Container& c, const T& value )
{
    iota( begin( c ), end( c ), value );
}



template< typename Container, typename T >
bool 
contains( const Container& c, const T& value )
{
    return contains( begin( c ), end( c ), value );
}



template< typename Container, typename Predicate >
bool all( const Container& c, Predicate pred )
{
    return all( begin( c ), end( c ), pred );
}



template< typename Container, typename Predicate >
bool 
none( const Container& c, Predicate pred )
{
    return none( begin( c ), end( c ), pred );
}



template< typename Container, typename Predicate >
bool 
any_if( const Container& c, Predicate pred )
{
    return any_if( begin( c ), end( c ), pred );
}



template< typename Container >
bool 
is_sorted( const Container& c )
{
    return is_sorted( begin( c ), end( c ) );
}

template< typename Container, typename Compare >
bool
is_sorted( const Container& c, Compare comp )
{
    return is_sorted( begin( c ), end( c ), comp );
}

template<class Container>
void
grow(Container& container, std::size_t by_how_much = 1)
{
    container.resize(container.size() + by_how_much);
}

template<class Container>
void
grow_to(Container& container, std::size_t to_what_size)
{
    container.resize(max(container.size(), to_what_size));
}

template<class Container, class T>
void
remove_and_erase(Container& container, const T& value)
{
    container.erase(remove(container, value), container.end());
}

template<class Container, class Predicate>
void
remove_and_erase_if(Container& container, Predicate p)
{
    container.erase(remove_if(container, p), container.end());
}

}

// namespace boost

#endif // BOOST_CONTAINER_ALGORITHM_HPP


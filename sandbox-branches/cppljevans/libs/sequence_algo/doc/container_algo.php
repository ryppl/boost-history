<?

include_once( "common.php" );
include_once( "util/boost.php" );

echo bareHtmlHeader( "Standard Container Algortihms" );

echo pictureAndName( "Standard Container Algorithms" );
echo overviewLinks(); echo hr();
echo introduction();  echo hr();
echo reference();     echo hr();
echo portability();   echo hr();
echo copyRightMEssage();    

echo htmlFooter();

//////////////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////////////
//
// overviewLinks(); 
// introduction();  
// reference();      
// portability();   
// references();    
// 
// introLink();
// synopsLink();
// portLink();
// refLink();
// iteratorRangeLink();
// functionList(); 
// stlLink1();
// stlLink2();
// nonModifyingLink();
// modifyingLink();
// numericLink();
// 
//

function overviewLinks()
{
    $list = "";
    $list .= item( introLink() ) . 
             item( referenceLink() ) .
             item( portLink() ) .
             item( refLink() ); 
    return ulist( $list );
}



function introduction()
{
    $res  = beginSection( introLink() );
    $motivation = p( "This library makes it possible to use all the standard 
                      algorithms with many different coontainers instead of a pair of iterators.
                      The magic that makes this possible are overloaded versions of the 
                     free-standing functions " . code( "begin()" ) . " and " . code( "end()" ) .
                     " and the accompanying traits class " . iteratorRangeLink() . "." ); 
    
    $advantages = P( "The container algorithms has several advantages over
                     the normal algorithms:" .
                     ulist( li( "Shorter and more comprehensible code" ) .
                            li( "Functional style programming is now possible" ) .
                            li( "Less error prone when using built-in arrays" ) .
                            li( "Container specialized algorithms are 
                                 called whenever possible" ) ) );
                     
    $example = p( "Below are given some small examples:" . 
                  pre( "
       //
       // Example 1: basic iteration
       //
       
       std::vector<int> my_vector;
       int              my_array[size];
       std::pair<iterator,iterator> my_range;
       // ...
       
       // for_each( begin( my_vector ), end( my_vector ), print() );
       for_each( my_vector, print() );
       
       // for_each( begin( my_array ), end( my_array ), print() );
       for_each( my_array, print() );
       
       // for_each( begin( my_range ), end( my_range ), print() );
       for_each( my_range, print() );
       
       //
       // Example 2: functional style programming
       //            (courtesy of Aleksey Gurtovoy)
       //
       
       // std::pair<record_iterator,record_iterator> sel = selection();
       // std::pair<record_iterator,record_iterator> mismatching_part =
       // std::mismatch( sel.first, sel.second, old_selection.begin() );
       // std::vector<record> v( mismatching_part.first, mismatching_part.second );
       std::vector<record> v( mismatch( selection(), begin( old_selection ) ):

       //                                                                                       
       // Example 3: specialized algorithm called for std::map
       //
       
       std::map<int,int> my_map;
       std::vector<int>  my_vector;
       // ...
       
       // my_map.find( 4 );
       find( my_map, 4 );
       // find( begin( my_vector ), end( my_vector ), 4 );
       find( my_vector, 4 );
                          
                       " ) );
                  
    $res .= $motivation . $advantages . $example;
                       
    return $res;
}



function reference()
{
    $res                   = beginSection( referenceLink() );
  
    $generalComments       = p( "The library is split up into several headers because
                              calling specialized algorithm for eg. " .  
                              code( "std::map::find()" ) . " and because it minimizes the
                              inclusion of other necessary headers. In total there are 
                              5 headers: " ) . 
        ulist( li( a( containerAlgoLink(), containerAlgoLink() ) . ": everything. " ) .
               li( a( nonModifyingLink(), nonModifyingLink() ) . ": non-modifying algorithms; includes " . 
                   code( "<set>" ) . " and " . code( "<map>" ) . "." ) );   

    $spezializedAlgorithms = ""; 
    $noModifying = "";
    $modifying = "";
    return $res . $generalComments . functionList();  
}



function portability()
{
    $res  = beginSection( portLink() );
    $res .= p( "The portability of the library depends totally on " . iteratorRangeLink() 
            . "." . "This means that you should at least expect everything to work with standard
            containers and pairs of iterators whereas array handling will be a problem on more challenged
            compilers." );
        
    return $res; 
}



function references()
{
    $res =  beginSection( refLink() );
    return $res;
}



function iteratorRangeLink()
{
    return a( "../../container_traits/doc/container_traits.html", code( "container_traits<>" ) );
}



function functionList()
{
    return pre( "
namespace boost 
{

/////////////////////////////////////////////////////////////////////////
// Nonmodifying Sequence Operations
/////////////////////////////////////////////////////////////////////////

template< typename Container, typename UnaryOperation >
inline UnaryOperation 
for_each( Container& c, UnaryOperation fun );

template< typename Container, typename UnaryOperation >
inline UnaryOperation 
for_each( const Container& c, UnaryOperation fun );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::iterator 
find( Container& c, const T& value );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::const_iterator 
find( const Container& c, const T& value );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::iterator 
find_if( Container& c, Predicate pred );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::const_iterator
find_if( const Container& c, Predicate pred );

template< typename Container >
inline typename iterator_range_traits<Container>::iterator
adjacent_find( Container& c );

template< typename Container >
inline typename iterator_range_traits<Container>::const_iterator
adjacent_find( const Container& c );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::iterator 
adjacent_find( Container& c, BinaryPredicate pred );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::const_iterator 
adjacent_find( const Container& c, BinaryPredicate pred );

template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::iterator 
find_first_of( Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::const_iterator 
find_first_of( const Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2, 
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::iterator 
find_first_of( Container1& c1, const Container2& c2, 
               BinaryPredicate pred );
template< typename Container1, typename Container2, 
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::const_iterator 
find_first_of( const Container1& c1, const Container2& c2, 
               BinaryPredicate pred );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::difference_type
count( const Container& c, const T& value );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::difference_type
count_if( const Container& c, Predicate pred );

template< typename Container, typename InputIterator >
inline std::pair< typename iterator_range_traits<Container>::iterator,
InputIterator >
mismatch( Container& c, InputIterator first2 );

template< typename Container, typename InputIterator >
inline std::pair< typename iterator_range_traits<Container>::const_iterator,
InputIterator >
mismatch( const Container& c, InputIterator first2 );

template< typename Container, typename InputIterator, 
typename BinaryPredicate >
inline std::pair< typename iterator_range_traits<Container>::iterator,
InputIterator >
mismatch( Container& c, InputIterator first2, BinaryPredicate pred );

template< typename Container, typename InputIterator, 
typename BinaryPredicate >
inline std::pair< typename iterator_range_traits<Container>::const_iterator, 
InputIterator >
mismatch( const Container& c, InputIterator first2, 
          BinaryPredicate pred );

template< typename Container, typename InputIterator >
inline bool 
equal( const Container& c, InputIterator first2 );

template< typename Container, typename InputIterator, 
typename BinaryPredicate > 
inline bool 
equal( const Container& c, InputIterator first2, 
       BinaryPredicate pred );
template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::iterator 
search( Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::const_iterator 
search( const Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::iterator
search( Container1& c1, const Container2& c2, BinaryPredicate pred );

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::const_iterator
search( const Container1& c1, const Container2& c2, BinaryPredicate pred );

template< typename Container, typename Integer, typename T >
inline typename iterator_range_traits<Container>::iterator 
search_n( Container& c, Integer count, const T& value );

template< typename Container, typename Integer, typename T >
inline typename iterator_range_traits<Container>::const_iterator 
search_n( const Container& c, Integer count, const T& value );

template< typename Container, typename Integer, 
typename T, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::iterator
search_n( Container& c, Integer count, const T& value,
          BinaryPredicate pred );
template< typename Container, typename Integer, 
typename T, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::const_iterator
search_n( const Container& c, Integer count, const T& value,
          BinaryPredicate pred );

template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::iterator 
find_end( Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2 >
inline typename iterator_range_traits<Container1>::const_iterator 
find_end( const Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::iterator 
find_end( Container1& c1, const Container2& c2, BinaryPredicate pred );

template< typename Container1, typename Container2,
typename BinaryPredicate >
inline typename iterator_range_traits<Container1>::const_iterator 
find_end( const Container1& c1, const Container2& c2, 
          BinaryPredicate pred );

/////////////////////////////////////////////////////////////////////////
// Modifying Sequance Operations
/////////////////////////////////////////////////////////////////////////

template< typename Container, typename OutputIterator >
inline OutputIterator
copy( const Container& c, OutputIterator result );

template< typename Container, typename BidirectionalIterator > 
inline BidirectionalIterator
copy_backward( const Container& c, BidirectionalIterator result ); 

template< typename Container, typename ForwardIterator >
inline ForwardIterator 
swap_ranges( Container& c, ForwardIterator first2 ); 

template< typename Container, typename OutputIterator, 
typename UnaryOperation > 
inline OutputIterator 
transform( const Container& c, OutputIterator result, 
           UnaryOperation fun );

template< typename Container, typename InputIterator,
typename OutputIterator, typename BinaryOperation >
inline OutputIterator
transform( const Container& c, InputIterator first2,
           OutputIterator result, BinaryOperation fun );

template< typename Container, typename T >
inline void
replace( Container& c, const T& what, const T& with_what );

template< typename Container, typename Predicate , typename T >
inline void
replace_if( Container& c, Predicate pred, const T& value );

template< typename Container, typename OutputIterator, typename T > 
inline OutputIterator
replace_copy( const Container& c, OutputIterator result, 
              const T& what, const T& with_what );

template< typename Container, typename OutputIterator, 
typename Predicate, typename T >
inline OutputIterator
replace_copy_if( const Container& c, OutputIterator result,
                 Predicate pred, const T& value );

template< typename Container, typename T >
inline void
fill( Container& c, const T& value );

template< typename Container, typename Integer, typename T >
inline void
fill_n( Container& c, Integer size, const T& value );

template< typename Container, typename Generator >
inline void
generate( Container& c, Generator gen );

template< typename Container, typename Integer, typename Generator >
void
generate_n( Container& c, Integer size, Generator gen );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::iterator
remove( Container& c, const T& what );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::iterator
remove_if( Container& c, Predicate pred );

template< typename Container, typename OutputIterator, typename T >
inline OutputIterator
remove_copy( const Container& c, OutputIterator result, const T& value );

template< typename Container, typename OutputIterator, typename Predicate >
inline OutputIterator
remove_copy_if( const Container& c, OutputIterator result, Predicate pred );

template< typename Container >
inline typename iterator_range_traits<Container>::iterator
unique( Container& c );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::iterator
unique( Container& c, BinaryPredicate pred );

template< typename Container, typename OutputIterator >
inline OutputIterator
unique_copy( const Container& c, OutputIterator result );

template< typename Container, typename OutputIterator, 
typename BinaryPredicate >
inline OutputIterator
unique_copy( const Container& c, OutputIterator result, 
             BinaryPredicate pred );

template< typename Container >
inline void
reverse( Container& c );

template< typename Container, typename OutputIterator >
inline OutputIterator
reverse_copy( const Container& c, OutputIterator result );

template< typename Container > 
inline void
rotate( Container& c, 
        typename iterator_range_traits<Container>::iterator middle );

template< typename Container, typename ForwardIterator, 
typename OutputIterator >
inline OutputIterator
rotate_copy( Container& c, ForwardIterator middle, OutputIterator result ); 

template< typename Container, typename ForwardIterator, 
typename OutputIterator >
inline OutputIterator
rotate_copy( const Container& c, ForwardIterator middle, 
             OutputIterator result ); 

template< typename Container >
inline void
random_shuffle( Container& c );

template< typename Container, typename Generator >
inline void
random_shuffle( Container& c, Generator gen );

/////////////////////////////////////////////////////////////////////////
// Sorted Sequences
/////////////////////////////////////////////////////////////////////////

template< typename Container >
inline void
sort( Container& c );

template< typename Container, typename Compare >
inline void
sort( Container& c, Compare comp );

template< typename Container >
inline void
stable_sort( Container& c );

template< typename Container, typename Compare >
inline void
stable_sort( Container& c, Compare comp );

template< typename Container, typename RandomAccessIterator >
inline void
partial_sort( Container& c, RandomAccessIterator middle );

template< typename Container, typename RandomAccessIterator, 
typename Compare >
inline void
partial_sort( Container& c, RandomAccessIterator middle,
              Compare comp );

template< typename Container1, typename InputIterator >
inline typename iterator_range_traits<InputIterator>::iterator
partial_sort_copy( const Container1& c1, InputIterator& c2 );

template< typename Container1, typename InputIterator, typename Compare >
inline typename iterator_range_traits<InputIterator>::iterator
partial_sort_copy( const Container1& c1, InputIterator& c2, Compare comp );

template< typename Container >
inline void
nth_element( Container& c, 
             typename iterator_range_traits<Container>::iterator nth );
template< typename Container, typename Compare >
inline void
nth_element( Container& c, 
             typename iterator_range_traits<Container>::iterator nth, 
             Compare comp );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::iterator
lower_bound( Container& c, const T& value );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::const_iterator
lower_bound( const Container& c, const T& value );

template< typename Container, typename T, typename Compare >
inline typename iterator_range_traits<Container>::iterator
lower_bound( Container& c, const T& value, Compare comp );

template< typename Container, typename T, typename Compare >
inline typename iterator_range_traits<Container>::const_iterator
lower_bound( const Container& c, const T& value, Compare comp );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::iterator
upper_bound( Container& c, const T& value );

template< typename Container, typename T >
inline typename iterator_range_traits<Container>::const_iterator
upper_bound( const Container& c, const T& value );

template< typename Container, typename T, typename Compare >
inline typename iterator_range_traits<Container>::iterator
upper_bound( Container& c, const T& value, Compare comp );

template< typename Container, typename T, typename Compare >
inline typename iterator_range_traits<Container>::const_iterator
upper_bound( const Container& c, const T& value, Compare comp );

template< typename Container, typename T >
inline typename pair_return<Container,Container>::mutable_pair 
equal_range( Container& c, const T& value );

template< typename Container, typename T >
inline typename pair_return<Container,Container>::const_pair 
equal_range( const Container& c, const T& value );

template< typename Container, typename T, typename Compare >
inline typename pair_return<Container,Container>::mutable_pair
equal_range( Container& c, const T& value, Compare comp );

template< typename Container, typename T, typename Compare >
inline typename pair_return<Container,Container>::const_pair
equal_range( const Container& c, const T& value, Compare comp );

template< typename Container, typename T >
inline bool
binary_search( const Container& c, const T& value );


template< typename Container, typename T, typename Compare >
inline bool
binary_search_( const Container& c, const T& value, Compare comp );

template< typename Container1, typename Container2, typename Container3 > 
inline typename iterator_range_traits<Container3>::iterator
merge( const Container1& c1, const Container2& c2, Container3& c3 );

template< typename Container1, typename Container2, typename Container3, 
typename Compare >
inline typename iterator_range_traits<Container3>::iterator
merge( const Container1& c1, const Container2& c2, Container3& c3,
       Compare comp );

template< typename Container >
inline void
inplace_merge( Container& c, 
               typename iterator_range_traits<Container>::iterator middle );

template< typename Container, typename Compare >
inline void
inplace_merge_( Container& c,
                typename iterator_range_traits<Container>::iterator middle, 
                Compare comp );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::iterator
partition( Container& c, Predicate pred );

template< typename Container, typename Predicate >
inline typename iterator_range_traits<Container>::iterator
stable_partition( Container& c, Predicate pred );

/////////////////////////////////////////////////////////////////////////
// Set Algorithms
/////////////////////////////////////////////////////////////////////////

template< typename Container1, typename Container2 >
inline bool 
includes( const Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2, typename Compare >
inline bool 
includes( const Container1& c1, const Container2& c2, Compare comp );

template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_union( const Container1& c1, const Container2& c2, 
           OutputIterator result );

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_union( const Container1& c1, const Container2& c2,
           OutputIterator result, Compare comp );

template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_intersection( const Container1& c1, const Container2& c2, 
                  OutputIterator result );

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare > 
inline OutputIterator
set_intersection( const Container1& c1, const Container2& c2, 
                  OutputIterator result, Compare comp );

template< typename Container1, typename Container2, 
typename OutputIterator > 
inline OutputIterator
set_difference( const Container1& c1, const Container2& c2,  
                OutputIterator result );

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_difference( const Container1& c1, const Container2& c2,  
                OutputIterator result, Compare comp );

template< typename Container1, typename Container2, 
typename OutputIterator >
inline OutputIterator
set_symmetric_difference( const Container1& c1, const Container2& c2,
                          OutputIterator result );

template< typename Container1, typename Container2, 
typename OutputIterator, typename Compare >
inline OutputIterator
set_symmetric_difference( const Container1& c1, const Container2& c2,
                          OutputIterator result, Compare comp );

///////////////////////////////////////////////////////////////////////////
// Heap Operations
///////////////////////////////////////////////////////////////////////////

template< typename Container >
inline void 
push_heap( Container& c );

template< typename Container, typename Compare >
inline void
push_heap( Container& c, Compare comp );

template< typename Container >
inline void 
pop_heap( Container& c );

template< typename Container, typename Compare >
inline void
pop_heap( Container& c, Compare comp );

template< typename Container >
inline void 
make_heap( Container& c );

template< typename Container, typename Compare >
inline void
make_heap( Container& c, Compare comp );

template< typename Container >
inline void 
sort_heap( Container& c );

template< typename Container, typename Compare >
inline void
sort_heap( Container& c, Compare comp );

/////////////////////////////////////////////////////////////////////////
// Minimum and Maximum
/////////////////////////////////////////////////////////////////////////

template< typename Container >
inline typename iterator_range_traits<Container>::iterator
min_element( Container& c );

template< typename Container >
inline typename iterator_range_traits<Container>::const_iterator
min_element( const Container& c );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::iterator
min_element( Container& c, BinaryPredicate pred );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::const_iterator
min_element( const Container& c, BinaryPredicate pred );

template< typename Container >
inline typename iterator_range_traits<Container>::iterator
max_element( Container& c );

template< typename Container >
inline typename iterator_range_traits<Container>::const_iterator
max_element( const Container& c );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::iterator
max_element( Container& c, BinaryPredicate pred );

template< typename Container, typename BinaryPredicate >
inline typename iterator_range_traits<Container>::const_iterator
max_element( const Container& c, BinaryPredicate pred );

template< typename Container1, typename Container2 >
inline bool
lexicographical_compare( const Container1& c1, const Container2& c2 );

template< typename Container1, typename Container2, 
typename BinaryPredicate > 
inline bool
lexicographical_compare( const Container1& c1, const Container2& c2, 
                         BinaryPredicate pred );

/////////////////////////////////////////////////////////////////////////
// Permutations
/////////////////////////////////////////////////////////////////////////

template< typename Container >
inline bool
next_permutation( Container& c );

template< typename Container, typename Compare >
inline bool
next_permutation( Container& c, Compare comp );

template< typename Container >
inline bool
prev_permutation( Container& c );

template< typename Container, typename Compare >
inline bool
prev_permutation( Container& c, Compare comp );

/////////////////////////////////////////////////////////////////////////
// Generalized Numeric Algorithms
/////////////////////////////////////////////////////////////////////////

template< typename Container, typename T >
inline T
accumulate( const Container& c, T initial );

template< typename Container, typename T, typename BinaryOperation >
inline T
accumulate( const Container& c, const T initial, BinaryOperation fun );

template< typename Container, typename InputIterator, typename T >
inline T                            
inner_product( const Container& c, InputIterator first2,
               const T initial );

template< typename Container, typename InputIterator, typename T,
typename BinaryOperation1, typename BinaryOperation2 >
inline T                            
inner_product( const Container& c, InputIterator first2,
               const T initial, BinaryOperation1 fun1, 
               BinaryOperation2 fun2 );

template< typename Container, typename OutputIterator >
inline OutputIterator
partial_sum( const Container& c, OutputIterator result );

template< typename Container, typename OutputIterator, 
typename BinaryOperation > 
inline OutputIterator
partial_sum_( const Container& c, OutputIterator result, 
              BinaryOperation fun );

template< typename Container, typename OutputIterator >
inline OutputIterator
adjacent_difference( const Container& c, OutputIterator result );

template<typename Container, typename OutputIterator, typename Predicate >
inline OutputIterator
adjacent_difference( const Container& c, OutputIterator result, 
                     Predicate pred );

} // namespace 'boost' " );

}

    

function stlLink1()
{
   return "http://www.cs.rpi.edu/projects/STL/htdocs/node13.html#stldatasheets";
}
    
    

function stlLink2()
{
   return "http://www.sgi.com/tech/stl/table_of_contents.html";
}



function containerAlgoLink()
{
   return "container_algo.hpp";
}



function nonModifyingLink()
{
    return "container_algo/non_modifying.hpp";
}

?>

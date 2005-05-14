
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <iostream>
#include <vector>

#include <boost/view/step_iterator.hpp>

#include <boost/tupple/iterator_tupple.hpp>

#include <boost/iterator/reverse_iterator.hpp>


void forward_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef std::vector<int>::iterator iter_type;
  boost::step_iterator<iter_type> it, 
                                  b1( v.begin(), 2 ),
                                  b2( v.begin()+1, 2 ),
                                  e( v.end(), 2 );

  BOOST_CHECK( (b1 - e) == boost::step_iterator<iter_type>::negative_infinite_distance() );
  BOOST_CHECK( (e - b1) == boost::step_iterator<iter_type>::infinite_distance() );
  BOOST_CHECK( !boost::is_reachable( b1, e ) );

  BOOST_CHECK( (e - b2) == 3 );
  BOOST_CHECK( boost::is_reachable( b2, e ) );

  // infinite loop
  /*for( it = b1; it != e; ++it )
    { std::cout << *it << " "; }
  std::cout << std::endl;*/

  iter_type vit;
  for( it = b2, vit = v.begin()+1; it != e; ++it, vit += 2 )
    { BOOST_CHECK( *it == *vit ); }

  for( it = b1, vit = v.begin(); it < e; ++it, vit += 2 )
    { BOOST_CHECK( *it == *vit ); }
}


void backward_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef std::vector<int>::iterator iter_type;
  boost::step_iterator<iter_type> it, 
                                  b1( v.end(), -2 ),
                                  b2( v.end()-1, -2 ),
                                  e( v.begin(), -2 );

  BOOST_CHECK( (b1 - e) == boost::step_iterator<iter_type>::negative_infinite_distance() );
  BOOST_CHECK( (e - b1) == boost::step_iterator<iter_type>::infinite_distance() );
  BOOST_CHECK( !boost::is_reachable( b1, e ) );

  BOOST_CHECK( (e - b2) == 3 );
  BOOST_CHECK( boost::is_reachable( b2, e ) );

  // infinite loop
  /*for( it = b1; it != e; ++it )
      { std::cout << *it << " "; }
  std::cout << std::endl;*/

  iter_type vit;
  boost::reverse_iterator<iter_type> rit;
  for( it = b2, rit = boost::make_reverse_iterator( v.end()-1 ), vit = (v.end()-1)-1; 
       it != e; ++it, ++++rit, vit -= 2 )
  { 
    BOOST_CHECK( *it == *rit ); 
    BOOST_CHECK( *it == *vit ); 
  }

  for( it = b1, rit = boost::make_reverse_iterator( v.end() ), vit = v.end()-1; 
       it < e; ++it, ++++rit, vit -= 2 )
  { 
    BOOST_CHECK( *it == *rit );
    BOOST_CHECK( *it == *vit ); 
  }
}


void tuple_step_test()
{
  typedef unsigned char PixelType; 

  typedef std::vector<PixelType> ImageType;
  typedef std::vector<PixelType>::iterator PixelIterator;

  int width = 11; int height = 9;
  int stride = width;
  ImageType image( width * height );

  // 4th approach:

  typedef boost::step_iterator<PixelIterator> RowIterator;
  typedef boost::tupple::iterator_tuple< RowIterator, RowIterator > RowType;

  PixelType value = 0;

  RowIterator imageBegin = boost::make_step_iterator( image.begin(), stride );
  RowIterator imageEnd   = boost::make_step_iterator( image.end(), stride );

  imageBegin = imageBegin + 2;

  RowType row( boost::make_step_iterator( image.begin(), stride ),
               boost::make_step_iterator( image.begin() + stride, stride ) );

  RowType endRow( boost::make_step_iterator( image.end(), stride ),
                  boost::make_step_iterator( image.end() + stride, stride ) );

  RowType::difference_type d = endRow - row;
  BOOST_CHECK( boost::is_reachable( row.front(), endRow.front() ) );
  
  for( ; row != endRow; ++row )
  {
    for( PixelIterator pixel = row.front(); 
         pixel != static_cast<PixelIterator>( row.back() ); ++pixel )
    {
      *pixel = value++;
    }
  }

  value = 0;
  for( PixelIterator it = image.begin(); it != image.end(); ++it )
  {
    BOOST_CHECK( *it == value++ );
  }

}


int test_main(int, char *[])
{
  forward_test();
  backward_test();

  tuple_step_test();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}

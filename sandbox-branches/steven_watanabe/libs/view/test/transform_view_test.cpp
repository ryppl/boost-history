
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <cmath>
#include <functional>
#include <algorithm>

#include <vector>
#include <map>
#include <string>

#include <boost/function.hpp>

#include <boost/view/transform_view.hpp>


const double eps = std::numeric_limits<double>::epsilon();


struct english2german: public std::unary_function<std::string,std::string>
{
  english2german()
  {
    dictionary[ std::string("hello") ]     = std::string("Hallo");
    dictionary[ std::string("world") ]     = std::string("Welt");
  }

  std::string operator()( const std::string& word ) const
  {
    std::map<std::string,std::string>::const_iterator i = dictionary.find( word );

    if( i != dictionary.end() )  { return (*i).second; }
    else                         { return word; }
  }

protected:
  std::map<std::string,std::string> dictionary;
};


void function_as_struct_test()
{
  std::vector<std::string> sentence;

  sentence.push_back( std::string("hello") );
  sentence.push_back( std::string("world") );

  typedef boost::view::transform_view< std::vector<std::string>, english2german > SatzT;
  SatzT satz( sentence );

  BOOST_CHECK( satz[0] == std::string("Hallo") );
  BOOST_CHECK( satz[1] == std::string("Welt") );
}


bool equals( double x, double y )
{
  return( fabs(x - y) <= eps );
}


void function_as_pointer_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N ); 

  typedef boost::view::transform_view< std::vector<int>, std::pointer_to_unary_function<double,double> > trans_view;
  typedef boost::view::transform_view< trans_view,       std::pointer_to_unary_function<double,double> > trans_trans_view;

#ifndef BOOST_MSVC
  trans_view absv( v, std::ptr_fun( fabs ) ); 
#else
  double (*doubleAbs)(double) = fabs; // Tell VC++ 7.0 which fabs to take to prevent an internal error.
  std::pointer_to_unary_function<double,double> absf( doubleAbs );
  trans_view absv( v, absf ); 
#endif

  BOOST_CHECK( v.size() == absv.size() );

  std::vector<int>::const_iterator it = v.begin(); trans_view::const_iterator absit = absv.begin();
  for( ; absit != absv.end(); ++it, ++absit )
    { BOOST_CHECK( equals( *absit, fabs( *it ) ) ); }
  

#ifndef BOOST_MSVC
  trans_trans_view sqrtv( absv, std::ptr_fun( sqrt ) );
#else
  double (*doubleSqrt)(double) = sqrt;
  std::pointer_to_unary_function<double,double> sqrtf( doubleSqrt );
  trans_trans_view sqrtv( absv, sqrtf );
#endif

  BOOST_CHECK( v.size() == sqrtv.size() );
  for( int i = 0; i < N; ++i )
    { BOOST_CHECK( equals( sqrtv[i], sqrt( fabs( v[i] ) ) ) ); }

}


int shiffle( int i ) { return i << 1; }
int shuffle( int i ) { return 1 << abs(i); }


void boost_function_test()
{
  int numbers[] = { -9, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::transform_view< std::vector<int>, boost::function1<int,int> > fct_view;

  boost::function1<int,int> f;
  f = shiffle;

  fct_view fview( v, f );

  for( int i = 0; i < N; ++i )
    { BOOST_CHECK( fview[i] == (v[i] << 1) ) ; }
}


double plus10percent( double x )
{
  return 1.1 * x;
}


double another10percent( std::pair< std::string, double > p )
{
  return 1.1 * p.second;
}


void map_test()
{
  std::map< std::string, double > stock;
  typedef std::pair< std::string, double > entry;

  stock.insert( entry( "microsoft", 24.95 ) );
  stock.insert( entry( "cisco", 13.4 ) );
  stock.insert( entry( "hp", 19.55 ) );

// Requires traits for index/data_type.
#if 0
  typedef boost::view::transform_view< std::map< std::string, double >, boost::function1<double,double> > value_view;

  BOOST_CHECK( typeid( value_view::index_type ) == typeid( std::map< std::string, double >::key_type ) );

  value_view value( stock, &plus10percent );

  BOOST_CHECK( equals( value[ std::string("microsoft") ], 1.1 * 24.95 ) );
  BOOST_CHECK( equals( value[ std::string("cisco") ], 1.1 * 13.4 ) );
  BOOST_CHECK( equals( value[ std::string("hp") ], 1.1 * 19.55 ) );

#endif

  typedef boost::view::transform_view< std::map< std::string, double >, boost::function1<double,entry> > anothervalue_view;
  anothervalue_view another( stock, &another10percent );

  std::map< std::string, double >::const_iterator it = stock.begin();
  anothervalue_view::const_iterator vit = another.begin();

  for( ; vit != another.end(); ++it, ++vit )
    { BOOST_CHECK( equals( *vit, 1.1 * (*it).second ) ); }
}


void parse_example()
{
  std::string s( "A sentence with five words." );

  // This does not work due to the slicing problem:
  //typedef transform_view< std::string, unary_function<char,bool> > parse_view;

  // Boost.Function does what we need; keep in mind the reversed order of template parameters.
  typedef boost::view::transform_view< std::string, boost::function1<bool,char> > parse_view;
  parse_view pview( s, std::bind2nd( std::equal_to<char>(), ' ' ) );

  std::vector< std::string > parsed;

  parse_view::const_iterator b = pview.begin();
  parse_view::const_iterator e;

  while( ( e = std::find( b, pview.end(), true ) ) != pview.end() )
  {
    parsed.push_back( std::string( s.begin() + (b-pview.begin()),
                                   s.begin() + (e-pview.begin()) ) );
    b = e + 1;
  }

  parsed.push_back( std::string( s.begin() + (b-pview.begin()),
                                 s.begin() + (e-pview.begin()) ) );

  BOOST_CHECK( parsed.size() == 5 );

  std::vector< std::string >::const_iterator it = parsed.begin();
  BOOST_CHECK( *it == "A" ); ++it;
  BOOST_CHECK( *it == "sentence" ); ++it;
  BOOST_CHECK( *it == "with" ); ++it;
  BOOST_CHECK( *it == "five" ); ++it;
  BOOST_CHECK( *it == "words." ); 
}



int test_main(int, char *[])
{
  function_as_struct_test();
  function_as_pointer_test();
  boost_function_test();
  map_test();
  parse_example();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}


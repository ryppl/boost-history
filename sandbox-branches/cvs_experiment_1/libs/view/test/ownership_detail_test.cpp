
#include <boost/config.hpp>

/* With my current MS VC++ 7.0 config, Boost.Test fails to compile,
   and I do not have the time to nail it down. Thus, VC++ tests are done
   with Boost 1.28.0.  - RR
*/

#ifdef BOOST_MSVC
#  define BOOST_INCLUDE_MAIN
#  include <boost/test/test_tools.hpp>
#else
#  include <boost/test/included/test_exec_monitor.hpp>
#endif


#include <vector>

#include <boost/view/detail/ownership_detail.hpp>


template<class U,class V> void compare_elementwise( const U& u, const V& v )
{
  BOOST_TEST( u.size() == v->size() );
  if( u.size() != v->size() ) return;

  typename U::const_iterator i = u.begin();
  typename V::element_type::const_iterator j = v->begin();

  for( ; i != u.end(); ++i, ++j )
  {
    BOOST_TEST( *i == *j );
  }
}


void typeid_test()
{
  typedef std::vector<char> container_type;

  BOOST_TEST( typeid( boost::view::ownership::wrap< container_type >::type )
              == typeid( boost::view::ownership::shared< container_type > ) );

  BOOST_TEST( typeid( boost::view::ownership::wrap< container_type >::domain )
              == typeid( container_type  ) );

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::unique< container_type > >::type )
              == typeid( boost::view::ownership::unique< container_type > ) );
  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::shared< container_type > >::type )
              == typeid( boost::view::ownership::shared< container_type > ) );
  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::external< container_type > >::type )
              == typeid( boost::view::ownership::external< container_type > ) );

  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::unique< container_type > >::domain )
              == typeid(  container_type  ));
  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::shared< container_type > >::domain )
              == typeid(  container_type  ) );
  BOOST_TEST( typeid( boost::view::ownership::wrap< boost::view::ownership::external< container_type > >::domain )
              == typeid( container_type ) );

#else
  //BOOST_ERROR( "Warning: Test is limited to shared ownership." );
#endif
}


void ownership_construction_test()
{
  typedef std::vector<char> container_type;
  container_type v(10,'*');

  boost::view::ownership::unique< container_type >   u(v);
  boost::view::ownership::shared< container_type >   s(v);
  boost::view::ownership::external< container_type > x(v);

  compare_elementwise( v, u );
  compare_elementwise( v, s );
  compare_elementwise( v, x );

  boost::view::ownership::unique< container_type >   uu(u);
  boost::view::ownership::unique< container_type >   us(s);
  boost::view::ownership::unique< container_type >   ux(x);

  compare_elementwise( v, uu );
  compare_elementwise( v, us );
  compare_elementwise( v, ux );

  boost::view::ownership::shared< container_type >   su(u);
  boost::view::ownership::shared< container_type >   ss(s);
  boost::view::ownership::shared< container_type >   sx(x);

  compare_elementwise( v, su );
  compare_elementwise( v, ss );
  compare_elementwise( v, sx );

  boost::view::ownership::external< container_type > xx(x);

  compare_elementwise( v, xx );
}


void ownership_usecount_test()
{
  typedef std::vector<char> container_type;
  container_type v(10,'*');

  boost::view::ownership::shared< container_type >   a(v);
  BOOST_TEST( a.use_count() == 1 );

  boost::view::ownership::shared< container_type >   b(a);
  BOOST_TEST( a.use_count() == 2 );
  BOOST_TEST( b.use_count() == 2 );

  boost::view::ownership::shared< boost::view::ownership::shared< container_type > >   aa(a);
  BOOST_TEST( aa.use_count() == 1 );
  BOOST_TEST( aa.get()->use_count() == 3 );
  BOOST_TEST( a.use_count() == 3 );
}



int test_main(int, char *[])
{
  typeid_test();

  ownership_construction_test();
  ownership_usecount_test();

  bool error_on_purpose = false;
  //BOOST_TEST( error_on_purpose );

  return 0;
}


#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <vector>

#include <boost/view/detail/ownership_detail.hpp>


template<class U,class V> void compare_elementwise( const U& u, const V& v )
{
  BOOST_CHECK( u.size() == v->size() );
  if( u.size() != v->size() ) return;

  typename U::const_iterator i = u.begin();
  typename V::element_type::const_iterator j = v->begin();

  for( ; i != u.end(); ++i, ++j )
  {
    BOOST_CHECK( *i == *j );
  }
}


void typeid_test()
{
  typedef std::vector<char> container_type;

  BOOST_CHECK( typeid( boost::view::ownership::wrap< container_type >::type )
              == typeid( boost::view::ownership::shared< container_type > ) );

  BOOST_CHECK( typeid( boost::view::ownership::wrap< container_type >::domain )
              == typeid( container_type  ) );

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::unique< container_type > >::type )
              == typeid( boost::view::ownership::unique< container_type > ) );
  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::shared< container_type > >::type )
              == typeid( boost::view::ownership::shared< container_type > ) );
  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::external< container_type > >::type )
              == typeid( boost::view::ownership::external< container_type > ) );

  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::unique< container_type > >::domain )
              == typeid(  container_type  ));
  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::shared< container_type > >::domain )
              == typeid(  container_type  ) );
  BOOST_CHECK( typeid( boost::view::ownership::wrap< boost::view::ownership::external< container_type > >::domain )
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
  BOOST_CHECK( a.use_count() == 1 );

  boost::view::ownership::shared< container_type >   b(a);
  BOOST_CHECK( a.use_count() == 2 );
  BOOST_CHECK( b.use_count() == 2 );

  boost::view::ownership::shared< boost::view::ownership::shared< container_type > >   bb(b);
  BOOST_CHECK( bb.use_count() == 1 );
  BOOST_CHECK( bb.get()->use_count() == 3 );
  BOOST_CHECK( b.use_count() == 3 );

  // Creates a shared container with its own new copy.
  boost::view::ownership::shared< container_type >   c(*a);
  BOOST_CHECK( c.use_count() == 1 );
  BOOST_CHECK( a.use_count() == 3 );

  a.swap( c );
  BOOST_CHECK( a.use_count() == 1 );
  BOOST_CHECK( c.use_count() == 3 );
}



int test_main(int, char *[])
{
  typeid_test();

  ownership_construction_test();
  ownership_usecount_test();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}

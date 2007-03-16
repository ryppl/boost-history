// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property/scalar_property.hpp>
#include <boost/property/object_property.hpp>
#include <boost/property/rank1_property.hpp>
#include <boost/property/rank1_object_property.hpp>
#include <boost/test/minimal.hpp>

class property_holder
{
   int a[ 10 ];
   char ch;
   public: // property char value;
      char get_value() const{ return ch; }
      void set_value( const char & c ){ ch = c; }
      boost::scalar_property< boost::object_property
      <
         char, property_holder,
         &property_holder::get_value,
         &property_holder::set_value
      > > value;
   public: // property int index[];
      int  get_index( int i ) const{ return a[ i ]; }
      void set_index( int i, const int & v ){ a[ i ] = v; }
      boost::rank1_property< boost::rank1_object_property
      <
         int, int, property_holder,
         &property_holder::get_index,
         &property_holder::set_index
      > > index;
   public:
      property_holder(): value( this ), index( this )
      {
      }
};

int test_main( int, char *[] )
{
   property_holder ob;

   ob.value = 'a';
   BOOST_TEST( ob.value == 'a' );

   ob.index[ 2 ] = 9;
   BOOST_TEST( ob.index[ 2 ] == 9 );

   return 0;
}

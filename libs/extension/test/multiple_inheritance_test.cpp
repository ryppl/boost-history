/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/extension/factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/convenience.hpp>

//  See the FAQ for info about why the following is necessary
//  here, but usually isn't.
#define BOOST_EXTENSION_VEHICLE_DECL BOOST_EXTENSION_IMPORT_DECL
#define BOOST_EXTENSION_COMPUTER_DECL BOOST_EXTENSION_IMPORT_DECL

#include "../examples/multiple_inheritance/vehicle.hpp"
#include "../examples/multiple_inheritance/computer.hpp"

#include <iostream>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>



using namespace boost::extensions;

BOOST_AUTO_TEST_CASE(multiple_inheritance_example)
{
  //  Create the factory_map object - it will hold all of the available
  //  constructors. Multiple zones can be constructed.
  factory_map twilight;

  // check if each library loads correct. It could be done with a helper function but I think that
  // it is clear this way because we are in a unit test, and the output for the helper function version
  // will be less descriptive

  // check if the libraries can be loaded
  shared_library libVehicle((std::string("libVehicle") + ".extension").c_str());
  shared_library libCar((std::string("libCar") + ".extension").c_str());
  shared_library libComputer((std::string("libComputer") + ".extension").c_str());
  shared_library libBoat((std::string("libBoat") + ".extension").c_str());
  shared_library libFlyingCar((std::string("libFlyingCar") + ".extension").c_str());
  shared_library libCarOfTheFuture((std::string("libCarOfTheFuture") + ".extension").c_str());
  shared_library libPlane((std::string("libPlane") + ".extension").c_str());
  BOOST_CHECK_EQUAL( libVehicle.open(), true );
  BOOST_CHECK_EQUAL( libCar.open(), true );
  BOOST_CHECK_EQUAL( libComputer.open(), true );
  BOOST_CHECK_EQUAL( libBoat.open(), true );
  BOOST_CHECK_EQUAL( libFlyingCar.open(), true );
  BOOST_CHECK_EQUAL( libCarOfTheFuture.open(), true );
  BOOST_CHECK_EQUAL( libPlane.open(), true );

  // check if the factory can return the functor for each library
  functor<void, factory_map &> load_func_vehicle = libVehicle.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_car = libCar.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_computer = libComputer.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_boat = libBoat.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_flyingcar = libFlyingCar.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_carofthefuture = libCarOfTheFuture.get_functor<void, factory_map &>("extension_export");
  functor<void, factory_map &> load_func_plane = libPlane.get_functor<void, factory_map &>("extension_export");
  BOOST_CHECK_EQUAL( load_func_vehicle.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_car.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_computer.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_boat.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_flyingcar.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_carofthefuture.is_valid(), true );
  BOOST_CHECK_EQUAL( load_func_plane.is_valid(), true );
  load_func_vehicle(twilight);
  load_func_car(twilight);
  load_func_computer(twilight);
  load_func_boat(twilight);
  load_func_flyingcar(twilight);
  load_func_carofthefuture(twilight);
  load_func_plane(twilight);


  // Computer test: we test if we obtain the computer implementation
  std::list<factory<computer, std::string> > & factory_list = twilight.get<computer, std::string>();  
  BOOST_CHECK_EQUAL( factory_list.size(), 2 );

  std::list<factory<computer, std::string> >::iterator comp = factory_list.begin();

  std::auto_ptr<computer> computer_ptr(comp->create());
  BOOST_CHECK_EQUAL( !computer_ptr.get(), 0 );

  BOOST_CHECK_EQUAL( comp->get_info(), "\nA computer exported as a computer" );
  BOOST_CHECK_EQUAL( computer_ptr->list_capabilities(), "\nIt computes.");

  // Vehicles test: we test if we obtain the different vehicles implementation
  std::list<factory<vehicle, std::string> > & factory_list2 = twilight.get<vehicle, std::string>();
  BOOST_CHECK_EQUAL( factory_list2.size(), 6 );

  std::list<factory<vehicle, std::string> >::iterator v = factory_list2.begin();

  // vehicle as a vehicle
  std::auto_ptr<vehicle> v1_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A vehicle exported as a vehicle" );
  BOOST_CHECK_EQUAL( v1_ptr->list_capabilities(), "\nIt is some sort of vehicle." );

  ++v;

  // car as a vehicle
  std::auto_ptr<vehicle> v2_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A car exported as a vehicle" );
  BOOST_CHECK_EQUAL( v2_ptr->list_capabilities(), "\nIt travels on roads." );

  ++v;

  // boat as a vehicle
  std::auto_ptr<vehicle> v3_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A boat exported as a vehicle" );
  BOOST_CHECK_EQUAL( v3_ptr->list_capabilities(), "\nIt floats on water." );

  ++v;

  // flying car as a vehicle
  std::auto_ptr<vehicle> v4_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A flying car exported as a vehicle");
  BOOST_CHECK_EQUAL( v4_ptr->list_capabilities(), "\nIt travels on roads.\nIt flies in the air.\nIt takes off from your driveway" );

  ++v;

  // a car of the future as a vehicle
  std::auto_ptr<vehicle> v5_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A car of the future exported as a vehicle" );
  BOOST_CHECK_EQUAL( v5_ptr->list_capabilities(), "\nIt floats on water.\nIt travels on roads.\nIt flies in the air.\n"
		     "It takes off from your driveway\nIt computes.\nCosts an arm and a leg" );

  ++v;

  // a plane as a vehicle
  std::auto_ptr<vehicle> v6_ptr(v->create());
  BOOST_CHECK_EQUAL( v->get_info(),  "A plane exported as a vehicle" );
  BOOST_CHECK_EQUAL( v6_ptr->list_capabilities(), "\nIt flies in the air.");
 
  // all tests done
}

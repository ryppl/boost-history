/* Boost stepper_euler.cpp test file

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky

 This file tests the use of the all different steppers with several state types:
 std::vector< double >
 vector_space_1d< double >  (see vector_space_1d.hpp)
 std::tr1::array< double , 1 >

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE odeint_stepper_with_units


#include <boost/test/unit_test.hpp>

#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/acceleration.hpp>
#include <boost/units/systems/si/io.hpp>

#include <boost/fusion/container.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/numeric/odeint/stepper/explicit_euler.hpp>
#include <boost/numeric/odeint/stepper/explicit_rk4.hpp>
#include <boost/numeric/odeint/stepper/explicit_error_rk54_ck.hpp>
#include <boost/numeric/odeint/stepper/explicit_error_dopri5.hpp>
#include <boost/numeric/odeint/stepper/controlled_error_stepper.hpp>
#include <boost/numeric/odeint/stepper/dense_output_explicit.hpp>
#include <boost/numeric/odeint/stepper/dense_output_controlled_explicit_fsal.hpp>
#include <boost/numeric/odeint/algebra/fusion_algebra.hpp>


using namespace boost::numeric::odeint;
using namespace boost::unit_test;
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace units = boost::units;
namespace si = boost::units::si;

typedef double value_type;
typedef units::quantity< si::time , value_type > time_type;
typedef units::quantity< si::length , value_type > length_type;
typedef units::quantity< si::velocity , value_type > velocity_type;
typedef units::quantity< si::acceleration , value_type > acceleration_type;
typedef fusion::vector< length_type , velocity_type > state_type;
typedef fusion::vector< velocity_type , acceleration_type > deriv_type;


void oscillator( const state_type &x , deriv_type &dxdt , time_type t )
{
	const units::quantity< si::frequency , value_type > omega = 1.0 * si::hertz;
	fusion::at_c< 0 >( dxdt ) = fusion::at_c< 1 >( x );
	fusion::at_c< 1 >( dxdt ) = - omega * omega * fusion::at_c< 0 >( x );
}

template< class Stepper >
void check_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
	typedef typename stepper_type::order_type order_type;
	typedef typename stepper_type::algebra_type algebra_type;
	typedef typename stepper_type::operations_type operations_type;

	const time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second );

	// test call method one
	stepper.do_step( oscillator , x , t , dt );

	// test call method two
	stepper.do_step( oscillator , x , t , x , dt );

	// test call method three
	deriv_type dxdt;
	oscillator( x , dxdt , t );
	stepper.do_step( oscillator , x , dxdt , t , dt );

	// test call method four
	oscillator( x , dxdt , t );
	stepper.do_step( oscillator , x , dxdt , t , x , dt );
}

template< class Stepper >
void check_fsal_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
	typedef typename stepper_type::order_type order_type;
	typedef typename stepper_type::algebra_type algebra_type;
	typedef typename stepper_type::operations_type operations_type;

	const time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second );

	// test call method one
	stepper.do_step( oscillator , x , t , dt );

	// test call method two
	stepper.do_step( oscillator , x , t , x , dt );

	// test call method three
	deriv_type dxdt;
	oscillator( x , dxdt , t );
	stepper.do_step( oscillator , x , dxdt , t , dt );

	// test call method four
	stepper.do_step( oscillator , x , dxdt , t , x , dxdt , dt );
}

template< class Stepper >
void check_error_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
	typedef typename stepper_type::order_type order_type;
	typedef typename stepper_type::algebra_type algebra_type;
	typedef typename stepper_type::operations_type operations_type;

	const time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second ) , xerr;

	// test call method one
	stepper.do_step( oscillator , x , t , dt , xerr );

	// test call method two
	stepper.do_step( oscillator , x , t , x , dt , xerr );

	// test call method three
	deriv_type dxdt;
	oscillator( x , dxdt , t );
	stepper.do_step( oscillator , x , dxdt , t , dt , xerr );

	// test call method four
	stepper.do_step( oscillator , x , dxdt , t , x , dt , xerr );
}

template< class Stepper >
void check_fsal_error_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
	typedef typename stepper_type::order_type order_type;
	typedef typename stepper_type::algebra_type algebra_type;
	typedef typename stepper_type::operations_type operations_type;

	const time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second ) , xerr;

	// test call method one
	stepper.do_step( oscillator , x , t , dt , xerr );

	// test call method two
	stepper.do_step( oscillator , x , t , x , dt , xerr );

	// test call method three
	deriv_type dxdt;
	oscillator( x , dxdt , t );
	stepper.do_step( oscillator , x , dxdt , t , dt , xerr );

	// test call method four
	stepper.do_step( oscillator , x , dxdt , t , x , dxdt , dt , xerr );
}

template< class Stepper >
void check_controlled_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
	typedef typename stepper_type::order_type order_type;
//	typedef typename stepper_type::algebra_type algebra_type;
//	typedef typename stepper_type::operations_type operations_type;

	time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second );

	// test call method one
	stepper.try_step( oscillator , x , t , dt );
}


template< class Stepper >
void check_dense_output_stepper( Stepper &stepper )
{
	typedef Stepper stepper_type;
	typedef typename stepper_type::state_type state_type;
	typedef typename stepper_type::value_type value_type;
	typedef typename stepper_type::deriv_type deriv_type;
	typedef typename stepper_type::time_type time_type;
//	typedef typename stepper_type::order_type order_type;

	time_type t( 0.0 * si::second );
	time_type dt( 0.1 * si::second );
	state_type x( 1.0 * si::meter , 0.0 * si::meter_per_second ) , x2;

	stepper.initialize( x , t , dt );
	stepper.do_step( oscillator );
	stepper.calc_state( dt / 2.0 , x2 );
}






class stepper_types : public mpl::vector
<
	explicit_euler< state_type , value_type , deriv_type , time_type , fusion_algebra > ,
	explicit_rk4< state_type , value_type , deriv_type , time_type , fusion_algebra > ,
	explicit_error_rk54_ck< state_type , value_type , deriv_type , time_type , fusion_algebra >
> { };

class fsal_stepper_types : public mpl::vector
<
	explicit_error_dopri5< state_type , value_type , deriv_type , time_type , fusion_algebra >
> { };

class error_stepper_types : public mpl::vector
<
	explicit_error_rk54_ck< state_type , value_type , deriv_type , time_type , fusion_algebra >
> { };

class fsal_error_stepper_types : public mpl::vector
<
	explicit_error_dopri5< state_type , value_type , deriv_type , time_type , fusion_algebra >
> { };

class controlled_stepper_types : public mpl::vector
<
	controlled_error_stepper< explicit_error_rk54_ck< state_type , value_type , deriv_type , time_type , fusion_algebra > > ,
	controlled_error_stepper< explicit_error_dopri5< state_type , value_type , deriv_type , time_type , fusion_algebra > >
> { };

class dense_output_stepper_types : public mpl::vector
<
	dense_output_explicit< explicit_euler< state_type , value_type , deriv_type , time_type , fusion_algebra > > ,
	dense_output_controlled_explicit_fsal<
		controlled_error_stepper< explicit_error_dopri5< state_type , value_type , deriv_type , time_type , fusion_algebra > > >
> { };




BOOST_AUTO_TEST_SUITE( stepper_with_units )

BOOST_AUTO_TEST_CASE_TEMPLATE( stepper_test , Stepper , stepper_types )
{
	Stepper stepper;
	check_stepper( stepper );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( fsl_stepper_test , Stepper , fsal_stepper_types )
{
	Stepper stepper;
	check_fsal_stepper( stepper );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( error_stepper_test , Stepper , error_stepper_types )
{
	Stepper stepper;
	check_error_stepper( stepper );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( fsal_error_stepper_test , Stepper , fsal_error_stepper_types )
{
	Stepper stepper;
	check_fsal_error_stepper( stepper );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( controlled_stepper_test , Stepper , controlled_stepper_types )
{
	Stepper stepper;
	check_controlled_stepper( stepper );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( dense_ouput_test , Stepper , dense_output_stepper_types )
{
	Stepper stepper;
	check_dense_output_stepper( stepper );
}


BOOST_AUTO_TEST_SUITE_END()

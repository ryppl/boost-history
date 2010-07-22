/*
 boost header: BOOST_NUMERIC_ODEINT/explicit_stepper_base.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED
#define BOOST_BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED


#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>

#include <boost/numeric/odeint/stepper/adjust_size.hpp>
#include <boost/numeric/odeint/algebra/standard_resize.hpp>




namespace boost {
namespace numeric {
namespace odeint {







/*
 * base class for explicit steppers
 * models the stepper concept
 */
template<
	class Stepper ,
	unsigned short Order ,
	class State ,
	class Time ,
	class Algebra ,
	class Operations ,
	class AdjustSizePolicy
>
class explicit_stepper_base
{
public:


	typedef State state_type;
	typedef Time time_type;
	typedef Algebra algebra_type;
	typedef Operations operations_type;
	typedef AdjustSizePolicy adjust_size_policy;
	typedef Stepper stepper_type;

	typedef explicit_stepper_base< Stepper , Order , State , Time , Algebra , Operations , AdjustSizePolicy > internal_stepper_base_type;

	typedef unsigned short order_type;
	static const order_type order_value = Order;

    order_type order( void ) const { return order_value; }


	explicit_stepper_base( void ) : m_size_adjuster() , m_dxdt()
	{
		m_size_adjuster.register_state( 0 , m_dxdt );
	}


    stepper_type& stepper( void ) { return *static_cast< stepper_type* >( this ); }
    const stepper_type& stepper( void ) const {return *static_cast< const stepper_type* >( this );}



	template< class System >
	void do_step( System system , state_type &x , time_type t , time_type dt )
	{
		m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		system( x , m_dxdt ,t );
		this->stepper().do_step_impl( system , x , m_dxdt , t , dt );
	}

	template< class System >
	void do_step( System system , state_type &x , const state_type dxdt , time_type t , time_type dt )
	{
		this->stepper().do_step_impl( system , x , dxdt , t , dt );
	}



	void adjust_size( const state_type &x )
	{
		m_size_adjuster.adjust_size( x );
	}


private:

	size_adjuster< state_type , 1 > m_size_adjuster;
	state_type m_dxdt;
};











/*
 * base class for explicit error steppers
 * models the error stepper concept
 * ToDo : test
 */
template<
	class ErrorStepper ,
	unsigned short StepperOrder ,
	unsigned short ErrorOrder ,
	class State ,
	class Time ,
	class Algebra ,
	class Operations ,
	class AdjustSizePolicy
>
class explicit_error_stepper_base
{
public:


	typedef State state_type;
	typedef Time time_type;
	typedef Algebra algebra_type;
	typedef Operations operations_type;
	typedef AdjustSizePolicy adjust_size_policy;
	typedef ErrorStepper stepper_type;

	typedef unsigned short order_type;
	static const order_type stepper_order_value = StepperOrder;
	static const order_type error_order_value = ErrorOrder;

    order_type stepper_order( void ) const { return stepper_order_value; }

    order_type error_order( void ) const { return error_order_value; }


	explicit_error_stepper_base( void ) : m_size_adjuster() , m_dxdt()
	{
		m_size_adjuster.register_state( 0 , m_dxdt );
	}


    stepper_type& stepper( void ) { return *static_cast< stepper_type* >( this ); }
    const stepper_type& stepper( void ) const {return *static_cast< const stepper_type* >( this );}



	template< class System >
	void do_step( System system , state_type &x , time_type t , time_type dt , state_type &xerr )
	{
		m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		system( x , m_dxdt ,t );
		this->stepper().do_step_impl( system , x , m_dxdt , t , dt , xerr );
	}

	template< class System >
	void do_step( System system , state_type &x , const state_type &dxdt , time_type t , time_type dt , state_type &xerr )
	{
		this->stepper().do_step_impl( system , x , dxdt , t , dt , xerr );
	}



	void adjust_size( const state_type &x )
	{
		m_size_adjuster.adjust_size( x );
	}


private:

	size_adjuster< state_type , 1 > m_size_adjuster;
	state_type m_dxdt;
};


















/*
 * base class for explicit stepper and error steppers
 * models the stepper AND the error stepper concept
 */
template<
	class Stepper ,
	unsigned short Order ,
	unsigned short StepperOrder ,
	unsigned short ErrorOrder ,
	class State ,
	class Time ,
	class Algebra ,
	class Operations ,
	class AdjustSizePolicy
>
class explicit_stepper_and_error_stepper_base
{
public:


	typedef State state_type;
	typedef Time time_type;
	typedef Algebra algebra_type;
	typedef Operations operations_type;
	typedef AdjustSizePolicy adjust_size_policy;
	typedef Stepper stepper_type;

	typedef unsigned short order_type;
	static const order_type order_value = Order;
	static const order_type stepper_order_value = StepperOrder;
	static const order_type error_order_value = ErrorOrder;

    order_type order( void ) const { return order_value; }
    order_type stepper_order( void ) const { return stepper_order_value; }
    order_type error_order( void ) const { return error_order_value; }


	explicit_stepper_and_error_stepper_base( void ) : m_size_adjuster() , m_dxdt()
	{
		m_size_adjuster.register_state( 0 , m_dxdt );
	}


    stepper_type& stepper( void ) { return *static_cast< stepper_type* >( this ); }
    const stepper_type& stepper( void ) const {return *static_cast< const stepper_type* >( this );}


	template< class System >
	void do_step( System system , state_type &x , time_type t , time_type dt )
	{
		m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		system( x , m_dxdt ,t );
		this->stepper().do_step_impl( system , x , m_dxdt , t , dt );
	}


	template< class System >
	void do_step( System system , state_type &x , time_type t , time_type dt , state_type &xerr )
	{
		m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		system( x , m_dxdt ,t );
		this->stepper().do_step_impl( system , x , m_dxdt , t , dt , xerr );
	}




	void adjust_size( const state_type &x )
	{
		m_size_adjuster.adjust_size( x );
	}


private:

	size_adjuster< state_type , 1 > m_size_adjuster;
	state_type m_dxdt;

};




} // odeint
} // numeric
} // boost


#endif //BOOST_BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_BASE_HPP_INCLUDED

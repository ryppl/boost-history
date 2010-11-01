/*
 boost header: numeric/odeint/explicit_stepper_and_error_stepper_fsal_base.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED

#include <boost/noncopyable.hpp>

#include <boost/numeric/odeint/stepper/adjust_size.hpp>
#include <boost/numeric/odeint/algebra/standard_resize.hpp>


namespace boost {
namespace numeric {
namespace odeint {

/*
 * base class for explicit stepper and error steppers with the fsal property
 * models the stepper AND the error stepper fsal concept
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
class explicit_stepper_and_error_stepper_fsal_base
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




    order_type order( void ) const
    {
    	return order_value;
    }

    order_type stepper_order( void ) const
    {
    	return stepper_order_value;
    }

    order_type error_order( void ) const
    {
    	return error_order_value;
    }




    explicit_stepper_and_error_stepper_fsal_base( void ) : m_size_adjuster() , m_dxdt() , m_first_call( true )
	{
		boost::numeric::odeint::construct( m_dxdt );
		m_size_adjuster.register_state( 0 , m_dxdt );
	}

	~explicit_stepper_and_error_stepper_fsal_base( void )
	{
		boost::numeric::odeint::destruct( m_dxdt );
	}





	// do_step( sys , x , t , dt )
	template< class System >
	void do_step( System &system , state_type &x , const time_type t , const time_type dt )
	{
	    m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
	    system( x , m_dxdt ,t );
		this->stepper().do_step_impl( system , x , m_dxdt , t , x , dt );
	}

	// do_step( sys , x , dxdt , t , dt )
	template< class System >
	void do_step( System &system , state_type &x , const state_type &dxdt , const time_type t , const time_type dt )
	{
		this->stepper().do_step_impl( system , x , dxdt , t , x , dt );
	}

	// do_step( sys , in , t , out , dt )
	template< class System >
	void do_step( System &system , const state_type &in , const time_type t , state_type &out , const time_type dt )
	{
	    m_size_adjuster.adjust_size_by_policy( in , adjust_size_policy() );
	    system( in , m_dxdt ,t );
		this->stepper().do_step_impl( system , in , m_dxdt , t , out , dt );
	}

	// do_step( sys , in , dxdt , t , out , dt )
	template< class System >
	void do_step( System &system , const state_type &in , const state_type &dxdt , const time_type t , state_type &out , const time_type dt )
	{
		this->stepper().do_step_impl( system , in , dxdt , t , out , dt );
	}






	// do_step( sys , x , t , dt , xerr )
	template< class System >
	void do_step( System &system , state_type &x , const time_type t , const time_type dt , state_type &xerr )
	{
	    if( m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() ) || m_first_call )
	    {
	        system( x , m_dxdt ,t );
	        m_first_call = false;
	    }
		this->stepper().do_step_impl( system , x , m_dxdt , t , x , dt , xerr );
	}

	// do_step( sys , x , dxdt , t , dt , xerr )
	template< class System >
	void do_step( System &system , state_type &x , state_type &dxdt , const time_type t , const time_type dt , state_type &xerr )
	{
		this->stepper().do_step_impl( system , x , dxdt , t , x , dt , xerr );
	}

	// do_step( sys , in , t , out , dt , xerr )
	template< class System >
	void do_step( System &system , const state_type &in , const time_type t , state_type &out , const time_type dt , state_type &xerr )
	{
	    if( m_size_adjuster.adjust_size_by_policy( in , adjust_size_policy() ) || m_first_call )
	    {
	        system( in , m_dxdt ,t );
	        m_first_call = false;
	    }
		this->stepper().do_step_impl( system , in , m_dxdt , t , out , dt , xerr );
	}

	// do_step( sys , in , dxdt , t , out , dt , xerr )
	template< class System >
	void do_step( System &system , const state_type &in , state_type &dxdt , const time_type t , state_type &out , const time_type dt , state_type &xerr )
	{
		this->stepper().do_step_impl( system , in , dxdt , t , out , dt , xerr );
	}



	void reset_step( state_type &dxdt )
	{
	    this->stepper().reset_step_impl( dxdt );
	}



	void adjust_size( const state_type &x )
	{
		if( m_size_adjuster.adjust_size( x ) )
		    m_first_call = true;
	}


private:

    stepper_type& stepper( void )
    {
    	return *static_cast< stepper_type* >( this );
    }

    const stepper_type& stepper( void ) const
    {
    	return *static_cast< const stepper_type* >( this );
    }


	size_adjuster< state_type , 1 > m_size_adjuster;
	state_type m_dxdt;
	bool m_first_call;

};


} // odeint
} // numeric
} // boost

#endif //BOOST_NUMERIC_ODEINT_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED

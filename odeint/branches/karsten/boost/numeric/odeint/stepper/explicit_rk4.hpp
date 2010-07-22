/*
 boost header: NUMERIC_ODEINT_STEPPER/explicit_rk4.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_RK4_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_RK4_HPP_INCLUDED


#include <boost/numeric/odeint/algebra/standard_algebra.hpp>
#include <boost/numeric/odeint/algebra/standard_operations.hpp>
#include <boost/numeric/odeint/algebra/standard_resize.hpp>

#include <boost/numeric/odeint/stepper/explicit_stepper_base.hpp>
#include <boost/numeric/odeint/stepper/detail/macros.hpp>

namespace boost {
namespace numeric {
namespace odeint {


template<
    class State ,
    class Time = double ,
	class Algebra = standard_algebra< State > ,
	class Operations = standard_operations< Time > ,
	class AdjustSizePolicy = adjust_size_initially_tag
	>
class explicit_rk4
: public explicit_stepper_base<
	  explicit_rk4< State , Time , Algebra , Operations , AdjustSizePolicy > ,
	  4 , State , Time , Algebra , Operations , AdjustSizePolicy >
{
public :


	BOOST_ODEINT_EXPLICIT_STEPPERS_TYPEDEFS( explicit_rk4 , 1 );


	explicit_rk4( void ) : m_size_adjuster() , m_dxt() , m_dxm() , m_dxh() , m_xt()
	{
		m_size_adjuster.register_state( 0 , m_dxt );
		m_size_adjuster.register_state( 1 , m_dxm );
		m_size_adjuster.register_state( 2 , m_dxh );
		m_size_adjuster.register_state( 3 , m_xt );
	}

	template< class System >
	void do_step_impl( System system , state_type &x , const state_type &dxdt , time_type t , time_type dt )
	{
		m_size_adjuster.adjust_size_by_policy( x , adjust_size_policy() );

        const time_type val1 = static_cast<time_type>( 1.0 );

        time_type  dh = static_cast<time_type>( 0.5 ) * dt;
        time_type th = t + dh;

        // dt * dxdt = k1
        // m_xt = x + dh*dxdt
        algebra_type::for_each3( m_xt , x , dxdt , typename operations_type::scale_sum2( val1 , dh ) );


        // dt * m_dxt = k2
        system( m_xt , m_dxt , th );

        // m_xt = x + dh*m_dxt
        algebra_type::for_each3( m_xt , x , m_dxt , typename operations_type::scale_sum2( val1 , dh ) );


        // dt * m_dxm = k3
        system( m_xt , m_dxm , th );
        //m_xt = x + dt*m_dxm
        algebra_type::for_each3( m_xt , x , m_dxm , typename operations_type::scale_sum2( val1 , dt ) );


        // dt * m_dxh = k4
        system( m_xt , m_dxh , t + dt );
        //x += dt/6 * ( m_dxdt + m_dxt + val2*m_dxm )
        time_type dt6 = dt / static_cast<time_type>( 6.0 );
        time_type dt3 = dt / static_cast<time_type>( 3.0 );
        algebra_type::for_each5( x , dxdt , m_dxt , m_dxm , m_dxh , typename operations_type::increment4( dt6 , dt3 , dt3 , dt6 ) );
	}


	void adjust_size( const state_type &x )
	{
		m_size_adjuster.adjust_size( x );
		stepper_base_type::adjust_size( x );
	}


private:

	size_adjuster< state_type , 4 > m_size_adjuster;

    state_type m_dxt;
    state_type m_dxm;
    state_type m_dxh;
    state_type m_xt;

};




} // odeint
} // numeric
} // boost


#endif //BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_RK4_HPP_INCLUDED

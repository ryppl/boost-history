/*
 boost header: BOOST_NUMERIC_ODEINT/implicit_euler.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_BOOST_NUMERIC_ODEINT_IMPLICIT_EULER_HPP_INCLUDED
#define BOOST_BOOST_NUMERIC_ODEINT_IMPLICIT_EULER_HPP_INCLUDED

#include <utility>

#include <boost/ref.hpp>

#include <boost/numeric/odeint/stepper/stepper_categories.hpp>

#include <boost/numeric/odeint/util/size_adjuster.hpp>
#include <boost/numeric/odeint/util/matrix_vector_adjust_size.hpp>
#include <boost/numeric/odeint/util/ublas_resize.hpp>
#include <boost/numeric/odeint/util/ublas_permutation_matrix_resize.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace boost {
namespace numeric {
namespace odeint {








template< class ValueType , class AdjustSizePolicy = adjust_size_initially_tag >
class implicit_euler
{
	void initialize( void )
	{
		m_state_adjuster.register_state( 0 , m_dxdt );
		m_state_adjuster.register_state( 1 , m_x );
		m_state_adjuster.register_state( 2 , m_b );
		m_matrix_adjuster.register_state( 0 , m_jacobi );
		m_pmatrix_adjuster.register_state( 0 , m_pm );

	}

	void copy( const implicit_euler &euler )
	{
		m_dxdt = euler.m_dxdt;
		m_x = euler.m_x;
		m_b = euler.m_b;
		m_jacobi = euler.m_jacobi;
		m_pm = euler.m_pm;
		m_epsilon = euler.m_epsilon;
	}

public:

    typedef ValueType value_type;
    typedef value_type time_type;
    typedef boost::numeric::ublas::vector< value_type > state_type;
    typedef state_type deriv_type;
    typedef boost::numeric::ublas::matrix< value_type > matrix_type;
    typedef boost::numeric::ublas::permutation_matrix< size_t > pmatrix_type;
    typedef AdjustSizePolicy adjust_size_policy;
	typedef stepper_tag stepper_category;

    implicit_euler( const value_type epsilon = 1E-6 )
    : m_epsilon( epsilon ) ,
      m_state_adjuster() , m_matrix_adjuster() , m_pmatrix_adjuster() ,
      m_dxdt() , m_x() , m_b() ,
      m_jacobi() , m_pm( 1 )
    {
    	initialize();
    }

    implicit_euler( const implicit_euler &euler )
    : m_epsilon( 1.0e-6 ) ,
      m_state_adjuster() , m_matrix_adjuster() , m_pmatrix_adjuster() ,
      m_dxdt() , m_x() , m_b() ,
      m_jacobi() , m_pm( 1 )
    {
    	initialize();
    	copy( euler );
    }

    implicit_euler& operator=( const implicit_euler &euler )
    {
    	copy( euler );
    	return *this;
    }

    template< class System >
    void do_step( System system , state_type &x , value_type t , value_type dt )
    {
    	typedef typename boost::unwrap_reference< System >::type system_type;
    	typedef typename boost::unwrap_reference< typename system_type::first_type >::type deriv_func_type;
    	typedef typename boost::unwrap_reference< typename system_type::second_type >::type jacobi_func_type;
    	system_type &sys = system;
    	deriv_func_type &deriv_func = sys.first;
    	jacobi_func_type &jacobi_func = sys.second;

		m_state_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		m_matrix_adjuster.adjust_size_by_policy( x , adjust_size_policy() );
		m_pmatrix_adjuster.adjust_size_by_policy( x , adjust_size_policy() );

		for( size_t i=0 ; i<x.size() ; ++i )
			m_pm[i] = i;

        t += dt;

        // apply first Newton step
        deriv_func( x , m_dxdt , t );

        m_b = dt * m_dxdt;

        jacobi_func( x , m_jacobi  , t );
        m_jacobi *= dt;
        m_jacobi -= boost::numeric::ublas::identity_matrix< value_type >( x.size() );

        solve( m_b , m_jacobi );

        m_x = x - m_b;

        // iterate Newton until some precision is reached
        // ToDo: maybe we should apply only one Newton step -> linear implicit one-step scheme
        while( boost::numeric::ublas::norm_2( m_b ) > m_epsilon )
        {
            deriv_func( m_x , m_dxdt , t );
            m_b = x - m_x + dt*m_dxdt;

            // simplified version, only the first Jacobian is used
//            jacobi( m_x , m_jacobi , t );
//            m_jacobi *= dt;
//            m_jacobi -= boost::numeric::ublas::identity_matrix< value_type >( x.size() );

            solve( m_b , m_jacobi );

            m_x -= m_b;
        }
        x = m_x;
    }


	template< class StateType >
	void adjust_size( const StateType &x )
	{
		m_state_adjuster.adjust_size( x );
		m_matrix_adjuster.adjust_size( x );
		m_pmatrix_adjuster.adjust_size( x );
	}


private:

    void solve( state_type &x , matrix_type &m )
    {
        int res = boost::numeric::ublas::lu_factorize( m , m_pm );
        if( res != 0 ) exit(0);
        boost::numeric::ublas::lu_substitute( m , m_pm , x );
    }

private:

    value_type m_epsilon;
    size_adjuster< state_type , 3 > m_state_adjuster;
    size_adjuster< matrix_type , 1 , matrix_vector_adjust_size > m_matrix_adjuster;
    size_adjuster< pmatrix_type , 1 > m_pmatrix_adjuster;
    state_type m_dxdt;
    state_type m_x;
    state_type m_b;
    matrix_type m_jacobi;
    pmatrix_type m_pm;


};


} // odeint
} // numeric
} // boost


#endif //BOOST_BOOST_NUMERIC_ODEINT_IMPLICIT_EULER_HPP_INCLUDED

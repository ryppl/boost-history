/*
 boost header: NUMERIC_ODEINT/adjust_size.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_NUMERIC_ODEINT_ADJUST_SIZE_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_ADJUST_SIZE_HPP_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_array.hpp>

#include <boost/numeric/odeint/algebra/standard_resize.hpp>


namespace boost {
namespace numeric {
namespace odeint {



/*
 * Tags to specify resize behavior of steppers
 */
struct adjust_size_manually_tag {};
struct adjust_size_initially_tag {};
struct adjust_size_always_tag {};






/*
 * Adjust size functionality with policies and resizeability
 */
template< class State , size_t Dim >
class size_adjuster : boost::noncopyable
{
public:

	size_adjuster() : m_is_initialized( false ) { }

	bool adjust_size( const State &x )
	{
		return adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
	}

	bool adjust_size_by_policy( const State &x , adjust_size_manually_tag )
	{
	    return false;
	}

	bool adjust_size_by_policy( const State &x , adjust_size_initially_tag )
	{
		if( !m_is_initialized )
		{
			m_is_initialized = true;
			return adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
		} else
		    return false;
	}

	bool adjust_size_by_policy( const State &x , adjust_size_always_tag )
	{
		return adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
	}

	void register_state( size_t idx , State &x )
	{
		m_states.replace( idx , &x );
	}


private:

	bool adjust_size_by_resizeability( const State &x , boost::true_type )
	{
	    bool changed = false;
		for( size_t i=0 ; i<Dim ; ++i )
		{
            boost::numeric::odeint::adjust_size( x , m_states[i] );
            changed = true;
		}
		return changed;
	}

	bool adjust_size_by_resizeability( const State &x , boost::false_type )
	{
	    return false;
	}


private :

	bool m_is_initialized;
	boost::ptr_array< State , Dim ,  boost::view_clone_allocator > m_states;
};









//template< class State , class AdjustSizeImpl >
//class size_adjuster
//{
//public:
//
//	size_adjuster( AdjustSizeImpl &adjust_size_impl ) : m_is_initialized( false ) , m_adjust_size_impl( adjust_size_impl ) { }
//
//	void adjust_size( const State &x )
//	{
//		adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
//	}
//
//	void adjust_size_by_policy( const State &x , adjust_size_manually_tag )
//	{
//	}
//
//	void adjust_size_by_policy( const State &x , adjust_size_initially_tag )
//	{
//		if( !m_is_initialized )
//		{
//			adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
//			m_is_initialized = true;
//		}
//	}
//
//	void adjust_size_by_policy( const State &x , adjust_size_always_tag )
//	{
//		adjust_size_by_resizeability( x , typename is_resizeable< State >::type() );
//	}
//
//
//private:
//
//
//	void adjust_size_by_resizeability( const State &x , boost::true_type )
//	{
//		m_adjust_size_impl( x );
//	}
//
//	void adjust_size_by_resizeability( const State &x , boost::false_type )
//	{
//	}
//
//
//private :
//
//	bool m_is_initialized;
//	AdjustSizeImpl &m_adjust_size_impl;
//};


} // odeint
} // numeric
} // boost


#endif //BOOST_NUMERIC_ODEINT_ADJUST_SIZE_HPP_INCLUDED

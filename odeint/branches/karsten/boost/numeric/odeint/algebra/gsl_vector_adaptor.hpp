/*
 boost header: xyz/gsl_vector_adaptor.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky
 Copyright 2009 Andre Bergner

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_NUMERIC_ODEINT_GSL_VECTOR_ADAPTOR_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_GSL_VECTOR_ADAPTOR_HPP_INCLUDED

#include <gsl/gsl_vector.h>
#include <boost/range.hpp>

#include <boost/iterator/iterator_facade.hpp>

namespace boost {
namespace numeric {
namespace odeint {


class const_gsl_vector_iterator;

/*
 * defines an iterator for gsl_vector
 */
class gsl_vector_iterator : public boost::iterator_facade< gsl_vector_iterator , double , boost::random_access_traversal_tag >
{
public :

	gsl_vector_iterator( void ): m_p(0) , m_stride( 0 ) { }
	explicit gsl_vector_iterator( gsl_vector &p ) : m_p( p.data ) , m_stride( p.stride ) { }
	friend gsl_vector_iterator end_iterator( gsl_vector & );

private :

	friend class boost::iterator_core_access;
	friend class const_gsl_vector_iterator;

	void increment( void ) { m_p += m_stride; }
	void decrement( void ) { m_p -= m_stride; }
	void advance( ptrdiff_t n ) { m_p += n*m_stride; }
	bool equal( const gsl_vector_iterator &other ) const { return this->m_p == other.m_p; }
	bool equal( const const_gsl_vector_iterator &other ) const;
	double& dereference( void ) const { return *m_p; }

	double *m_p;
	size_t m_stride;
};



/*
 * defines an const iterator for gsl_vector
 */
class const_gsl_vector_iterator : public boost::iterator_facade< const_gsl_vector_iterator , double , boost::random_access_traversal_tag >
{
public :

	const_gsl_vector_iterator( void ): m_p(0) , m_stride( 0 ) { }
	explicit const_gsl_vector_iterator( const gsl_vector &p ) : m_p( p.data ) , m_stride( p.stride ) { }
	const_gsl_vector_iterator( const gsl_vector_iterator &p ) : m_p( p.m_p ) , m_stride( p.m_stride ) { }

private :

	friend class boost::iterator_core_access;
	friend class gsl_vector_iterator;
	friend const_gsl_vector_iterator end_iterator( const gsl_vector & );

	void increment( void ) { m_p += m_stride; }
	void decrement( void ) { m_p -= m_stride; }
	void advance( ptrdiff_t n ) { m_p += n*m_stride; }
	bool equal( const const_gsl_vector_iterator &other ) const { return this->m_p == other.m_p; }
	bool equal( const gsl_vector_iterator &other ) const { return this->m_p == other.m_p; }
	const double& dereference( void ) const { return *m_p; }

	const double *m_p;
	size_t m_stride;
};


bool gsl_vector_iterator::equal( const const_gsl_vector_iterator &other ) const { return this->m_p == other.m_p; }


gsl_vector_iterator end_iterator( gsl_vector &x )
{
	gsl_vector_iterator iter( x );
	iter.m_p += iter.m_stride * x.size;
	return iter;
}

const_gsl_vector_iterator end_iterator( const gsl_vector &x )
{
	const_gsl_vector_iterator iter( x );
	iter.m_p += iter.m_stride * x.size;
	return iter;
}


} // namespace odeint
} // namespace numeric
} // namespace boost


namespace boost
{
	template<>
	struct range_mutable_iterator< gsl_vector >
	{
		typedef boost::numeric::odeint::gsl_vector_iterator type;
	};

	template<>
	struct range_const_iterator< gsl_vector >
	{
		typedef boost::numeric::odeint::const_gsl_vector_iterator type;
	};
} // namespace boost



namespace boost {
namespace numeric {
namespace odeint {


inline gsl_vector_iterator range_begin( gsl_vector &x )
{
	return gsl_vector_iterator( x );
}

inline const_gsl_vector_iterator range_begin( const gsl_vector &x )
{
	return const_gsl_vector_iterator( x );
}

inline gsl_vector_iterator range_end( gsl_vector &x )
{
	return end_iterator( x );
}

inline const_gsl_vector_iterator range_end( const gsl_vector &x )
{
	return end_iterator( x );
}


} // namespace odeint
} // namespace numeric
} // namespace boost






#endif // BOOST_NUMERIC_ODEINT_GSL_VECTOR_ADAPTOR_HPP_INCLUDED

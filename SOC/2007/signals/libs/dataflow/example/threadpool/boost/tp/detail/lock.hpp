// Copyright Howard Hinnant 2007. Distributed under the Boost // Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_TP_DETAIL_LOCK_H
#define BOOST_TP_DETAIL_LOCK_H

#include <boost/thread.hpp>

namespace boost { namespace tp {
namespace detail
{
// returns index of failed lock or -1 on success
template< class L1, class L2 >
int try_lock( L1 & l1, L2 & l2)
{
	unique_lock< L1 > u( l1, try_to_lock);
	if ( u.owns_lock())
	{
		if ( l2.try_lock() )
		{
			u.release();
			return -1;
		}
		return 1;
	}
	return 0;
}

template< class L1, class L2, class L3 >
int try_lock( L1 & l1, L2 & l2, L3 & l3)
{
	unsigned int r = 0;
	unique_lock< L1 > u( l1, try_to_lock);
	if ( u.owns_lock() )
	{
		r = try_lock( l2, l3);
		if ( r == -1)
			u.release();
		else
			++r;
	}
	return r;
}

template< class L1, class L2, class L3, class L4 >
int try_lock( L1 & l1, L2 & l2, L3 & l3, L4 & l4)
{
	unsigned int r = 0;
	unique_lock< L1 > u( l1, try_to_lock);
	if ( u.owns_lock() )
	{
		r = try_lock( l2, l3, l4);
		if ( r == -1)
			u.release();
		else
			++r;
	}
	return r;
}

template< class L1, class L2, class L3, class L4, class L5 >
int try_lock( L1 & l1, L2 & l2, L3 & l3, L4 & l4, L5 & l5)
{
	unsigned int r = 0;
	unique_lock< L1 > u( l1, try_to_lock);
	if ( u.owns_lock() )
	{
		r = try_lock( l2, l3, l4, l5);
		if ( r == -1)
			u.release();
		else
			++r;
	}
	return r;
}

template< class L1, class L2, class L3 >
void __lock_first( int __i, L1 & l1, L2 & l2, L3 & l3)
{
	while ( true)
	{
		switch ( __i)
		{
		case 0:
			{
				unique_lock< L1 > __u1( l1);
				__i = try_lock( l2, l3);
				if ( __i == -1)
				{
					__u1.release();
					return;
				}
			}
			++__i;
			boost::thread::yield();
			break;
		case 1:
			{
				unique_lock< L2 > __u2( l2);
				__i = try_lock( l3, l1);
				if ( __i == -1)
				{
					__u2.release();
					return;
				}
			}
			if ( __i == sizeof( L3) )
				__i = 0;
			else
				__i += 2;
			boost::thread::yield();
			break;
		default:
			__lock_first( __i - 2, l3, l1, l2);
			return;
		}
	}
}

template< class L1, class L2 >
void lock( L1 & l1, L2 & l2)
{
	while ( true)
	{
		{
			unique_lock< L1 > __u1( l1);
			if ( l2.try_lock() )
			{
				__u1.release();
				break;
			}
		}
		boost::thread::yield();

		{
			unique_lock< L2 > __u2( l2);
			if ( l1.try_lock())
			{
				__u2.release();
				break;
			}
		}
		boost::thread::yield();
	}
}

template< class L1, class L2, class L3 >
void lock( L1 & l1, L2 & l2, L3 & l3)
{ __lock_first( 0, l1, l2, l3); }

} }
}

#endif // BOOST_TP_DETAIL_LOCK_H

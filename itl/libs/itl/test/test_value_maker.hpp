/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_test_value_maker_JOFA_080916_H__
#define __itl_test_value_maker_JOFA_080916_H__

#include <boost/itl/type_traits/neutron.hpp>

namespace boost{ namespace itl
{

/*JODO completion like that:
std::string make(int n)
{
	std::string value = neutron<std::string>::value();
	int abs_n = n<0 ? -n : n;
	for(int i=1; i<abs_n; i++)
		value += i%2==1 ? "hello " : "world ";
}
*/

template <class BicrementableT>
BicrementableT make(int n)
{
    BicrementableT value = neutron<BicrementableT>::value();
    if(n>=0)
        for(int i=0; i<n; i++)
            ++value;
    else
        for(int i=0; i>n; i--)
            --value;

    return value;
}


template <class Type>
struct test_value;

template<>
struct test_value<std::string>
{
	static std::string make(int n)
	{
		std::string value = neutron<std::string>::value();
		int abs_n = n<0 ? -n : n;
		for(int i=1; i<abs_n; i++)
			value += (i%2==1 ? "hello " : "world ");
		
		return value;
	}
};

template <class Type>
struct test_value
{ 
	static Type make(int n)
	{
		Type value = neutron<Type>::value();
		if(n>=0)
			for(int i=0; i<n; i++)
				++value;
		else
			for(int i=0; i>n; i--)
				--value;

		return value;
	}
};


template <class ItvMapT>
struct map_val
{
	typedef typename ItvMapT::domain_type       domain_type;
	typedef typename ItvMapT::codomain_type     codomain_type;
	typedef typename ItvMapT::interval_type     interval_type;
	typedef typename ItvMapT::value_type        value_type;
	typedef typename ItvMapT::domain_mapping_type domain_mapping_type;

	static interval<domain_type> interval(int lower, int upper, int bounds = 2)
	{
		return interval_type(test_value<domain_type>::make(lower), 
			                 test_value<domain_type>::make(upper), bounds);
	}

	static value_type val_pair(int lower, int upper, int val, int bounds = 2)
	{
		return value_type( interval(lower, upper, bounds), 
			               test_value<codomain_type>::make(val) );
	}

	static domain_mapping_type map_pair(int key, int val)
	{
		return domain_mapping_type(test_value<domain_type>::make(key), 
			                     test_value<codomain_type>::make(val));
	}
};

#define IIv(low,up,val) map_val<IntervalMapT>::val_pair(low,up,val, closed_bounded)
#define IDv(low,up,val) map_val<IntervalMapT>::val_pair(low,up,val, right_open)
#define CIv(low,up,val) map_val<IntervalMapT>::val_pair(low,up,val, left_open)
#define CDv(low,up,val) map_val<IntervalMapT>::val_pair(low,up,val, open_bounded)
#define K_v(key,val)    map_val<IntervalMapT>::map_pair(key,val)

}} // namespace boost itl

#endif 


/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_interval_set_algo_JOFA_081005_H__
#define __itl_interval_set_algo_JOFA_081005_H__

#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/notate.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/detail/element_comparer.hpp>
#include <boost/itl/detail/subset_comparer.hpp>

namespace boost{namespace itl
{

template <class IntervalContainerT>
typename IntervalContainerT::size_type continuous_cardinality(const IntervalContainerT& object)
{
    typedef typename IntervalContainerT::size_type size_type;
    typedef typename IntervalContainerT::interval_type interval_type;

    size_type size = neutron<size_type>::value();
    size_type interval_size;
    const_FORALL(typename IntervalContainerT, it, object)
    {
		interval_size = continuous_interval<interval_type>::cardinality(IntervalContainerT::key_value(it));
        if(interval_size == std::numeric_limits<size_type>::infinity())
            return interval_size;
        else
            size += interval_size;
    }
    return size;
}

template <class IntervalContainerT>
typename IntervalContainerT::size_type discrete_cardinality(const IntervalContainerT& object)
{
    typedef typename IntervalContainerT::size_type size_type;
    typedef typename IntervalContainerT::interval_type interval_type;

    size_type size = neutron<size_type>::value();
    const_FORALL(typename IntervalContainerT, it, object)
		size += discrete_interval<interval_type>::cardinality(IntervalContainerT::key_value(it));
    return size;
}

struct continuous_interval_container
{
    template<class IntervalContainerT> 
    static typename IntervalContainerT::size_type 
        cardinality(const IntervalContainerT& cont) 
    { return continuous_cardinality(cont); }
};

struct discrete_interval_container
{
    template<class IntervalContainerT> 
    static typename IntervalContainerT::size_type 
        cardinality(const IntervalContainerT& cont) 
    { return discrete_cardinality(cont); }
};


namespace Interval_Set
{

//------------------------------------------------------------------------------
// Lexicographical comparison on ranges of two interval container 
//------------------------------------------------------------------------------

template<class LeftT, class RightT>
bool is_element_equal(const LeftT& left, const RightT& right)
{
	return subset_compare
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			) == inclusion::equal;
}

template<class LeftT, class RightT>
bool is_element_less(const LeftT& left, const RightT& right)
{
	return element_compare
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			)  == comparison::less;
}

template<class LeftT, class RightT>
bool is_element_greater(const LeftT& left, const RightT& right)
{
	return element_compare
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			)  == comparison::greater;
}

//------------------------------------------------------------------------------
// Subset/superset compare on ranges of two interval container 
//------------------------------------------------------------------------------

template<class LeftT, class RightT>
bool is_inclusion_equal(const LeftT& left, const RightT& right)
{
	return subset_compare
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			) == inclusion::equal;
}

template<class LeftT, class RightT>
bool is_contained_in(const LeftT& left, const RightT& right)
{
	int result =
		subset_compare
	    (
			left, right, 
			left.begin(), left.end(), 
			right.begin(), right.end()
		);
	return result == inclusion::subset || result == inclusion::equal;
}

template<class LeftT, class RightT>
bool contains(const LeftT& left, const RightT& right)
{
	int result =
		subset_compare
	    (
			left, right, 
			left.begin(), left.end(), 
			right.begin(), right.end()
		);
	return result == inclusion::superset || result == inclusion::equal;
}

} // namespace Interval_Set
    
}} // namespace itl boost

#endif 


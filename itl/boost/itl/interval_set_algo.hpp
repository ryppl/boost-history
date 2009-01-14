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

template<class LeftT, class RightT>
class interval_sequence_tracker
{
public:
    typedef typename LeftT::const_iterator  LeftIterT;
    typedef typename RightT::const_iterator RightIterT;

    interval_sequence_tracker(const LeftT&      left,
		                      const RightT&     right,
		                      const LeftIterT&  left_end,
		                      const RightIterT& right_end)
        : _left(left), _right(right),
	      _left_end(left_end), _right_end(right_end), 
		  _compare_codomain(false), _result(result_is_equal)
    {
		_scope = enclosure(_left) & enclosure(_right);
	}

    enum{firstboth, nextboth, nextleft, nextright, leftaligned, stop};
	enum{result_is_less = -1, result_is_equal = 0, result_is_greater = 1};

	void set_compare_codomain(bool truth=true)
	{ _compare_codomain = truth; }

	bool compare_codomain()const { return _compare_codomain; }

    int result()const{ return _result; }

	bool covalues_are_equal(LeftIterT& left, RightIterT& right)
	{
		if(LeftT::codomain_value(left) < RightT::codomain_value(right))
			_result = result_is_less;
		if(RightT::codomain_value(right) < LeftT::codomain_value(left))
			_result = result_is_greater;
		return _result == result_is_equal;
	}

    int proceed(LeftIterT& left, RightIterT& right)
    {
        if(LeftT::key_value(left).upper_equal(RightT::key_value(right)))
        { 
            ++left; 
            ++right;    
            return nextboth;
        }
        else if(LeftT::key_value(left).upper_less(RightT::key_value(right)))
        {
            _prior_left = left;
            ++left;
            return nextleft;
        }
        else
        {
            _prior_right = right;
            ++right;
            return nextright;
        }
    }

    int next_both(LeftIterT& left, RightIterT& right)
    {
        if(left == _left_end)
        {
            _result = (right == _right_end) ? result_is_equal : result_is_less;
            return stop;
        }

        // left != _left_end
        if(right == _right_end)
		{
			_result = result_is_greater;
            return stop;
		}

		// Two matching intervals for left and right
		// if they both start at or before the _scope, they are
		// assumed to be leftaligned.
		if(!(    LeftT::key_value(left).lower_less_equal(_scope) 
			  && RightT::key_value(right).lower_less_equal(_scope)))
		{
			// The starting intervals have to begin equally
			if(LeftT::key_value(left).lower_less(RightT::key_value(right)))
			{   // left: same A... = sameA...
				// right:same  B.. = sameB...
				_result = result_is_less;
				return stop;
			}

			if(LeftT::key_value(right).lower_less(RightT::key_value(left)))
			{   // left: same  B.. = sameB...
				// right:same A... = sameA...
				_result = result_is_greater;
				return stop;
			}

			if(compare_codomain() && covalues_are_equal(left, right))
				return stop;
		}

		// If left and right intervals reach to or beyond the _scope's end
		// the result is equality
		if(   _scope.upper_less_equal(LeftT::key_value(left))
		   && _scope.upper_less_equal(RightT::key_value(right)))
			return stop;

        return leftaligned;
    }

    int next_left(LeftIterT& left, RightIterT& right)
    {
        if(left == _left_end)
		{   // left: same
			// right:sameA...
			_result = result_is_less;
            return stop;
		}

        if(!LeftT::key_value(_prior_left).touches(LeftT::key_value(left)))
		{   // left: same B = sameB...
			// right:sameA  = sameA...
			_result = result_is_greater;
            return stop;
		}

        if(compare_codomain() && covalues_are_equal(left, right))
			return stop;

		// If left and right intervals reach to or beyond the _scope's end
		// the result is equality
		if(   _scope.upper_less_equal(LeftT::key_value(left))
		   && _scope.upper_less_equal(RightT::key_value(right)))
			return stop;

        return proceed(left, right);
    }

    int next_right(LeftIterT& left, RightIterT& right)
    {
        if(right == _right_end)
		{   // left: sameA...
			// right:same
			_result = result_is_greater;
            return stop;
		}

        if(!RightT::key_value(_prior_right).touches(RightT::key_value(right)))
		{
			// left: sameA... = sameA...
			// right:same B.. = sameB...
			_result = result_is_less;
            return stop;
		}

        if(compare_codomain() && covalues_are_equal(left, right))
			return stop;

		// If left and right intervals reach to or beyond the _scope's end
		// the result is equality
		if(   _scope.upper_less_equal(LeftT::key_value(left))
		   && _scope.upper_less_equal(RightT::key_value(right)))
			return stop;

        return proceed(left, right);
    }

private:
	const LeftT&  _left;
	const RightT& _right;
    LeftIterT     _left_end;
    RightIterT    _right_end;
	bool          _compare_codomain;
    LeftIterT     _prior_left;
    RightIterT    _prior_right;
    int           _result;
	typename LeftT::interval_type _scope;
};

/* Lexicographical comparison on ranges of two interval container */
template<class LeftT, class RightT>
int lexicographical_compare_3way
(
	const LeftT& left,   //sub
	const RightT& right, //super
	typename LeftT::const_iterator  left_begin,   
	typename LeftT::const_iterator  left_end,
	typename RightT::const_iterator right_begin, 
	typename RightT::const_iterator right_end
)
{
    typedef interval_sequence_tracker<LeftT,RightT> Step;
    Step step(left, right, left_end, right_end);
	step.set_compare_codomain(is_map<LeftT>::value && is_map<RightT>::value);

    typename LeftT::const_iterator  left_  = left_begin;
    typename RightT::const_iterator right_ = right_begin;

    int state = Step::nextboth;
    while(state != Step::stop)
    {
        switch(state){
        case Step::nextboth:    state = step.next_both(left_, right_);  break;
        case Step::nextleft:    state = step.next_left(left_, right_);  break;
        case Step::nextright:   state = step.next_right(left_, right_); break;
        case Step::leftaligned: state = step.proceed(left_, right_);    break;
        }
    }
    return step.result();
}

template<class LeftT, class RightT>
bool is_element_equal(const LeftT& left, const RightT& right)
{
	return lexicographical_compare_3way
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			) == 0;
}

template<class LeftT, class RightT>
bool is_element_less(const LeftT& left, const RightT& right)
{
	return lexicographical_compare_3way
		    (
				left, right, 
				left.begin(), left.end(), 
				right.begin(), right.end()
			) == -1;
}

} // namespace Interval_Set
    
}} // namespace itl boost

#endif 


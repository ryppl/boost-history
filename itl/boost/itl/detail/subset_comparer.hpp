/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_subset_comparer_JOFA_090202_H__
#define __itl_subset_comparer_JOFA_090202_H__

#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/relation_state.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/interval.hpp>

namespace boost{namespace itl
{

namespace Interval_Set
{

template<class LeftT, class RightT>
class subset_comparer
{
public:
    typedef typename LeftT::const_iterator  LeftIterT;
    typedef typename RightT::const_iterator RightIterT;

    subset_comparer(const LeftT&      left,
		            const RightT&     right,
		            const LeftIterT&  left_end,
		            const RightIterT& right_end)
        : _left(left), _right(right),
	      _left_end(left_end), _right_end(right_end), 
		  _compare_codomain(false), _result(equal)
    {}

    enum{nextboth, nextleft, nextright, stop};

	enum
	{
		unrelated  = inclusion::unrelated, 
		subset     = inclusion::subset,     // left is_subset_of   right 
		superset   = inclusion::superset,   // left is_superset_of right
		equal      = inclusion::equal       // equal = subset | superset
	};

	void set_compare_codomain(bool truth=true)
	{ _compare_codomain = truth; }

	bool compare_codomain()const { return _compare_codomain; }

    int result()const{ return _result; }

	bool covalues_are_equal(LeftIterT& left, RightIterT& right)
	{
		if(LeftT::codomain_value(left) == RightT::codomain_value(right))
			return true;
		
		_result = unrelated;
		return false;
	}

	int restrict_result(int state) { return _result &= state; }

    int proceed(LeftIterT& left, RightIterT& right)
    {
		if(LeftT::key_value(left).upper_less(RightT::key_value(right)))
		{   // left  ..)  
			// right .....)
			_prior_left = left;
			++left;
			return nextleft;
		}
		else if(RightT::key_value(right).upper_less(LeftT::key_value(left)))
		{   // left  .....)
			// right ..)
			_prior_right = right;
            ++right;
            return nextright;
        }
		else//LeftT::key_value(left).upper_equal(RightT::key_value(right))
		{   // left  ..)
			// right ..)
            ++left; 
            ++right;    
            return nextboth;
		}
    }

    int next_both(LeftIterT& left, RightIterT& right)
    {
        if(left == _left_end && right == _right_end)
            return stop;
		else if(left == _left_end)
		{   // left: ....end    left could be subset
			// right:....[..
			restrict_result(subset);
			return stop;
		}
        else if(right == _right_end)
		{   // left: ....[..    left could be superset
			// right:....end
			restrict_result(superset);
			return stop;
		}
		else if(LeftT::key_value(left).exclusive_less(RightT::key_value(right)))
		{   // left: [..) . . .[---)      left could be superset
			// right:           [..)....  if [---) exists
			restrict_result(superset);
			if(unrelated == _result)
				return stop;
			else
			{
				LeftIterT joint_ = _left.lower_bound(RightT::key_value(right));
				if(joint_ == _left.end())
				{
					_result = unrelated;
					return stop;
				}
				else
				{
					left = joint_;
					return nextboth;
				}
			}
		}
		else if(RightT::key_value(right).exclusive_less(LeftT::key_value(left)))
		{   // left:             [..  left could be subset
			// right:....) . . .[---) if [---) exists 
			restrict_result(subset);
			if(unrelated == _result)
				return stop;
			else
			{
				RightIterT joint_ = _right.lower_bound(LeftT::key_value(left));
				if(joint_ == _right.end())
				{
					_result = unrelated;
					return stop;
				}
				else
				{
					right = joint_;
					return nextboth;
				}
			}
		}

		// left and right have intervals with nonempty intersection:
        if(compare_codomain() && !covalues_are_equal(left, right))
			return stop;

		// examine left borders only. Right borders are checked in proceed
		if(LeftT::key_value(left).lower_less(RightT::key_value(right)))
		{   // left: ....[...     left could be superset
			// right:....   [..
			if(unrelated == restrict_result(superset))
				return stop;
		}
		else if(LeftT::key_value(right).lower_less(RightT::key_value(left)))
		{   // left: ....   [..   left can be subset
			// right:....[...
			if(unrelated == restrict_result(subset))
				return stop;
		}
		//else LeftT::key_value(right).lower_equal(RightT::key_value(left))
		    // left: ....[..   both can be equal
			// right:....[..
			// nothing to do: proceed

		return proceed(left, right);
    }

    int next_left(LeftIterT& left, RightIterT& right)
    {
        if(left == _left_end)
		{   // left: ..)end    left could be subset
			// right:......)
			restrict_result(subset);
			return stop;			
		}
        else if(!LeftT::key_value(_prior_left).touches(LeftT::key_value(left)))
		{   // left: ..)   [..
			// right:.........)
			if(RightT::key_value(right).lower_less(LeftT::key_value(left)))
			{	//   ..)   [..   left could be subset
				//   ..........)
				if(unrelated == restrict_result(subset))
					return stop;			
			}
			//else   ..)   [...
				//          [..
			if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) 
				&& !covalues_are_equal(left, right))
				return stop;
		}
		else
		{   // left: ..)[..  left could be subset
			// right:.......)
			if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) 
				&& !covalues_are_equal(left, right))
				return stop;
		}

        return proceed(left, right);
    }


    int next_right(LeftIterT& left, RightIterT& right)
    {
        if(right == _right_end)
		{   // left: ......)    left could be superset
			// right:..)end
			restrict_result(superset);
			return stop;			
		}
        else if(!RightT::key_value(_prior_right).touches(RightT::key_value(right)))
		{   // left: .........)
			// right:..)   [..
			if(LeftT::key_value(left).lower_less(RightT::key_value(right)))
			{	//       [....)  left could be superset
				//   ..)   [..
				if(unrelated == restrict_result(superset))
					return stop;			
			}
			//else       [....)
				//   ..)   [..
			if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) 
				&& !covalues_are_equal(left, right))
				return stop;
		}
		else
		{
			if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) 
				&& !covalues_are_equal(left, right))
				return stop;
		}

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
};





//------------------------------------------------------------------------------
// Subset/superset comparison on ranges of two interval container 
//------------------------------------------------------------------------------
template<class LeftT, class RightT>
int subset_compare
(
	const LeftT& left,   //sub
	const RightT& right, //super
	typename LeftT::const_iterator  left_begin,   
	typename LeftT::const_iterator  left_end,
	typename RightT::const_iterator right_begin, 
	typename RightT::const_iterator right_end
)
{
    typedef subset_comparer<LeftT,RightT> Step;
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
        }
    }
    return step.result();
}


} // namespace Interval_Set
    
}} // namespace itl boost

#endif 


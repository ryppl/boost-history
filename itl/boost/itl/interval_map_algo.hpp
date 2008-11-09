/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_interval_map_algo_JOFA_081026_H__
#define __itl_interval_map_algo_JOFA_081026_H__

#include <boost/itl/notate.hpp>
#include <boost/itl/type_traits/neutron.hpp>

namespace boost{namespace itl
{

namespace Map
{

template<class LeftT, class RightT>
class interval_map_sequence_tracker
{
public:
    typedef typename LeftT::const_iterator  LeftIterT;
    typedef typename RightT::const_iterator RightIterT;

    interval_map_sequence_tracker(const LeftT& left, 
                                  const RightT& right)
        : _left(left), _right(right), _result(false)
    {}

    enum{nextboth, nextleft, nextright, leftaligned, stop};

    bool result()const{ return _result; }

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
        if(left == _left.end())
        {
            _result = (right == _right.end()) ? true : false;
            return stop;
        }

        // left != _left.end()
        if(right == _right.end())
            return stop; //_result = false;

        // The starting intervals have to begin equally
        if(!LeftT::key_value(left).lower_equal(RightT::key_value(right)))
            return stop; //_result = false;

        if(!(LeftT::data_value(left) == RightT::data_value(right)))
            return stop; //_result = false;

        return leftaligned;
    }

    int next_left(LeftIterT& left, RightIterT& right)
    {
        if(left == _left.end())
            return stop;

        if(!LeftT::key_value(_prior_left).touches(LeftT::key_value(left)))
            return stop; //_result = false;

        if(!(LeftT::data_value(left) == RightT::data_value(right)))
            return stop; //_result = false;

        return proceed(left, right);
    }

    int next_right(LeftIterT& left, RightIterT& right)
    {
        if(right == _right.end())
            return stop;

        if(!RightT::key_value(_prior_right).touches(RightT::key_value(right)))
            return stop; //_result = false;

        if(!(LeftT::data_value(left) == RightT::data_value(right)))
            return stop; //_result = false;

        return proceed(left, right);
    }

private:
    const LeftT&  _left;
    const RightT& _right;
    LeftIterT     _prior_left;
    RightIterT    _prior_right;
    bool          _result;
};

template<class LeftT, class RightT>
bool is_element_equal(const LeftT& left, const RightT& right)
{
    if(left.empty())
        return right.empty();
    else if(right.empty())
        return false;

    typedef interval_map_sequence_tracker<LeftT,RightT> Step;
    Step step(left, right);

    typename LeftT::const_iterator  left_  = left.begin();
    typename RightT::const_iterator right_ = right.begin();

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

/*CL
template<class LeftT, class RightT>
int lexicographical_compare(LeftT::const_iterator left_begin,   LeftT::const_iterator left_end, 
							RightT::const_iterator right_begin, RightT::const_iterator right_end)
{
    if(left.empty())
        return right.empty();
    else if(right.empty())
        return false;

    typedef interval_map_sequence_tracker<LeftT,RightT> Step;
    Step step(left_end, right_end);

    typename LeftT::const_iterator  left_  = left.begin();
    typename RightT::const_iterator right_ = right.begin();

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
*/

} //Map
    
}} // namespace itl boost

#endif 


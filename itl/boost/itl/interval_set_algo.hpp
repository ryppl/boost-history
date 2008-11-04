/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_interval_set_algo_JOFA_081005_H__
#define __itl_interval_set_algo_JOFA_081005_H__

#include <boost/itl/notate.hpp>
#include <boost/itl/type_traits/neutron.hpp>

namespace boost{namespace itl
{

template <class IntervalContainerT>
typename IntervalContainerT::size_type continuous_cardinality(const IntervalContainerT& object)
{
    typedef typename IntervalContainerT::size_type size_type;

    size_type size = neutron<size_type>::value();
    size_type interval_size;
    const_FORALL(typename IntervalContainerT, it, object)
    {
        interval_size = IntervalContainerT::key_value(it).continuous_cardinality();
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

    size_type size = neutron<size_type>::value();
    const_FORALL(typename IntervalContainerT, it, object)
        size += IntervalContainerT::key_value(it).discrete_cardinality();
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


namespace Set
{

template<class LeftIntervalSetT, class RightIntervalSetT>
class interval_set_sequence_tracker
{
public:
    typedef typename LeftIntervalSetT::const_iterator  LeftIterT;
    typedef typename RightIntervalSetT::const_iterator RightIterT;

    interval_set_sequence_tracker(const LeftIntervalSetT& left, 
                              const RightIntervalSetT& right)
        : _left(left), _right(right), _result(false)
    {}

    enum{nextboth, nextleft, nextright, leftaligned, stop};

    bool result()const{ return _result; }

    int proceed(LeftIterT& left, RightIterT& right)
    {
        if((*left).upper_equal(*right))
        { 
            ++left; 
            ++right;    
            return nextboth;
        }
        else if((*left).upper_less(*right))
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
        if(!(*left).lower_equal(*right))
            return stop; //_result = false;

        return leftaligned;
    }

    int next_left(LeftIterT& left, RightIterT& right)
    {
        if(left == _left.end())
            return stop;
        if(!(*_prior_left).touches(*left))
            return stop; //_result = false;

        return proceed(left, right);
    }

    int next_right(LeftIterT& left, RightIterT& right)
    {
        if(right == _right.end())
            return stop;
        if(!(*_prior_right).touches(*right))
            return stop; //_result = false;

        return proceed(left, right);
    }

private:
    const LeftIntervalSetT&  _left;
    const RightIntervalSetT& _right;
    LeftIterT                _prior_left;
    RightIterT               _prior_right;
    bool                     _result;
};

template<class LeftT, class RightT>
bool is_element_equal(const LeftT& left, const RightT& right)
{
    if(left.empty())
        return right.empty();
    else if(right.empty())
        return false;

    typedef interval_set_sequence_tracker<LeftT,RightT> Step;
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

} //Set
    
}} // namespace itl boost

#endif 


/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_MAPALGO_H_JOFA_080225__
#define __itl_MAPALGO_H_JOFA_080225__

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/set_algo.hpp>

namespace boost{namespace itl
{
namespace Map 
{

template<class MapType>
bool contained_in(const MapType& sub, const MapType& super)
{
    if(&super == &sub)                   return true;
    if(sub.empty())                      return true;
    if(super.empty())                    return false;
    if(super.size()    < sub.size()    ) return false;
    if(*sub.begin()    < *super.begin()) return false;
    if(*super.rbegin() < *sub.rbegin() ) return false;

    typename MapType::const_iterator common_lwb_;
    typename MapType::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sub, super))
        return false;

    typename MapType::const_iterator sub_ = sub.begin(), super_;
    while(sub_ != sub.end())
    {
        super_ = super.find((*sub_).KEY_VALUE);
        if(super_ == super.end()) 
            return false;
        else if(!(sub_->CONT_VALUE == super_->CONT_VALUE))
            return false;
        sub_++;
    }
    return true;
}

template<class MapType>
bool contained_in(const typename MapType::set_type& sub, const MapType& super)
{
	typedef typename MapType::set_type SetType;

    if(sub.empty())                      return true;
    if(super.empty())                    return false;
    if(super.size()    < sub.size()    ) return false;
    if(*sub.begin()    < *super.begin()) return false;
    if(*super.rbegin() < *sub.rbegin() ) return false;

    typename SetType::const_iterator common_lwb_;
    typename SetType::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sub, super))
        return false;

    typename SetType::const_iterator sub_ = sub.begin();
	typename MapType::const_iterator super_;
    while(sub_ != sub.end())
    {
        super_ = super.find(*sub_++);
        if(super_ == super.end()) 
            return false;
    }
    return true;
}


template <class MapType>
bool intersects(const MapType& left, const MapType& right)
{
	typename MapType::const_iterator right_common_lower_;
	typename MapType::const_iterator right_common_upper_;
	if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
		return false;

	typename MapType::const_iterator right_ = right_common_lower_;
	while(right_ != right_common_upper_)
		if(left.intersects(*right_++))
			return true;

	return false;
}

template <class ObjectT, class CoObjectT>
bool key_intersects(const ObjectT& left, const CoObjectT& right)
{
	typename CoObjectT::const_iterator right_common_lower_;
	typename CoObjectT::const_iterator right_common_upper_;
	if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
		return false;

	typename CoObjectT::const_iterator right_ = right_common_lower_;
	while(right_ != right_common_upper_)
		if(left.intersects(CoObjectT::key_value(right_++)))
			return true;

	return false;
}

//----------------------------------------------------------------------
// flip
//----------------------------------------------------------------------
template<class MapType>
void flip(MapType& result, const MapType& x2)
{
    if(is_total<MapType>::value && absorbs_neutrons<MapType>::value)
    {
        result.clear();
        return;
    }

    typename MapType::const_iterator x2_ = x2.begin(), cur_x2_, x1_;
    while(x2_ != x2.end()) 
    {
        cur_x2_ = x2_;
        std::pair<typename MapType::iterator,bool> insertion 
            = result.insert(*x2_++);
        if(!insertion.WAS_SUCCESSFUL)
        {
            //result.erase(insertion.ITERATOR);
            if(is_set<typename MapType::codomain_type>::value)
            {
                typename MapType::iterator res_ = insertion.ITERATOR;
                typename MapType::codomain_type common_value = res_->CONT_VALUE;
                typename MapType::key_type key_value = res_->KEY_VALUE;
                typename MapType::inverse_codomain_intersect()(common_value, cur_x2_->CONT_VALUE);
                result.subtract(*res_);
                result.add(typename MapType::value_type(key_value, common_value));
            }
            else
                result.subtract(*insertion.ITERATOR);
        }
    }

    if(is_total<MapType>::value && !absorbs_neutrons<MapType>::value)
        FORALL(typename MapType, it_, result)
            it_->CONT_VALUE = neutron<typename MapType::codomain_type>::value();
}



template<class MapType>
typename MapType::const_iterator next_proton(typename MapType::const_iterator& iter_, const MapType& object)
{
    while(   iter_ != object.end() 
          && iter_->CONT_VALUE == neutron<typename MapType::codomain_type>::value())
        ++iter_;

    return iter_;
}

/** Function template <tt>lexicographical_equal</tt> implements 
lexicographical equality except for neutronic content values. */
template<class MapType>
bool lexicographical_protonic_equal(const MapType& left, const MapType& right)
{
    if(&left == &right)        
        return true;

    typename MapType::const_iterator left_  = left.begin();
    typename MapType::const_iterator right_ = right.begin();

    left_  = next_proton(left_,  left);
    right_ = next_proton(right_, right);

    while(left_ != left.end() && right_ != right.end())
    {
        if(!(left_->KEY_VALUE == right_->KEY_VALUE && left_->CONT_VALUE == right_->CONT_VALUE))
            return false;

        ++left_;
        ++right_;
        left_  = next_proton(left_,  left);
        right_ = next_proton(right_, right);
    }

    return left_ == left.end() && right_ == right.end();
}

/*CL
//------------------------------------------------------------------------------
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


    int co_compare(LeftIterT& left, RightIterT& right)
    {
	    using namespace boost::mpl;

		return  
		    if_<
                bool_<is_concept_equivalent<is_interval_map,LeftT,RightT>::value>,
                map_codomain_compare<LeftT,RightT>,
                empty_codomain_compare<LeftT,RightT>
            >
            ::type::apply(left,right);
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
        if(compare_codomain())
			if(unrelated == restrict_result(co_compare(left,right)))
				return stop;

        // examine left borders only. Right borders are checked in proceed
        if(LeftT::key_value(left).lower_less(RightT::key_value(right)))
        {   // left: ....[...     left could be superset
            // right:....   [..
            if(unrelated == restrict_result(superset))
                return stop;
        }
        else if(RightT::key_value(right).lower_less(LeftT::key_value(left)))
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
            {   //   ..)   [..   left could be subset
                //   ..........)
                if(unrelated == restrict_result(subset))
                    return stop;            
            }
            //else   ..)   [...
            //          [..
            if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) )
				if(unrelated == restrict_result(co_compare(left,right)))
					return stop;
        }
        else
        {   // left: ..)[..  left could be subset
            // right:.......)
            if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) )
				if(unrelated == restrict_result(co_compare(left,right)))
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
            {   //       [....)  left could be superset
                //   ..)   [..
                if(unrelated == restrict_result(superset))
                    return stop;            
            }
            //else       [....)
            //   ..)   [..
            if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) )
				if(unrelated == restrict_result(co_compare(left,right)))
					return stop;
        }
        else
        {
            if(compare_codomain() && !LeftT::key_value(left).is_disjoint(RightT::key_value(right)) )
				if(unrelated == restrict_result(co_compare(left,right)))
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



*/

} // namespace Map
}} // namespace boost itl

#endif


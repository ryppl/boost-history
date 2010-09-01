/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_SET_ALGO_HPP_JOFA_081005
#define BOOST_ITL_INTERVAL_SET_ALGO_HPP_JOFA_081005

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/relation_state.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/type_traits/is_total.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/detail/element_comparer.hpp>
#include <boost/itl/detail/interval_subset_comparer.hpp>

namespace boost{namespace itl
{

template <class IntervalContainerT>
typename IntervalContainerT::size_type continuous_cardinality(const IntervalContainerT& object)
{
    typedef typename IntervalContainerT::size_type size_type;
    typedef typename IntervalContainerT::interval_type interval_type;

    size_type size = neutron<size_type>::value();
    size_type interval_size;
    ITL_const_FORALL(typename IntervalContainerT, it, object)
    {
        //CL interval_size = continuous_interval_<interval_type>::cardinality(IntervalContainerT::key_value(it));
        interval_size = itl::cardinality(IntervalContainerT::key_value(it));
        if(interval_size == infinity<size_type>::value())
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
    ITL_const_FORALL(typename IntervalContainerT, it, object)
        size += discrete_interval_<interval_type>::cardinality(IntervalContainerT::key_value(it));
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

template<class IntervalContainerT>
bool is_joinable(const IntervalContainerT& container, 
                 typename IntervalContainerT::const_iterator first, 
                 typename IntervalContainerT::const_iterator past) 
{
    if(first == container.end())
        return true;

    typename IntervalContainerT::const_iterator it_ = first, next_ = first;
    ++next_;

    while(next_ != container.end() && it_ != past)
        if(!itl::touches(IntervalContainerT::key_value(it_++),
                         IntervalContainerT::key_value(next_++)))
            return false;

    return true;
}


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
typename enable_if<mpl::and_<is_concept_combinable<is_interval_set, is_interval_map, LeftT, RightT>, 
                             is_total<RightT> >,
                   bool>::type
within(const LeftT&, const RightT&)
{
    return true;
}

template<class LeftT, class RightT>
typename enable_if<mpl::and_<is_concept_combinable<is_interval_set, is_interval_map, LeftT, RightT>, 
                             mpl::not_<is_total<RightT> > >,
                   bool>::type
within(const LeftT& sub, const RightT& super)
{
    int result =
        subset_compare
        (
            sub, super, 
            sub.begin(), sub.end(), 
            super.begin(), super.end()
        );
    return result == inclusion::subset || result == inclusion::equal;
}


template<class LeftT, class RightT> //JODO Codereplication: Unify some more
typename enable_if<is_concept_combinable<is_interval_map, is_interval_map, LeftT, RightT>, 
                   bool>::type
within(const LeftT& sub, const RightT& super)
{
    int result =
        subset_compare
        (
            sub, super, 
            sub.begin(), sub.end(), 
            super.begin(), super.end()
        );
    return result == inclusion::subset || result == inclusion::equal;
}

template<class LeftT, class RightT>
typename enable_if<is_concept_combinable<is_interval_set, is_interval_set, LeftT, RightT>, 
                   bool>::type
within(const LeftT& sub, const RightT& super)
{
    int result =
        subset_compare
        (
            sub, super, 
            sub.begin(), sub.end(), 
            super.begin(), super.end()
        );
    return result == inclusion::subset || result == inclusion::equal;
}



template<class LeftT, class RightT>
typename enable_if<mpl::and_<is_concept_combinable<is_interval_map, is_interval_set, LeftT, RightT>, 
                             is_total<LeftT> >,
                   bool>::type
contains(const LeftT&, const RightT&)
{
    return true;
}

template<class LeftT, class RightT>
typename enable_if<mpl::and_<is_concept_combinable<is_interval_map, is_interval_set, LeftT, RightT>, 
                             mpl::not_<is_total<LeftT> > >,
                   bool>::type
contains(const LeftT& super, const RightT& sub)
{
    int result =
        subset_compare
        (
            super, sub, 
            super.begin(), super.end(), 
            sub.begin(), sub.end()
        );
    return result == inclusion::superset || result == inclusion::equal;
}

template<class LeftT, class RightT>
typename enable_if<is_concept_combinable<is_interval_set, is_interval_set, LeftT, RightT>, 
                   bool>::type
contains(const LeftT& super, const RightT& sub)
{
    int result =
        subset_compare
        (
            super, sub, 
            super.begin(), super.end(), 
            sub.begin(), sub.end()
        );
    return result == inclusion::superset || result == inclusion::equal;
}

template<class IntervalContainerT>
bool contains(const IntervalContainerT& super, 
              const typename IntervalContainerT::element_type& element)
{
    return super.contains(element);
}

template<class IntervalContainerT>
bool contains(const IntervalContainerT& container, 
              const typename IntervalContainerT::segment_type& sub_interval)
{
    typedef typename IntervalContainerT::const_iterator const_iterator;
    if(itl::is_empty(sub_interval)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior = container.equal_range(sub_interval);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = prior(exterior.second);

    return
          itl::contains(hull(*(exterior.first), *last_overlap), sub_interval)
      &&  Interval_Set::is_joinable(container, exterior.first, last_overlap);
}


template<class IntervalContainerT>
bool is_dense(const IntervalContainerT& container, 
              typename IntervalContainerT::const_iterator first, 
              typename IntervalContainerT::const_iterator past) 
{
    if(first == container.end())
        return true;

    typename IntervalContainerT::const_iterator it_ = first, next_ = first;
    ++next_;

    while(next_ != container.end() && it_ != past)
        if(!itl::touches(IntervalContainerT::key_value(it_++), 
                         IntervalContainerT::key_value(next_++)))
            return false;

    return true;
}

} // namespace Interval_Set

namespace detail
{

template<class Type>
typename Type::iterator 
    join_on_left(Type& object, typename Type::iterator& left_, 
                               typename Type::iterator& right_)
{
    typedef typename Type::value_type    value_type;
    typedef typename Type::interval_type interval_type;
    // both left and right are in the set and they are neighbours
    BOOST_ASSERT(exclusive_less(Type::key_value(left_), Type::key_value(right_)));
    BOOST_ASSERT(touches(Type::key_value(left_), Type::key_value(right_)));

    interval_type right_itv = Type::key_value(right_);
    object.erase(right_);
    const_cast<value_type&>(Type::key_value(left_)) 
        = hull(Type::key_value(left_), right_itv);

    return left_;
}

template<class Type>
typename Type::iterator
    join_neighbours(Type& object, typename Type::iterator it_)
{
    using namespace detail;
    typedef typename Type::iterator iterator;

    if(it_ == object.begin())
    {
        iterator it_nxt=it_; it_nxt++;
        if(it_nxt!=object.end() && touches(Type::key_value(it_), 
                                           Type::key_value(it_nxt))) 
            return join_on_left(object, it_, it_nxt);
    }
    else
    {
        // there is a predecessor
        iterator pred_ = it_; pred_-- ;

        if(touches(Type::key_value(pred_), Type::key_value(it_))) 
        {
            iterator it_extended = join_on_left(object, pred_, it_);

            iterator succ_=it_extended; succ_++;
            if(succ_!=object.end())
            {
                // it's a non border element that might have two touching neighbours
                if(touches(Type::key_value(it_extended), Type::key_value(succ_))) 
                    return join_on_left(object, it_extended, succ_);
                else
                    return it_extended;
            }
            else
                return it_extended;
        }
        else
        {
            iterator succ_=it_; succ_++;
            if(succ_!=object.end())
            {
                // it's a non border element that might have a right touching neighbour
                if(touches(Type::key_value(it_), Type::key_value(succ_))) 
                    return join_on_left(object, it_, succ_);
            }
        }
    }

    return it_;
}


template<class Type>
typename Type::iterator 
    join_under(Type& object, const typename Type::value_type& addend)
{
    //ASSERT: There is at least one interval in object that overlaps with addend
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    std::pair<iterator,iterator> overlap = object.equal_range(addend);
    iterator first_ = overlap.first,
             end_   = overlap.second,
             last_  = end_; --last_;

    iterator second_= first_; ++second_;

    interval_type left_resid  = right_subtract(Type::key_value(first_), addend);
    interval_type right_resid =  left_subtract(Type::key_value(last_) , addend);

    object.erase(second_, end_);

    const_cast<value_type&>(Type::key_value(first_)) 
        = hull(hull(left_resid, addend), right_resid);
    return first_;
}

//==============================================================================
//= Addition joining
//==============================================================================
template<class Type>
typename Type::iterator 
    joining_add(Type& object, const typename Type::value_type& addend)
{
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    if(itl::is_empty(addend)) 
        return object.end();

    std::pair<iterator,bool> insertion = object._insert(addend);

    if(insertion.second)
        return join_neighbours(object, insertion.first);
    else
        return join_neighbours(object, join_under(object, addend));
}

template<class Type>
typename Type::iterator 
    joining_add(Type& object, typename Type::iterator    prior_, 
                        const typename Type::value_type& addend)
{
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    if(itl::is_empty(addend)) 
        return prior_;

    iterator insertion = object._insert(prior_, addend);

    if(*insertion == addend)
        return join_neighbours(object, insertion);
    else
        return join_neighbours(object, join_under(object, addend));
}


//==============================================================================
//= Addition separating
//==============================================================================
template<class Type>
typename Type::iterator 
    separating_add(Type& object, const typename Type::value_type& addend)
{
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    if(itl::is_empty(addend)) 
        return object.end();

    std::pair<iterator,bool> insertion = object._insert(addend);

    if(insertion.second)
        return insertion.first;
    else
        return join_under(object, addend);
}

template<class Type>
typename Type::iterator 
    separating_add(Type& object, typename Type::iterator    prior_, 
                           const typename Type::value_type& addend)
{
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    if(itl::is_empty(addend)) 
        return prior_;

    iterator insertion = object._insert(prior_, addend);

    if(*insertion == addend)
        return insertion;
    else
        return join_under(object, addend);
}


//==============================================================================
//= Addition splitting
//==============================================================================

template<class Type>
void add_front(Type& object, const typename Type::interval_type& inter_val, 
                                   typename Type::iterator&      first_)
{
	typedef typename Type::interval_type interval_type;
	typedef typename Type::iterator      iterator;
    // If the collision sequence has a left residual 'left_resid' it will
    // be split, to provide a standardized start of algorithms:
    // The addend interval 'inter_val' covers the beginning of the collision sequence.

    // only for the first there can be a left_resid: a part of *first_ left of inter_val
    interval_type left_resid = right_subtract(*first_, inter_val);

    if(!itl::is_empty(left_resid))
    {   //            [------------ . . .
        // [left_resid---first_ --- . . .
        iterator prior_ = object.prior(first_);
        const_cast<interval_type&>(*first_) = left_subtract(*first_, left_resid);
        //NOTE: Only splitting
        iterator insertion_ = object._insert(prior_, left_resid);
    }

    //POST:
    // [----- inter_val ---- . . .
    // ...[-- first_ --...
}


template<class Type>
void add_segment(Type& object, const typename Type::interval_type& inter_val, 
                                     typename Type::iterator&      it_      )
{
	typedef typename Type::interval_type interval_type;
    interval_type lead_gap = right_subtract(inter_val, *it_);
    if(!itl::is_empty(lead_gap))
        //           [lead_gap--- . . .
        // [prior_)           [-- it_ ...
        object._insert(prior(it_), lead_gap);

    // . . . --------- . . . addend interval
    //      [-- it_ --)      has a common part with the first overval
    ++it_;
}


template<class Type>
void add_main(Type& object, typename Type::interval_type& rest_interval, 
                            typename Type::iterator&      it_,
                      const typename Type::iterator&      last_)
{
	typedef typename Type::interval_type interval_type;
    interval_type cur_interval;
    while(it_ != last_)
    {
        cur_interval = *it_ ;
        add_segment(object, rest_interval, it_);
        // shrink interval
        rest_interval = left_subtract(rest_interval, cur_interval);
    }
}


template<class Type>
void add_rear(Type& object, const typename Type::interval_type& inter_val, 
                                  typename Type::iterator&      it_      )
{
	typedef typename Type::interval_type interval_type;
	typedef typename Type::iterator      iterator;

    iterator prior_ = object.prior(it_);
    interval_type cur_itv = *it_;

    interval_type lead_gap = right_subtract(inter_val, cur_itv);
    if(!itl::is_empty(lead_gap))
        //          [lead_gap--- . . .
        // [prior_)          [-- it_ ...
        object._insert(prior_, lead_gap);
    
    interval_type end_gap = left_subtract(inter_val, cur_itv);
    if(!itl::is_empty(end_gap))
        // [---------------end_gap)
        //      [-- it_ --)
        it_ = object._insert(it_, end_gap);
    else
    {
        // only for the last there can be a right_resid: a part of *it_ right of addend
        interval_type right_resid = left_subtract(cur_itv, inter_val);

        if(!itl::is_empty(right_resid))
        {
            // [--------------)
            //      [-- it_ --right_resid)
            const_cast<interval_type&>(*it_) = right_subtract(*it_, right_resid);
            it_ = object._insert(it_, right_resid);
        }
    }
}


template<class Type>
typename Type::iterator 
    splitting_add(Type& object, const typename Type::value_type& addend)
{
	typedef typename Type::interval_type interval_type;
	typedef typename Type::iterator      iterator;

    if(itl::is_empty(addend)) 
        return object.end();

    std::pair<iterator,bool> insertion = object._insert(addend);

    if(insertion.second)
        return insertion.first;
    {
        iterator first_ = object.lower_bound(addend),
                 last_  = insertion.first;
        //BOOST_ASSERT(next(last_) == this->_set.upper_bound(inter_val));

        iterator it_ = first_;
        interval_type rest_interval = addend;

        add_front(object, rest_interval, it_);
        add_main (object, rest_interval, it_, last_);
        add_rear (object, rest_interval, it_);
        return it_;
    }
}


template<class Type>
typename Type::iterator 
    splitting_add(Type& object, typename Type::iterator    prior_, 
	                      const typename Type::value_type& addend)
{
	typedef typename Type::interval_type interval_type;
	typedef typename Type::iterator      iterator;

    if(itl::is_empty(addend)) 
        return prior_;

    iterator insertion = object._insert(prior_, addend);

    if(*insertion == addend)
        return insertion;
    {
        std::pair<iterator,iterator> overlap = object.equal_range(addend);
        iterator first_ = overlap.first,
                 end_   = overlap.second,
                 last_  = end_; --last_;

        iterator it_ = first_;
        interval_type rest_interval = addend;

        add_front(object, rest_interval, it_);
        add_main (object, rest_interval, it_, last_);
        add_rear (object, rest_interval, it_);

        return it_;
    }
}


//==============================================================================
//= Subtraction
//==============================================================================
template<class Type>
void subtract(Type& object, const typename Type::value_type& minuend)
{
    typedef typename Type::iterator      iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;

    if(itl::is_empty(minuend)) return;

    std::pair<iterator, iterator> exterior = object.equal_range(minuend);
    if(exterior.first == exterior.second) return;

    iterator first_ = exterior.first;
    iterator end_   = exterior.second;
    iterator last_  = end_; --last_;

    interval_type leftResid = right_subtract(*first_, minuend);
    interval_type rightResid; 
    if(first_ != end_  )
        rightResid = left_subtract(*last_ , minuend);

    object.erase(first_, end_);

    if(!itl::is_empty(leftResid))
        object._insert(leftResid);

    if(!itl::is_empty(rightResid))
        object._insert(rightResid);
}



} // namespace detail

}} // namespace itl boost

#endif 


/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_MAP_ALGO_HPP_JOFA_100730
#define BOOST_ITL_INTERVAL_MAP_ALGO_HPP_JOFA_100730

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/not.hpp>

#include <boost/itl/type_traits/is_total.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/relation_state.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/detail/element_comparer.hpp>
#include <boost/itl/detail/interval_subset_comparer.hpp>

namespace boost{namespace itl
{


namespace Interval_Map
{

//JODO CLEAN UP for: #pragma warning(disable:4127) // conditional expression is constant

template<class IntervalMapT>
bool is_joinable(const IntervalMapT& container, 
                 typename IntervalMapT::const_iterator first, 
                 typename IntervalMapT::const_iterator past) 
{
    if(first == container.end())
        return true;

    typename IntervalMapT::const_iterator it_ = first, next_ = first;
    ++next_;

    const typename IntervalMapT::codomain_type& co_value 
        = IntervalMapT::co_value(first);
    while(it_ != past)
    {
        if(IntervalMapT::co_value(next_) != co_value)
            return false;
        if(!itl::touches(IntervalMapT::key_value(it_++),
                         IntervalMapT::key_value(next_++)))
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------
//- Containedness of key objects
//------------------------------------------------------------------------------

//- domain_type ----------------------------------------------------------------
template<class IntervalMapT>
typename enable_if<mpl::not_<is_total<IntervalMapT> >, bool>::type
contains(const IntervalMapT& container, 
         const typename IntervalMapT::domain_type& key) 
{
    return container.find(key) != container.end();
}

template<class IntervalMapT>
typename enable_if<is_total<IntervalMapT>, bool>::type
contains(const IntervalMapT& container, 
         const typename IntervalMapT::domain_type& key) 
{
    return true;
}

//- interval_type --------------------------------------------------------------
template<class IntervalMapT>
typename enable_if<mpl::not_<is_total<IntervalMapT> >, bool>::type
contains(const IntervalMapT& container, 
         const typename IntervalMapT::interval_type& sub_interval) 
{
    typedef typename IntervalMapT::const_iterator const_iterator;
    if(itl::is_empty(sub_interval)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior = container.equal_range(sub_interval);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = prior(exterior.second);

    return
          itl::contains(hull(exterior.first->first, last_overlap->first), sub_interval)
      &&  Interval_Set::is_joinable(container, exterior.first, last_overlap);
}

template<class IntervalMapT>
typename enable_if<is_total<IntervalMapT>, bool>::type
contains(const IntervalMapT& container, 
         const typename IntervalMapT::interval_type& sub_interval) 
{
    return true;
}

//- set_type -------------------------------------------------------------------
template<class IntervalMapT, class IntervalSetT>
typename enable_if<mpl::and_<mpl::not_<is_total<IntervalMapT> >
                            ,is_interval_set<IntervalSetT> >, bool>::type
contains(const IntervalMapT& super_map, const IntervalSetT& sub_set) 
{
    return Interval_Set::within(sub_set, super_map);
}

template<class IntervalMapT, class IntervalSetT>
typename enable_if<mpl::and_<is_total<IntervalMapT>
                            ,is_interval_set<IntervalSetT> >, bool>::type
contains(const IntervalMapT& super_map, const IntervalSetT& sub_set) 
{
    return true;
}


//------------------------------------------------------------------------------
//- Containedness of sub objects
//------------------------------------------------------------------------------

template<class IntervalMapT>
bool contains(const IntervalMapT& container, 
              const typename IntervalMapT::element_type& key_value_pair) 
{
    typename IntervalMapT::const_iterator it_ = container.find(key_value_pair.key);
    return it_ != container.end() && it_->second == key_value_pair.data;
}

template<class IntervalMapT>
bool contains(const IntervalMapT& container, 
              const typename IntervalMapT::segment_type sub_segment) 
{
    typedef typename IntervalMapT::const_iterator const_iterator;
    typename IntervalMapT::interval_type sub_interval = sub_segment.first;
    if(itl::is_empty(sub_interval)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior = container.equal_range(sub_interval);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = prior(exterior.second);

    if(!(sub_segment.second == exterior.first->second) )
        return false;

    return
          itl::contains(hull(exterior.first->first, last_overlap->first), sub_interval)
      &&  Interval_Map::is_joinable(container, exterior.first, last_overlap);
}


template<class IntervalMapT>
bool contains(const IntervalMapT& super, const IntervalMapT& sub) 
{
    return Interval_Set::within(sub, super);
}


} // namespace Interval_Map


namespace detail
{


template <class Type, class Combiner>
inline typename Type::iterator
gap_insert(               Type&                object, 
                 typename Type::iterator       prior_, 
           const typename Type::interval_type& inter_val, 
           const typename Type::codomain_type& co_val)
{
    typedef typename Type::value_type value_type;
    // inter_val is not conained in this map. Insertion will be successful
    BOOST_ASSERT(object.find(inter_val) == object.end());
    BOOST_ASSERT(!(absorbs_neutrons<Type>::value && co_val==Combiner::neutron()));

    return object._insert(prior_, value_type(inter_val, version<Combiner>()(co_val)));
}


template<class Type, class Combiner>
void add_segment(               Type&                object,
                 const typename Type::interval_type& inter_val, 
                 const typename Type::codomain_type& co_val, 
                       typename Type::iterator&      it_       )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::iterator      iterator;

    interval_type lead_gap = right_subtract(inter_val, it_->first);
    if(!itl::is_empty(lead_gap))
    {
        // [lead_gap--- . . .
        //          [-- it_ ...
        iterator prior_ = prior(it_); 
        iterator inserted_ = gap_insert<Type,Combiner>(object, prior_, lead_gap, co_val);
        if(prior_ != object.end() && joinable(object, prior_, inserted_))
            join_on_right(object, prior_, inserted_);
    }

    // . . . --------- . . . addend interval
    //      [-- it_ --)      has a common part with the first overval
    Combiner()(it_->second, co_val);
	if(absorbs_neutrons<Type>::value && it_->second == Combiner::neutron())
        object.erase(it_++);
    else
    {
        detail::join_left(object, it_);
        ++it_;
    }
}



} // namespace detail

}} // namespace itl boost

#endif 


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
#include <boost/itl/interval_combining_style.hpp>
#include <boost/itl/detail/element_comparer.hpp>
#include <boost/itl/detail/interval_subset_comparer.hpp>

namespace boost{namespace itl
{


namespace Interval_Map
{
using namespace segmental;

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

template <class Type, class Combiner>
inline typename Type::iterator
_map_insert(               Type&                object, 
                  typename Type::iterator       prior_, 
            const typename Type::interval_type& inter_val, 
            const typename Type::codomain_type& co_val)
{
    typedef typename Type::value_type value_type;

    return object._insert(prior_, value_type(inter_val, version<Combiner>()(co_val)));
}

template <class Type, class Combiner>
inline std::pair<typename Type::iterator, bool>
_map_insert(               Type&                object, 
            const typename Type::interval_type& inter_val, 
            const typename Type::codomain_type& co_val)
{
    typedef typename Type::value_type value_type;

    return object._insert(value_type(inter_val, version<Combiner>()(co_val)));
}

template<class Type, class Combiner, int absorbs_neutrons>
struct on_neutric;

template <class Type, class Combiner>
inline std::pair<typename Type::iterator, bool>
informing_add(          Type&                object,
         const typename Type::iterator&      prior_, 
         const typename Type::interval_type& inter_val, 
         const typename Type::codomain_type& co_val   )
{
    typedef typename Type::value_type value_type;
    typedef typename Type::iterator   iterator;
    typedef typename on_neutric<Type,Combiner,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    // Never try to insert a neutron into a neutron absorber here:
    BOOST_ASSERT(!(on_neutric_::is_absorbable(co_val)));

    iterator inserted_ 
        = object._insert(prior_, value_type(inter_val, Combiner::neutron()));

    if(inserted_->first == inter_val && inserted_->second == Combiner::neutron())
    {
        Combiner()(inserted_->second, co_val);
        return std::pair<iterator,bool>(inserted_, true);
    }
    else
        return std::pair<iterator,bool>(inserted_, false);
}

template <class Type>
inline std::pair<typename Type::iterator, bool>
informing_insert(       Type&                object,
         const typename Type::iterator&      prior_, 
         const typename Type::interval_type& inter_val, 
         const typename Type::codomain_type& co_val   )
{
    typedef typename Type::value_type value_type;
    typedef typename Type::iterator   iterator;

    iterator inserted_
        = object._insert(prior_, value_type(inter_val, co_val));

    if(inserted_ == prior_)
        return std::pair<iterator,bool>(inserted_, false);
    else if(inserted_->first == inter_val)
        return std::pair<iterator,bool>(inserted_, true);
    else
        return std::pair<iterator,bool>(inserted_, false);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class Type, class Combiner>
struct on_neutric<Type, Combiner, false>
{
    typedef on_neutric type;
    typedef typename Type::codomain_type codomain_type;

    static bool is_absorbable(const codomain_type&){ return false; }
};

template<class Type, class Combiner>
struct on_neutric<Type, Combiner, true>
{
    typedef on_neutric type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::codomain_combine codomain_combine;

    static bool is_absorbable(const codomain_type& co_value)
    {
        return absorbs_neutrons<Type>::value 
            && co_value == Combiner::neutron();
    }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class Type, int combining_style>
struct on_style;

template<class Type>
struct on_style<Type, interval_combine::splitting>
{
    typedef on_style type;
    typedef typename Type::iterator iterator;

    static iterator handle_inserted(Type&, iterator it_){ return it_; }
    static void handle_inserted(Type&, iterator, iterator&)      {}
};

template<class Type>
struct on_style<Type, interval_combine::joining>
{
    typedef on_style type;
    typedef typename Type::iterator iterator;

    static iterator handle_inserted(Type& object, iterator it_)
    { 
        return join_neighbours(object, it_); 
    }

    static void handle_inserted(Type& object, iterator inserted_, iterator& it_)
    {
        join_left(object, inserted_);
        // after filling that gap there may be another joining opportunity
        join_left(object, it_);
    }
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template<class Type, class Combiner, bool abosorbs_neutrons, int combining_style>
struct on_segment;

template<class Type, class Combiner>
struct on_segment<Type, Combiner, false, interval_combine::splitting>
{
    typedef on_segment type;
    typedef typename Type::iterator iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::codomain_type codomain_type;

    static void handle_left_combined(Type&, iterator){}
    static void handle_combined(Type&, iterator){}
    static void handle_succeeded_combined(Type&, iterator, iterator){}
    static void handle_preceeded_combined(Type&, iterator, iterator&){}
    static void handle_inserted(Type&, iterator, iterator){}
    static void handle_reinserted(Type&, iterator insertion_){}

    static void insert_at(Type& object, iterator& it_, iterator,
                          const interval_type& end_gap, const codomain_type& co_val)
    {
        it_ = gap_insert<Type,Combiner>(object, it_, end_gap, co_val);
    }

};

template<class Type, class Combiner>
struct on_segment<Type, Combiner, true, interval_combine::splitting>
{
    typedef on_segment type;
    typedef typename Type::iterator iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::codomain_type codomain_type;

    static void handle_left_combined(Type& object, iterator it_)
    {
        if(it_->second == Combiner::neutron())
            object.erase(it_);
    }

    static void handle_combined(Type& object, iterator it_)
    {
        if(it_->second == Combiner::neutron())
            object.erase(it_);
    }

    static void handle_preceeded_combined(Type& object, iterator prior_, iterator& it_)
    {
        if(it_->second == Combiner::neutron())
        {
            object.erase(it_);
            it_ = prior_;
        }
    }

    static void handle_succeeded_combined(Type& object, iterator it_, iterator)
    {
        if(it_->second == Combiner::neutron())
            object.erase(it_);
    }

    static void handle_inserted(Type&, iterator, iterator){}
    static void handle_reinserted(Type&, iterator insertion_){}

    static void insert_at(Type& object, iterator& it_, iterator prior_,
                          const interval_type& end_gap, const codomain_type& co_val)
    {
        if(it_->second == Combiner::neutron())
        {
            object.erase(it_);
            it_ = gap_insert<Type,Combiner>(object, prior_, end_gap, co_val);
        }
        else
            it_ = gap_insert<Type,Combiner>(object, it_, end_gap, co_val);
    }

};

template<class Type, class Combiner>
struct on_segment<Type, Combiner, false, interval_combine::joining>
{
    typedef on_segment type;
    typedef typename Type::iterator iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::codomain_type codomain_type;

    static void handle_left_combined(Type& object, iterator it_)
    {
        join_left(object, it_);
    }

    static void handle_combined(Type& object, iterator it_)
    {
        join_neighbours(object, it_);
    }

    static void handle_preceeded_combined(Type& object, iterator, iterator& it_)
    {
        join_neighbours(object, it_);
    }

    static void handle_succeeded_combined(Type& object, iterator it_, iterator next_)
    {
        join_left(object, it_);
        join_neighbours(object, next_);
    }

    static void handle_inserted(Type& object, iterator prior_, iterator inserted_)
    {
        if(prior_ != object.end() && joinable(object, prior_, inserted_))
            join_on_right(object, prior_, inserted_);
    }
    static void handle_reinserted(Type& object, iterator insertion_)
    { join_right(object, insertion_); }

    static void insert_at(Type& object, iterator& it_, iterator,
                          const interval_type& end_gap, const codomain_type& co_val)
    {
        join_left(object, it_);
        iterator inserted_ = gap_insert<Type,Combiner>(object, it_, end_gap, co_val);
        it_ = join_neighbours(object, inserted_);
    }
};

template<class Type, class Combiner>
struct on_segment<Type, Combiner, true, interval_combine::joining>
{
    typedef on_segment type;
    typedef typename Type::iterator iterator;
    typedef typename Type::interval_type interval_type;
    typedef typename Type::codomain_type codomain_type;

    static void handle_left_combined(Type& object, iterator it_)
    {
        if(it_->second == Combiner::neutron())
            object.erase(it_);
        else
            join_left(object, it_);
    }

    static void handle_combined(Type& object, iterator it_)
    {
        if(it_->second == Combiner::neutron())
            object.erase(it_);
        else
            join_neighbours(object, it_);
    }

    static void handle_preceeded_combined(Type& object, iterator prior_, iterator& it_)
    {
        if(it_->second == Combiner::neutron())
        {
            object.erase(it_);
            it_ = prior_;
        }
        else // After a new combination (e.g. combiner=max) joining neighbours may be possible
            join_neighbours(object, it_);
    }

    static void handle_succeeded_combined(Type& object, iterator it_, iterator next_)
    {
        if(it_->second==Combiner::neutron())
        {
            object.erase(it_);
            join_right(object, next_);
        }
        else
        {
            join_left(object, it_);
            join_neighbours(object, next_);
        }
    }

    static void handle_inserted(Type& object, iterator prior_, iterator inserted_)
    {
        if(prior_ != object.end() && joinable(object, prior_, inserted_))
            join_on_right(object, prior_, inserted_);
    }

    static void handle_reinserted(Type& object, iterator insertion_)
    { join_right(object, insertion_); }

    static void insert_at(Type& object, iterator& it_, iterator prior_, 
                          const interval_type& end_gap, const codomain_type& co_val)
    {
        if(it_->second == Combiner::neutron())
        {
            object.erase(it_);
            it_ = gap_insert<Type,Combiner>(object, prior_, end_gap, co_val);
            join_right(object, it_);
        }
        else 
        {
            join_left(object, it_);
            iterator inserted_ = gap_insert<Type,Combiner>(object, it_, end_gap, co_val);
            it_ = join_neighbours(object, inserted_);
        }
    }
};
//------------------------------------------------------------------------------

//==============================================================================
//= Addition detail
//==============================================================================

template<class Type, class Combiner>
void add_segment(               Type&                object,
                 const typename Type::interval_type& inter_val, 
                 const typename Type::codomain_type& co_val, 
                       typename Type::iterator&      it_       )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_segment<Type,Combiner,
                                absorbs_neutrons<Type>::value,
                                Type::fineness>::type on_segment_;

    interval_type lead_gap = right_subtract(inter_val, it_->first);
    if(!itl::is_empty(lead_gap))
    {
        // [lead_gap--- . . .
        //          [-- it_ ...
        iterator prior_ = prior(it_); 
        iterator inserted_ = gap_insert<Type,Combiner>(object, prior_, lead_gap, co_val);
        on_segment_::handle_inserted(object, prior_, inserted_);
    }

    // . . . --------- . . . addend interval
    //      [-- it_ --)      has a common part with the first overval
    Combiner()(it_->second, co_val);
    on_segment_::handle_left_combined(object, it_++);
}



template<class Type, class Combiner>
void add_main(               Type&                object,
                    typename Type::interval_type& rest_interval, 
              const typename Type::codomain_type& co_val, 
                    typename Type::iterator&      it_,       
              const typename Type::iterator&      last_         )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::iterator      iterator;

    interval_type cur_interval;
    while(it_!=last_)
    {
        cur_interval = it_->first ;
        add_segment<Type,Combiner>(object, rest_interval, co_val, it_);
        // shrink interval
        rest_interval = left_subtract(rest_interval, cur_interval);
    }
}



template<class Type, class Combiner>
void add_rear(               Type&                object,
                    typename Type::interval_type& inter_val, 
              const typename Type::codomain_type& co_val, 
                    typename Type::iterator&      it_      )     
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_segment<Type,Combiner,
                                absorbs_neutrons<Type>::value,
                                Type::fineness>::type on_segment_;

    iterator prior_ = object.prior(it_);
    interval_type cur_itv = it_->first ;

    interval_type lead_gap = right_subtract(inter_val, cur_itv);
    if(!itl::is_empty(lead_gap))
    {   //         [lead_gap--- . . .
        // [prior)          [-- it_ ...
        iterator inserted_ = gap_insert<Type,Combiner>(object, prior_, lead_gap, co_val);
        on_segment_::handle_inserted(object, prior_, inserted_);
    }

    interval_type end_gap = left_subtract(inter_val, cur_itv);
    if(!itl::is_empty(end_gap))
    {
        // [----------------end_gap)
        //  . . . -- it_ --)
        Combiner()(it_->second, co_val);
        on_segment_::insert_at(object, it_, prior_, end_gap, co_val);
    }
    else
    {
        // only for the last there can be a right_resid: a part of *it_ right of x
        interval_type right_resid = left_subtract(cur_itv, inter_val);

        if(itl::is_empty(right_resid))
        {
            // [---------------)
            //      [-- it_ ---)
            Combiner()(it_->second, co_val);
            on_segment_::handle_preceeded_combined(object, prior_, it_);
        }
        else
        {
            // [--------------)
            //      [-- it_ --right_resid)
            const_cast<interval_type&>(it_->first) = right_subtract(it_->first, right_resid);

            //NOTE: This is NOT an insertion that has to take care for correct application of
            // the Combiner functor. It only reestablished that state after splitting the
            // 'it_' interval value pair. Using _map_insert<Combiner> does not work here.
            iterator insertion_ = object._insert(it_, value_type(right_resid, it_->second));
            on_segment_::handle_reinserted(object, insertion_);

            Combiner()(it_->second, co_val);
            on_segment_::handle_preceeded_combined(object, insertion_, it_);
        }
    }
}

//==============================================================================
//= Addition
//==============================================================================
template<class Type, class Combiner>
typename Type::iterator add(               Type&             object,
                            const typename Type::value_type& addend)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_style<Type,Type::fineness>::type on_style_;
    typedef typename on_neutric<Type,Combiner,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    const interval_type& inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return object.end();

    const codomain_type& co_val = addend.second;
    if(on_neutric_::is_absorbable(co_val))
        return object.end();

    std::pair<iterator,bool> insertion 
        = _map_insert<Type,Combiner>(object, inter_val, co_val);

    if(insertion.second)
        return on_style_::handle_inserted(object, insertion.first);
    else
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = object.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert(end_ == this->_map.upper_bound(inter_val));

        iterator it_ = first_;
        interval_type rest_interval = inter_val;

        Interval_Set::add_front              (object, rest_interval,         it_       );
        Interval_Map::add_main<Type,Combiner>(object, rest_interval, co_val, it_, last_);
        Interval_Map::add_rear<Type,Combiner>(object, rest_interval, co_val, it_       );
        return it_;
    }
}


template<class Type, class Combiner>
typename Type::iterator add(               Type&             object,
                                  typename Type::iterator    prior_,
                            const typename Type::value_type& addend)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_style<Type,Type::fineness>::type on_style_;
    typedef typename on_neutric<Type,Combiner,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    const interval_type& inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return prior_;

    const codomain_type& co_val = addend.second;
    if(on_neutric_::is_absorbable(co_val))
        return prior_;

    std::pair<iterator,bool> insertion 
        = informing_add<Type,Combiner>(object,prior_, inter_val, co_val);

    if(insertion.second)
        return on_style_::handle_inserted(object, insertion.first);
    else
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = object.equal_range(inter_val);
        iterator it_   = overlap.first,
                 last_ = overlap.second;
                 --last_;
        interval_type rest_interval = inter_val;

        Interval_Set::add_front              (object, rest_interval,         it_       );
        Interval_Map::add_main<Type,Combiner>(object, rest_interval, co_val, it_, last_);
        Interval_Map::add_rear<Type,Combiner>(object, rest_interval, co_val, it_       );

        return it_;
    }
}

//==============================================================================
//= Subtract detail
//==============================================================================

template<class Type>
void subtract_front(               Type&                object,
                    const typename Type::interval_type& inter_val, 
                          typename Type::iterator&      it_       )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::iterator      iterator;

    interval_type left_resid = right_subtract(it_->first, inter_val);

    if(!itl::is_empty(left_resid)) //                     [--- inter_val ---)
    {                              //[prior_) [left_resid)[--- it_ . . .
        iterator prior_ = object.prior(it_); 
        const_cast<interval_type&>(it_->first) = left_subtract(it_->first, left_resid);
        object._insert(prior_, value_type(left_resid, it_->second));
        // The segemnt *it_ is split at inter_val.first(), so as an invariant
        // segment *it_ is always "under" inter_val and a left_resid is empty.
    }
}


template<class Type, class Combiner>
void subtract_main(               Type&                object,
                   const typename Type::codomain_type& co_val, 
                         typename Type::iterator&      it_,       
                   const typename Type::iterator&      last_         )
{
    typedef typename on_segment<Type,Combiner,
                                absorbs_neutrons<Type>::value,
                                Type::fineness>::type on_segment_;
    while(it_ != last_)
    {
        Combiner()(it_->second, co_val);
        on_segment_::handle_left_combined(object, it_++);
    }
}


template<class Type, class Combiner>
void subtract_rear(               Type&                object,
                         typename Type::interval_type& inter_val, 
                   const typename Type::codomain_type& co_val, 
                         typename Type::iterator&      it_      )     
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_segment<Type,Combiner,
                                absorbs_neutrons<Type>::value,
                                Type::fineness>::type on_segment_;

    interval_type right_resid = left_subtract(it_->first, inter_val);

    if(itl::is_empty(right_resid))
    {
        Combiner()(it_->second, co_val);
        on_segment_::handle_combined(object, it_);
    }
    else
    {
        const_cast<interval_type&>(it_->first) = right_subtract(it_->first, right_resid);
        iterator next_ = object._insert(it_, value_type(right_resid, it_->second));
        Combiner()(it_->second, co_val);
        on_segment_::handle_succeeded_combined(object, it_, next_);
    }
}

//==============================================================================
//= Subtract
//==============================================================================
template<class Type, class Combiner>
void subtract(               Type&             object,
              const typename Type::value_type& minuend)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_style<Type,Type::fineness>::type on_style_;
    typedef typename on_neutric<Type,Combiner,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    interval_type inter_val = minuend.first;
    if(itl::is_empty(inter_val)) 
        return;

    const codomain_type& co_val = minuend.second;
    if(on_neutric_::is_absorbable(co_val)) 
        return;

    std::pair<iterator, iterator> exterior = object.equal_range(inter_val);
    if(exterior.first == exterior.second)
        return;

    iterator last_  = prior(exterior.second);
    iterator it_    = exterior.first;
    Interval_Map::subtract_front<Type>         (object, inter_val,         it_       );
    Interval_Map::subtract_main <Type,Combiner>(object,            co_val, it_, last_);
    Interval_Map::subtract_rear <Type,Combiner>(object, inter_val, co_val, it_       );
}


//==============================================================================
//= Insertion
//==============================================================================

template<class Type>
void insert_main(               Type&                object,
                       typename Type::interval_type& inter_val, 
                 const typename Type::codomain_type& co_val, 
                       typename Type::iterator&      it_,       
                 const typename Type::iterator&      last_    )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_style<Type,Type::fineness>::type on_style_;

    iterator end_   = last_ ; ++end_;
    iterator prior_ = it_, inserted_;
    if(prior_ != object.end())
        --prior_;
    interval_type rest_interval = inter_val, left_gap, cur_itv;
    interval_type last_interval = last_ ->first;

    while(it_ != end_  )
    {
        cur_itv = it_->first ;            
        left_gap = right_subtract(rest_interval, cur_itv);

        if(!itl::is_empty(left_gap))
        {
            inserted_ = object._insert(prior_, value_type(left_gap, co_val));
            on_style_::handle_inserted(object, inserted_, it_);
        }

        // shrink interval
        rest_interval = left_subtract(rest_interval, cur_itv);
        prior_ = it_;
        ++it_;
    }

    //insert_rear(rest_interval, co_val, last_):
    interval_type end_gap = left_subtract(rest_interval, last_interval);
    if(!itl::is_empty(end_gap))
    {
        inserted_ = object._insert(prior_, value_type(end_gap, co_val));
        it_ = on_style_::handle_inserted(object, inserted_);
    }
    else
        it_ = prior_;
}



template<class Type>
typename Type::iterator insert(               Type&             object,
                               const typename Type::value_type& addend)
{
    typedef typename Type::interval_type    interval_type;
    typedef typename Type::codomain_type    codomain_type;
    typedef typename Type::iterator         iterator;
    typedef typename Type::codomain_combine codomain_combine;
    typedef typename on_style<Type,Type::fineness>::type on_style_;
    typedef typename on_neutric<Type, codomain_combine,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return object.end();

    const codomain_type& co_val = addend.second;
    if(on_neutric_::is_absorbable(co_val)) 
        return object.end();

    std::pair<iterator,bool> insertion = object._insert(addend);

    if(insertion.second)
        return on_style_::handle_inserted(object, insertion.first);
    else
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = object.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert((++last_) == this->_map.upper_bound(inter_val));
        iterator it_ = first_;
        Interval_Map::insert_main(object, inter_val, co_val, it_, last_);
        return it_;
    }
}


template<class Type>
typename Type::iterator insert(               Type&             object,
                                     typename Type::iterator    prior_,
                               const typename Type::value_type& addend)
{
    typedef typename Type::interval_type    interval_type;
    typedef typename Type::codomain_type    codomain_type;
    typedef typename Type::iterator         iterator;
    typedef typename Type::codomain_combine codomain_combine;
    typedef typename on_style<Type,Type::fineness>::type on_style_;
    typedef typename on_neutric<Type, codomain_combine,
                                absorbs_neutrons<Type>::value>::type on_neutric_;

    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return prior_;

    const codomain_type& co_val = addend.second;
    if(on_neutric_::is_absorbable(co_val)) 
        return prior_;

    std::pair<iterator,bool> insertion 
        = informing_insert(object, prior_, inter_val, co_val);

    if(insertion.second)
        return on_style_::handle_inserted(object, insertion.first);
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = object.equal_range(inter_val);
        iterator it_    = overlap.first,
                 last_  = prior(overlap.second);
        Interval_Map::insert_main(object, inter_val, co_val, it_, last_);
        return it_;
    }
}


//==============================================================================
//= Erasure
//==============================================================================

template<class Type>
void erase_rest(               Type&                object,
                      typename Type::interval_type& inter_val, 
                const typename Type::codomain_type& co_val, 
                      typename Type::iterator&      it_,       
                const typename Type::iterator&      last_    )
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::iterator      iterator;

    // For all intervals within loop: it_->first are contained_in inter_val
    while(it_ != last_)
        if(it_->second == co_val)
            object.erase(it_++); 
        else it_++;

    //erase_rear:
    if(it_->second == co_val)
    {
        interval_type right_resid = left_subtract(it_->first, inter_val);
        if(itl::is_empty(right_resid))
            object.erase(it_);
        else
            const_cast<interval_type&>(it_->first) = right_resid;
    }
}

template<class Type>
void erase(Type& object, const typename Type::value_type& minuend)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::value_type    value_type;
    typedef typename Type::iterator      iterator;
    typedef typename on_neutric<Type,
                                typename Type::codomain_combine,
                                absorbs_neutrons<Type>::value>::type
                                         on_neutric_;

    interval_type inter_val = minuend.first;
    if(itl::is_empty(inter_val)) 
        return;

    const codomain_type& co_val = minuend.second;
    if(on_neutric_::is_absorbable(co_val))
        return;

    std::pair<iterator,iterator> exterior = object.equal_range(inter_val);
    if(exterior.first == exterior.second)
        return;

    iterator first_ = exterior.first, end_ = exterior.second, 
             last_  = object.prior(end_);
    iterator second_= first_; ++second_;

    if(first_ == last_) 
    {   //     [----inter_val----)
        //   .....first_==last_.....
        // only for the last there can be a right_resid: a part of *it_ right of minuend
        interval_type right_resid = left_subtract(first_->first, inter_val);

        if(first_->second == co_val)
        {   
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(!itl::is_empty(left_resid)) //            [----inter_val----)
            {                              // [left_resid)..first_==last_......
                const_cast<interval_type&>(first_->first) = left_resid;
                if(!itl::is_empty(right_resid))
                    object._insert(first_, value_type(right_resid, co_val));
            }
            else if(!itl::is_empty(right_resid))
                const_cast<interval_type&>(first_->first) = right_resid;
            else
                object.erase(first_);
        }
    }
    else
    {
        // first AND NOT last
        if(first_->second == co_val)
        {
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(itl::is_empty(left_resid))
                object.erase(first_);
            else
                const_cast<interval_type&>(first_->first) = left_resid;
        }

        erase_rest(object, inter_val, co_val, second_, last_);
    }
}

} // namespace Interval_Map

}} // namespace itl boost

#endif 


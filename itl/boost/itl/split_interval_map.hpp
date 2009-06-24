/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_split_interval_map_h_JOFA_000706__
#define __itl_split_interval_map_h_JOFA_000706__

#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/interval_base_map.hpp>
#include <boost/itl/interval_maps.hpp>
#include <boost/itl/split_interval_set.hpp>

namespace boost{namespace itl
{

/** implements a map as a map of intervals - on insertion overlapping intervals are split and associated values are combined. */
template
<
    typename DomainT,
    typename CodomainT,
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section  = ITL_SECTION_INSTANCE(itl::inplace_et, CodomainT), 
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
>
class split_interval_map:
    public interval_base_map<split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                             DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
{
public:
    typedef Traits traits;
    typedef split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> type;
    typedef       interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> joint_type;
    typedef type overloadable_type;

    typedef interval_base_map <split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                               DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> base_type;

	typedef DomainT domain_type;
	typedef CodomainT codomain_type;
    typedef Interval<DomainT,Compare> interval_type;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::element_type element_type;
    typedef typename base_type::segment_type segment_type;
    typedef typename base_type::domain_mapping_type    domain_mapping_type;
    typedef typename base_type::interval_mapping_type  interval_mapping_type;
    typedef typename base_type::ImplMapT ImplMapT;

    typedef typename base_type::codomain_combine codomain_combine;

    typedef interval_set<DomainT,Compare,Interval,Alloc> interval_set_type;
    typedef interval_set_type set_type;

    enum { fineness = 3 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    split_interval_map(): base_type() {}
    /// Copy constructor
    split_interval_map(const split_interval_map& src): base_type(src) {}

    explicit split_interval_map(domain_mapping_type& base_pair): base_type()
    { add(base_pair); }

    explicit split_interval_map(const value_type& value_pair): base_type()
    { add(value_pair); }

    /// Assignment operator
    template<class SubType>
    split_interval_map& operator =
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Compare,Combine,Section,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_map.
    template<class SubType>
    void assign(const interval_base_map<SubType,DomainT,CodomainT,
                                        Traits,Compare,Combine,Section,Interval,Alloc>& src)
    {
        typedef interval_base_map<SubType,DomainT,CodomainT,
                                  Traits,Compare,Combine,Section,Interval,Alloc> base_map_type;
        this->clear();
        // Can be implemented via _map.insert: Interval joining not necessary.
        const_FORALL(typename base_map_type, it, src) 
            this->_map.insert(*it); 
    }

    //==========================================================================
    //= Selection
    //==========================================================================

    //MEMO DESIGN DECISION: split_interval_map::operator[](interval): 
    // If used for selection will deliver a set of associated
    // values. It could only implemented for a single key value. But this 
    // would mislead the unexperienced user to hash a split_interval_map into 
    // singular intervals by inapt usage of op[]. So op[] will not be implemented
    // codomain_type& operator[](const interval_type& interval_of_keys)

private:
    // The following _suffixed function templates funx_ are implementations
    // correspoding unsuffixed function templates funx of the base class.
    // For funx_ want to be used in the base class via derived SubType pointer:
    // that()->funx_. So they must be either public or made accessible by a
    // friend declaration:
    friend class  
        interval_base_map
        <split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                            DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc  >;

    bool contains_(const value_type& x)const;

    template<class Combiner>
    void add_(const value_type&);

    template<class Combiner>
    void subtract_(const value_type&);

    void insert_(const value_type& value);
    void erase_(const value_type& value);

private:
    void handle_neighbours(const iterator& it){}
    
    void fill(const value_type&);
    iterator fill(iterator&, const interval_type&, const codomain_type&);

    template<class Combiner>
    void fill_gap(const value_type&);

    template<class Combiner>
    void fill_gap(iterator& prior_, const interval_type&, const CodomainT&);

    template<class Combiner>
    void add_main(interval_type& x_itv, const CodomainT& x_val, 
                 iterator& it, const iterator& end_it);
    template<class Combiner>
    void add_segment(const interval_type& x_itv, const CodomainT& x_val, 
                     iterator& it);

    void add_front(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    template<class Combiner>
    void add_rear(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    template<class Combiner>
    void subtract_main(const interval_type& x_itv, const CodomainT& x_val, 
                       iterator& it, iterator& end_it);

    void subtract_front(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    template<class Combiner>
    void subtract_rear(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    void insert_range(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it);
    //CL void insert_rear(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    void erase_rest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& lst_it);
} ;


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
bool split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::contains_(const value_type& interv_value)const
{
    interval_type interv = interv_value.KEY_VALUE;
    if(interv.empty()) 
        return true;

    type section;
    add_intersection(section, interv);
    return is_element_equal(section, type(interv_value));
}




template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::fill(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return;
    if(Traits::absorbs_neutrons && value.CONT_VALUE == codomain_combine::neutron())
        return;
    this->_map.insert(value);
}

template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
typename split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::iterator
    split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::fill(iterator& prior_, const interval_type& inter_val, const codomain_type& co_val)
{
    //collision free insert is asserted
    if(inter_val.empty())
        return this->_map.end();
    if(Traits::absorbs_neutrons && co_val == codomain_combine::neutron())
        return this->_map.end();

    return this->_map.insert(prior_, value_type(inter_val, co_val));
}

template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::fill_gap(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return;
    if(Traits::absorbs_neutrons && value.CONT_VALUE == Combiner::neutron())
        return;

	this->template map_insert<Combiner>(value.KEY_VALUE, value.CONT_VALUE);
}

template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::fill_gap(iterator& prior_, const interval_type& inter_val, const codomain_type& co_val)
{
    //collision free insert is asserted
    if(inter_val.empty())
        return;
    if(Traits::absorbs_neutrons && co_val == Combiner::neutron())
        return;

	this->template map_insert<Combiner>(prior_, inter_val, co_val);
}



//-----------------------------------------------------------------------------
// add<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_(const value_type& x)
{
    const interval_type& inter_val = x.KEY_VALUE;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = x.CONT_VALUE;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    std::pair<iterator,bool> insertion 
		= this->template map_insert<Combiner>(inter_val, co_val);

    if(!insertion.WAS_SUCCESSFUL)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator fst_it = this->_map.lower_bound(inter_val),
                 lst_it = insertion.ITERATOR;
        //assert(end_it == this->_map.upper_bound(inter_val));

		iterator it_ = fst_it;
		interval_type rest_interval = inter_val;

		add_front         (rest_interval, co_val, it_);
        add_main<Combiner>(rest_interval, co_val, it_, lst_it);
		add_rear<Combiner>(rest_interval, co_val, it_);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_front(const interval_type& inter_val, const CodomainT& co_val, iterator& first_)
{
	// If the collision sequence has a right residual 'right_resid' is will
	// be split, to provide a standardized start of algorithms:
	// The addend interval 'inver_val' covers the beginning of the collision sequence.

    // only for the first there can be a left_resid: a part of *first_ left of inter_val
    interval_type left_resid = right_subtract(first_->KEY_VALUE, inter_val);

	if(!left_resid.empty())
	{   //            [------------ . . .
		// [left_resid---fst_it --- . . .
		iterator prior_ = this->prior(first_);
		const_cast<interval_type&>(first_->KEY_VALUE).left_subtract(left_resid);
		//NOTE: Only splitting
		iterator insertion_ = this->_map.insert(prior_, value_type(left_resid, first_->CONT_VALUE));
	}

	//POST:
	// [----- inter_val ---- . . .
	// ...[-- first_ --...
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_main(interval_type& x_rest, const CodomainT& co_val, iterator& it, const iterator& lst_it)
{
	interval_type cur_interval;
    while(it!=lst_it)
    {
        cur_interval = it->KEY_VALUE ;
		add_segment<Combiner>(x_rest, co_val, it);
        // shrink interval
        x_rest.left_subtract(cur_interval);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_segment(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type lead_gap = right_subtract(inter_val, it_->KEY_VALUE);
	if(!lead_gap.empty())
	{
		// [------ . . .
		//     [-- it ...
		iterator prior_ = it_; 
		if(prior_ != this->_map.begin())
			this->template map_insert<Combiner>(--prior_, lead_gap, co_val);
		else
			this->template map_insert<Combiner>(lead_gap, co_val);
	}

	// . . . --------- . . . addend interval
	//      [-- it_ --)      has a common part with the first overval
	Combiner()(it_->CONT_VALUE, co_val);
	if(Traits::absorbs_neutrons && it_->CONT_VALUE == Combiner::neutron())
		this->_map.erase(it_++);
	else 
		++it_;
}



template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it)
{
	iterator prior_ = this->prior(it);
    interval_type cur_itv = (*it).KEY_VALUE ;

    interval_type lead_gap = right_subtract(inter_val, cur_itv);
	if(!lead_gap.empty())
		//         [------ . . .
		// [prior)     [-- it ...
		this->template map_insert<Combiner>(prior_, lead_gap, co_val);
	

    interval_type end_gap = left_subtract(inter_val, cur_itv);
	if(!end_gap.empty())
	{
		// [------------------)
		//      [-- it --)
		Combiner()(it->CONT_VALUE, co_val);

        if(Traits::absorbs_neutrons && it->CONT_VALUE == Combiner::neutron())
		{
            this->_map.erase(it);
			iterator inserted_ = this->template map_insert<Combiner>(prior_, end_gap, co_val);
		}
        else
		{
			fill_gap<Combiner>(it, end_gap, co_val);
		}
	}
	else
	{
		// only for the last there can be a right_resid: a part of *it right of x
		interval_type right_resid = left_subtract(cur_itv, inter_val);

		if(right_resid.empty())
		{
			// [---------------)
			//      [-- it ----)
			Combiner()(it->CONT_VALUE, co_val);

			if(Traits::absorbs_neutrons && it->CONT_VALUE == Combiner::neutron())
				this->_map.erase(it);
		}
		else
		{
			// [-------------)
			//      [-- it ---right_resid)
			const_cast<interval_type&>(it->KEY_VALUE).right_subtract(right_resid);

			//NOTE: This is NOT an insertion that has to take care for correct application of
			// the Combiner functor. It only reestablished that state after splitting the
			// 'it' interval value pair. Using map_insert<Combiner> does not work here.
			iterator insertion_ = this->_map.insert(it, value_type(right_resid, it->CONT_VALUE));

			Combiner()(it->CONT_VALUE, co_val);

			if(Traits::absorbs_neutrons && it->CONT_VALUE == Combiner::neutron())
				this->_map.erase(it);
		}
	}
}




//-----------------------------------------------------------------------------
// subtract<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_(const value_type& minuend)
{
    interval_type inter_val = minuend.KEY_VALUE;

    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = minuend.CONT_VALUE;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    iterator fst_it = this->_map.lower_bound(inter_val);
    if(fst_it==this->_map.end()) 
		return;
    iterator end_it = this->_map.upper_bound(inter_val);
    if(fst_it==end_it) 
		return;

	iterator lst_it = end_it; --lst_it;
	iterator it_    = fst_it;
	subtract_front         (inter_val, co_val, it_);
    subtract_main<Combiner>(inter_val, co_val, it_, lst_it);
	subtract_rear<Combiner>(inter_val, co_val, it_);
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_front(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type left_resid = right_subtract(it_->KEY_VALUE, inter_val);

    if(!left_resid.empty())
    {
		iterator prior_ = it_;
		if(prior_ != this->_map.begin())
			--prior_;
		const_cast<interval_type&>(it_->KEY_VALUE).left_subtract(left_resid);
		this->_map.insert(prior_, value_type(left_resid, it_->CONT_VALUE));
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_main(const interval_type& inter_val, const CodomainT& co_val, 
                    iterator& it_, iterator& lst_it)
{
    while(it_ != lst_it)
    {
        Combiner()(it_->CONT_VALUE, co_val);
        if(Traits::absorbs_neutrons && it_->CONT_VALUE==Combiner::neutron())
            this->_map.erase(it_++); 
        else ++it_;
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_)
{
    interval_type right_resid = left_subtract(it_->KEY_VALUE, inter_val);

    if(right_resid.empty())
    {
        CodomainT& cur_val = it_->CONT_VALUE ;
        Combiner()(cur_val, co_val);
        if(Traits::absorbs_neutrons && cur_val==Combiner::neutron())
            this->_map.erase(it_);
    }
    else
    {   // . . . ---)
		// . . . ---right_resid) : split it_
		const_cast<interval_type&>(it_->KEY_VALUE).right_subtract(right_resid);
		iterator next_ = this->_map.insert(it_, value_type(right_resid, it_->CONT_VALUE));
		Combiner()(it_->CONT_VALUE, co_val);
        if(Traits::absorbs_neutrons && it_->CONT_VALUE==Combiner::neutron())
            this->_map.erase(it_);
    }
}


//-----------------------------------------------------------------------------
// insert(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_(const value_type& addend)
{
    interval_type inter_val = addend.KEY_VALUE;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = addend.CONT_VALUE;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    std::pair<iterator,bool> insertion = this->_map.insert(addend);

    if(!insertion.WAS_SUCCESSFUL)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator fst_it = this->_map.lower_bound(inter_val),
                 lst_it = insertion.ITERATOR;
        //assert((++lst_it) == this->_map.upper_bound(inter_val));
		iterator it_ = fst_it;
		insert_range(inter_val, co_val, it_, lst_it);
    }
}


template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_range(const interval_type& inter_val, const CodomainT& co_val, iterator& it, iterator& lst_it)
{
	iterator end_it = lst_it; ++end_it;
	iterator prior_ = it, inserted_;
	if(prior_ != this->_map.end())
		--prior_;
    interval_type rest_interval = inter_val, left_gap, cur_itv;
	interval_type last_interval = lst_it->KEY_VALUE;

    while(it != end_it)
    {
        cur_itv = it->KEY_VALUE ;            
        left_gap = right_subtract(rest_interval, cur_itv);

        if(!left_gap.empty())
			inserted_ = this->_map.insert(prior_, value_type(left_gap, co_val));

        // shrink interval
        rest_interval.left_subtract(cur_itv);
		prior_ = it;
		++it;
    }

    //insert_rear(rest_interval, co_val, lst_it):
    interval_type end_gap = left_subtract(rest_interval, last_interval);
	if(!end_gap.empty())
		inserted_ = this->_map.insert(prior_, value_type(end_gap, co_val));
}


//-----------------------------------------------------------------------------
// erase(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_(const value_type& minuend)
{
    interval_type inter_val = minuend.KEY_VALUE;
    if(inter_val.empty()) 
        return;

    const CodomainT& co_val = minuend.CONT_VALUE;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    iterator fst_it = this->_map.lower_bound(inter_val);
    if(fst_it==this->_map.end()) 
		return;
    iterator end_it = this->_map.upper_bound(inter_val);
    if(fst_it==end_it) 
		return;

	iterator lst_it = end_it; --lst_it;

    iterator snd_it = fst_it; snd_it++;
    if(fst_it == lst_it) 
    {
        // only for the last there can be a right_resid: a part of *it right of minuend
        interval_type right_resid;  (*fst_it).KEY_VALUE.left_subtract(right_resid, inter_val);

        if(fst_it->CONT_VALUE == co_val)
        {
			interval_type left_resid = right_subtract(fst_it->KEY_VALUE, inter_val);
			if(!left_resid.empty())
			{
				const_cast<interval_type&>(fst_it->KEY_VALUE) = left_resid;
				if(!right_resid.empty())
					this->_map.insert(fst_it, value_type(right_resid, co_val));
			}
			else if(!right_resid.empty())
				const_cast<interval_type&>(fst_it->KEY_VALUE) = right_resid;
			else
                this->_map.erase(fst_it);
        }
    }
    else
    {
        // first AND NOT last
        if(fst_it->CONT_VALUE == co_val)
		{
			interval_type left_resid = right_subtract(fst_it->KEY_VALUE, inter_val);
			if(left_resid.empty())
				this->_map.erase(fst_it);
			else
				const_cast<interval_type&>(fst_it->KEY_VALUE) = left_resid;
		}

        erase_rest(inter_val, co_val, snd_it, lst_it);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_rest(const interval_type& inter_val, const CodomainT& co_val, 
                 iterator& it_, iterator& lst_it)
{
    // For all intervals within loop: it_->KEY_VALUE are contained_in inter_val
    while(it_ != lst_it)
        if((*it_).CONT_VALUE == co_val)
            this->_map.erase(it_++); 
        else it_++;

	//erase_rear:
    if(it_->CONT_VALUE == co_val)
	{
        interval_type right_resid = left_subtract(it_->KEY_VALUE, inter_val);
		if(right_resid.empty())
			this->_map.erase(it_);
		else
			const_cast<interval_type&>(it_->KEY_VALUE) = right_resid;
	}
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_set<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    enum{value = true}; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = has_inverse<CodomainT>::value; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = Traits::absorbs_neutrons; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = Traits::is_total; 
};


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    static std::string apply()
    { 
        return "sp_itv_map<"+ type_to_string<DomainT>::apply()   + ","
                            + type_to_string<CodomainT>::apply() + ","
                            + type_to_string<Traits>::apply()    +">"; 
    }
};


}} // namespace itl boost

#endif



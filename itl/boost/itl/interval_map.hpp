/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/

/* ------------------------------------------------------------------
class interval_map
--------------------------------------------------------------------*/
#ifndef __interval_map_h_JOFA_080705__
#define __interval_map_h_JOFA_080705__

#include <boost/assert.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_base_map.hpp>
#include <boost/itl/interval_maps.hpp>

namespace boost{namespace itl
{

/// implements a map as a map of intervals - on insertion overlapping intervals are split and associated values are combined.
/** 
    Template-class <b>interval_map</b>
    implements a map as a map of intervals - On insertion overlapping intervals are
    <b>split</b> and associated values are combined.
   
    Template parameter <b>DomainT</b>: Domain type of the map. Also type of the
    map's keys.

      Suitable as domain types are all datatypes that posess a partial order.
    In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
    atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.

    Datatypes for the codomain parameter have to <b>implement</b> operations
    <tt>+=</tt>, <tt>-=</tt>, <tt>==</tt> (equality) and <tt>CodomainT()</tt> (default constructor).

    The default constructor <tt>CodomainT()</tt> has to contruct a neutral element
    such that the following holds:

      If <tt>x = y; y += CodomainT();</tt> then <tt>x==y;</tt> and
      If <tt>x = y; y -= CodomainT();</tt> then <tt>x==y;</tt>

      Template parameter <b>Interval=itl::interval</b>: Template type of interval used
    to implement the map. The default <b>itl::interval</b> uses the
    interval class template that comes with this library. Own implementation of interval
    classes are possible (but not trivial).

    <b>interval_map</b> implements a map <tt>map<DomainT, CodomainT></tt> as a map
    of intervals <tt>map<interval<DomainT>, CodomainT, ExclusiveLessT<Interval> ></tt>

    Interval maps <tt>interval_map<DomainT,CodomainT></tt> can be used similar (and in many
    aspects exactly like) common stl-maps. Unlike to stl-maps where you store
    a value for every key an interval map stores a contents value for an interval of
    keys. In it's degenerated form the key intervals contain a single element
    only. Then the interval map works like a normal stl-map. But if you work in problem
    domains where you associate values to large contiguous intervals, interval maps
    are very useful and efficient.


    Class <tt>interval_base_map</tt>  yields the following benefits:
  
    <ul>
        <li> A set of intervals is conceived as the domain set of the map. 
            The complexity involved with
            operations on intervals maps is encapsulated. The user of the class who
            wants to perform operations on interval maps is no more concerned
            with questions of overlapping, joining and bordering intervals.
        <li>
            <b>interval_map</b> gives an efficient implementation of maps consisting
            of larger contiguous chunks. Very large, even uncountably infinite maps
            can be represented in a compact way and handled efficiently.
        <li>
            <b>interval_map</b> serves as a overlay- or collision-computer.
    </ul>

    <b>interval_map as overlay computer</b>

    An object <tt>interval_map<int,int> overlays;</tt> computes the overlays or 
    collisions of intervalls which have been inserted into it, if they are
    associated with the <tt>int</tt>-value <tt>1</tt> as the codommain value.

    If a newly inserted interval overlaps with intervals which are already in the
    map, the interval is split up at the borders of the collisions. The assiciated
    values of the overlapping intervals are incremented by 1, thus counting
    the numbers of overlaps.

    If sets are used as codomain types, interval_maps will compute unions of
    associated maps in case of interval collisions.

      <b>Restrictions: </b>
    
    A small number of functions can only be used for <b>discrete</b> domain datatypes 
    (<tt>short, int, Date</tt> etc.) that implement operators <tt>++</tt> and <tt>--</tt>.

    These functions are tagged in the documentation. Using such functions
    for continuous domain datatypes yields compiletime errors. C.f. getting
    the <tt>first()</tt> element of a left open interval makes sense for intervals of
    int but not for intervals of double.

    @author  Joachim Faulhaber
*/
template
<
    typename DomainT,
    typename CodomainT,
    class Traits = itl::neutron_absorber,
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_ALLOC   Alloc    = std::allocator
>
class interval_map:

    public interval_base_map<interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>, 
                             DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
{
public:
    typedef Traits traits;
    typedef interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc> type;
    typedef type joint_type;
    typedef interval_base_map <interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>, 
                               DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc> base_type;

    typedef interval_map<DomainT,CodomainT,itl::neutron_absorber,Interval,Compare,Combine,Alloc>
            neutron_absorber_type;

    typedef Interval<DomainT,Compare> interval_type;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::mapping_type mapping_type;
    typedef typename base_type::mapping_pair_type mapping_pair_type;
    typedef typename base_type::ImplMapT ImplMapT;

    typedef interval_set<DomainT,Interval,Compare,Alloc> interval_set_type;
    typedef interval_set_type set_type;

    /// Default constructor for the empty map 
    interval_map(): base_type() {}
    /// Copy constructor
    interval_map(const interval_map& src): base_type(src) {}


    /// Copy constructor for base_type
    template<class SubType>
    explicit interval_map
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Interval,Compare,Combine,Alloc>& src)
    { assign(src); }

    explicit interval_map(mapping_pair_type& base_pair): base_type()
    { add(base_pair); }

    explicit interval_map(const value_type& value_pair): base_type()
    { add(value_pair); }

    /// Assignment operator
    template<class SubType>
    interval_map& operator =
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Interval,Compare,Combine,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_map.
    template<class SubType>
    void assign(const interval_base_map<SubType,DomainT,CodomainT,
                                        Traits,Interval,Compare,Combine,Alloc>& src)
    {
        typedef interval_base_map<SubType,DomainT,CodomainT,
                                  Traits,Interval,Compare,Combine,Alloc> base_map_type;
        this->clear();
        // Can be implemented via _map.insert: Interval joining not necessary.
        const_FORALL(typename base_map_type, it, src) 
            this->add(*it); 
    }
 
private:
	// The following _suffixed function templates funx_ are implementations
	// correspoding unsuffixed function templates funx of the base class.
	// For funx_ want to be used in the base class via derived SubType pointer:
	// that()->funx_. So they must be either public or made accessible by a
	// friend declaration:
	friend class base_type;

    bool contains_(const value_type& x)const;

    template<class Combiner>
    void add_(const value_type&);

    template<class Combiner>
    void subtract_(const value_type&);

    void insert_(const value_type& value);
    void erase_(const value_type& value);

private:
    bool insertable(const value_type& value)const
    { 
        return !value.KEY_VALUE.empty() 
            && !(Traits::absorbs_neutrons && value.CONT_VALUE == CodomainT()); 
    }

    bool join_left(iterator& it);
    bool join_right(iterator& it);
    void join_neighbours(iterator& it){ join_left(it); join_right(it); };
    bool joinable(const iterator& some, const iterator& next)const;
    iterator joint_insert(iterator& some, const iterator& next);

    template<class Combiner>
    iterator fill_gap_join_left(const value_type&);

    template<class Combiner>
    iterator fill_gap_join_both(const value_type&);

    iterator fill_join_left(const value_type&);
    iterator fill_join_both(const value_type&);

    template<class Combiner>
    void add_rest(const interval_type& x_itv, const CodomainT& x_val, 
		          iterator& it, iterator& end_it);

    template<class Combiner>
    void add_rear(const interval_type& x_itv, const CodomainT& x_val, 
		          iterator& it);

    template<class Combiner>
    void subtract_rest(const interval_type& x_itv, const CodomainT& x_val, 
		               iterator& it, iterator& end_it);

    void insert_rest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it);
    void insert_rear(const interval_type& x_itv, const CodomainT& x_val, iterator& it);

    void erase_rest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it);
} ;


template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
bool interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
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
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
bool interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::joinable(const iterator& some, const iterator& next)const
{
    // assert: next != end && some++ == next
    return some->KEY_VALUE.touches(next->KEY_VALUE) 
        && some->CONT_VALUE == next->CONT_VALUE; 
}

template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
typename interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>::iterator 
    interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::joint_insert(iterator& left_it, const iterator& right_it)
{
    // both left and right are in the set and they are neighbours
    BOOST_ASSERT(right_it != _map.end());
    BOOST_ASSERT(left_it->KEY_VALUE.exclusive_less(right_it->KEY_VALUE));
    BOOST_ASSERT(left_it->KEY_VALUE.touches(right_it->KEY_VALUE));

    interval_type interval    = left_it->KEY_VALUE;
    //It has to be a copy, because is location will be erased 
    //JODO: Try optimizing inplace.
    CodomainT value = left_it->CONT_VALUE;
    interval.extend(right_it->KEY_VALUE);

    this->_map.erase(left_it);
    this->_map.erase(right_it);
    
    std::pair<iterator,bool> insertion = this->_map.insert(make_pair(interval, value));
    iterator new_it = insertion.ITERATOR;
    BOOST_ASSERT(insertion.WAS_SUCCESSFUL);
    BOOST_ASSERT(new_it!=this->_map.end());
    left_it = new_it;
    return new_it;
}



template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
bool interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::join_left(iterator& it)
{
    if(it == this->_map.begin())
        return false;

    // there is a predecessor
    iterator it_pred = it; it_pred-- ;

    if(joinable(it_pred, it)) 
    {
        iterator it_leftExtended = joint_insert(it_pred, it);
        //CAUTION: it is now invalidated
        it = it_leftExtended;
        return true;
    }

    return false;
}

template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
bool interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::join_right(iterator& it)
{
    if(it == this->_map.end())
        return false;

    // there is a successor
    iterator it_succ = it; it_succ++ ;

    if(it_succ != this->_map.end() && joinable(it, it_succ)) 
    {
        joint_insert(it, it_succ);
        return true;
    }

    return false;
}



template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
typename interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>::iterator
interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::fill_join_left(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return this->_map.end();
    if(Traits::absorbs_neutrons && value.CONT_VALUE == CodomainT())
        return this->_map.end();

    std::pair<iterator,bool> insertion = this->_map.insert(value);

    join_left(insertion.ITERATOR);

    return insertion.ITERATOR; //JODO return value currently unused
}

template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
typename interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>::iterator
interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::fill_join_both(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return this->_map.end();
    if(Traits::absorbs_neutrons && value.CONT_VALUE == CodomainT())
        return this->_map.end();

    std::pair<iterator,bool> insertion = this->_map.insert(value);

    join_neighbours(insertion.ITERATOR);

    return insertion.ITERATOR;
}

//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
typename interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>::iterator
interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::fill_gap_join_left(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return this->_map.end();
    if(Traits::absorbs_neutrons && value.CONT_VALUE == CodomainT())
        return this->_map.end();

    std::pair<iterator,bool> insertion;
    if(Traits::emits_neutrons)
    {
        CodomainT added_val = CodomainT();
        Combiner()(added_val, value.CONT_VALUE);
        insertion = this->_map.insert(value_type(value.KEY_VALUE, added_val));
    }
    else
        insertion = this->_map.insert(value);

    join_left(insertion.ITERATOR);

    return insertion.ITERATOR; //JODO return value currently unused
}

template <typename DomainT, typename CodomainT, class Traits,
          template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
typename interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>::iterator
interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::fill_gap_join_both(const value_type& value)
{
    //collision free insert is asserted
    if(value.KEY_VALUE.empty())
        return this->_map.end();
    if(Traits::absorbs_neutrons && value.CONT_VALUE == CodomainT())
        return this->_map.end();

    std::pair<iterator,bool> insertion;
    if(Traits::emits_neutrons)
    {
        CodomainT added_val = CodomainT();
        Combiner()(added_val, value.CONT_VALUE);
        insertion = this->_map.insert(value_type(value.KEY_VALUE, added_val));
    }
    else
        insertion = this->_map.insert(value);

    join_neighbours(insertion.ITERATOR);

    return insertion.ITERATOR;
}


//-----------------------------------------------------------------------------
// add<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::add_(const value_type& x)
{
    const interval_type& x_itv = x.KEY_VALUE;
    if(x_itv.empty()) 
        return;

    const CodomainT& x_val = x.CONT_VALUE;
    if(Traits::absorbs_neutrons && x_val==CodomainT()) 
        return;

    std::pair<iterator,bool> insertion;
    if(Traits::emits_neutrons)
    {
        CodomainT added_val = CodomainT();
        Combiner()(added_val, x_val);
        insertion = this->_map.insert(value_type(x_itv, added_val));
    }
    else
        insertion = this->_map.insert(x);

    if(insertion.WAS_SUCCESSFUL)
        join_neighbours(insertion.ITERATOR);
    else
    {
        // Detect the first and the end iterator of the collision sequence
        iterator fst_it = this->_map.lower_bound(x_itv);
        iterator end_it = insertion.ITERATOR;
        if(end_it != this->_map.end())
            end_it++; 
        //assert(end_it == this->_map.upper_bound(x_itv));

        interval_type fst_itv = (*fst_it).KEY_VALUE;
        CodomainT cur_val     = (*fst_it).CONT_VALUE;


        interval_type leadGap; x_itv.left_surplus(leadGap, fst_itv);
        // this is a new Interval that is a gap in the current map
        //The first collision interval may grow by joining neighbours after insertion

        // only for the first there can be a leftResid: a part of *it left of x
        interval_type leftResid;  
        fst_itv.left_surplus(leftResid, x_itv);

        // handle special case for first

        interval_type interSec;
        fst_itv.intersect(interSec, x_itv);

        CodomainT cmb_val = cur_val;
        Combiner()(cmb_val, x_val);

        iterator snd_it = fst_it; snd_it++; 

        if(snd_it == end_it) 
        {
            // first == last

            interval_type endGap; x_itv.right_surplus(endGap, fst_itv);
            // this is a new Interval that is a gap in the current map

            // only for the last there can be a rightResid: a part of *it right of x
            interval_type rightResid;  (*fst_it).KEY_VALUE.right_surplus(rightResid, x_itv);

            this->_map.erase(fst_it);
            fill_join_left(value_type(leftResid,  cur_val));

            if(endGap.empty() && rightResid.empty())
                fill_join_both(value_type(interSec,   cmb_val));
            else
                fill_join_left(value_type(interSec,   cmb_val));

            if(!leadGap.empty())
                fill_gap_join_both<Combiner>(value_type(leadGap, x_val));
            if(!endGap.empty())
                fill_gap_join_both<Combiner>(value_type(endGap, x_val));
            else
                fill_join_left(value_type(rightResid, cur_val));
        }
        else
        {
            this->_map.erase(fst_it);
            fill_join_left(value_type(leftResid, cur_val));
            fill_join_left(value_type(interSec,  cmb_val));

            if(!leadGap.empty())
                fill_gap_join_both<Combiner>(value_type(leadGap, x_val));

            // shrink interval
            interval_type x_rest(x_itv);
            x_rest.left_subtract(fst_itv);

            add_rest<Combiner>(x_rest, x_val, snd_it, end_it);
        }
    }
}

template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::add_rest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it)
{
    iterator nxt_it = it; nxt_it++;
    interval_type x_rest = x_itv, left_gap, common, cur_itv;

    while(nxt_it!=end_it)
    {
        cur_itv = (*it).KEY_VALUE ;            
        x_rest.left_surplus(left_gap, cur_itv);

        Combiner()(it->CONT_VALUE, x_val);
        fill_gap_join_left<Combiner>(value_type(left_gap, x_val)); //A posteriori

        if(Traits::absorbs_neutrons && it->CONT_VALUE == CodomainT())
            this->_map.erase(it++);
        else
        {
            // after filling that gap there may be another joining opportunity
            join_left(it);
            it++;
        }

        // shrink interval
        x_rest.left_subtract(cur_itv);
        nxt_it++;
    }

    add_rear<Combiner>(x_rest, x_val, it);
}

template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::add_rear(const interval_type& x_rest, const CodomainT& x_val, iterator& it)
{
    interval_type cur_itv = (*it).KEY_VALUE ;
    CodomainT     cur_val = (*it).CONT_VALUE ;

    interval_type lead_gap;
    x_rest.left_surplus(lead_gap, cur_itv);

    interval_type common;
    cur_itv.intersect(common, x_rest);

    CodomainT cmb_val = cur_val;
    Combiner()(cmb_val, x_val);

    interval_type end_gap; 
    x_rest.right_surplus(end_gap, cur_itv);
    
    // only for the last there can be a rightResid: a part of *it right of x
    interval_type right_resid;  
    cur_itv.right_surplus(right_resid, x_rest);

    this->_map.erase(it);
    if(end_gap.empty() && right_resid.empty())
        fill_join_both(value_type(common,   cmb_val));
    else
        fill_join_left(value_type(common,   cmb_val));

    if(!lead_gap.empty())
        fill_gap_join_both<Combiner>(value_type(lead_gap, x_val));
    if(!end_gap.empty())
        fill_gap_join_both<Combiner>(value_type(end_gap, x_val));
    else
        fill_join_left(value_type(right_resid, cur_val));
}


//-----------------------------------------------------------------------------
// subtract<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::subtract_(const value_type& x)
{
    const interval_type& x_itv = x.KEY_VALUE;

    if(x_itv.empty()) 
        return;

    const CodomainT& x_val = x.CONT_VALUE;
    if(Traits::absorbs_neutrons && x_val==CodomainT()) 
        return;

    iterator fst_it = this->_map.lower_bound(x_itv);
    if(fst_it==this->_map.end()) return;
    iterator end_it = this->_map.upper_bound(x_itv);
    if(fst_it==end_it) return;

    interval_type fst_itv = (*fst_it).KEY_VALUE ;
    // must be copies because fst_it will be erased
    CodomainT fst_val = (*fst_it).CONT_VALUE ;

    // only for the first there can be a leftResid: a part of *it left of x
    interval_type leftResid;  
    fst_itv.left_surplus(leftResid, x_itv);

    // handle special case for first

    interval_type interSec;
    fst_itv.intersect(interSec, x_itv);

    CodomainT cmb_val = fst_val;
    Combiner()(cmb_val, x_val);

    iterator snd_it = fst_it; snd_it++;
    if(snd_it == end_it) 
    {
        // only for the last there can be a rightResid: a part of *it right of x
        interval_type rightResid;  (*fst_it).KEY_VALUE.right_surplus(rightResid, x_itv);

        this->_map.erase(fst_it);
        fill_join_left(value_type(leftResid, fst_val));

        if(rightResid.empty())
            fill_join_both(value_type(interSec,   cmb_val));
        else
            fill_join_left(value_type(interSec,   cmb_val));

        fill_join_both(value_type(rightResid, fst_val));
    }
    else
    {
        // first AND NOT last
        this->_map.erase(fst_it);
        
        fill_join_left(value_type(leftResid, fst_val));
        fill_join_left(value_type(interSec,  cmb_val));

        // shrink interval
        interval_type x_rest(x_itv);
        x_rest.left_subtract(fst_itv);

        subtract_rest<Combiner>(x_rest, x_val, snd_it, end_it);
    }
}



template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
    template<class Combiner>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::subtract_rest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it)
{
    iterator nxt_it=it; nxt_it++;

    while(nxt_it!=end_it)
    {
        CodomainT& cur_val = (*it).CONT_VALUE ;
        Combiner()(cur_val, x_val);

        if(Traits::absorbs_neutrons && cur_val==CodomainT())
            this->_map.erase(it++); 
        else
        {
            join_left(it);
            it++;
        }

        nxt_it=it; nxt_it++;
    }

    // it refers the last overlaying intervals of x_itv
    const interval_type& cur_itv = (*it).KEY_VALUE ;

    interval_type rightResid; 
    cur_itv.right_surplus(rightResid, x_itv);

    if(rightResid.empty())
    {
        CodomainT& cur_val = (*it).CONT_VALUE ;
        Combiner()(cur_val, x_val);
        if(Traits::absorbs_neutrons && cur_val==CodomainT())
            this->_map.erase(it);
        else
        {
            join_left(it);
            // cur_val is the last -= modified value. There may be an
            // adjoint right neighbour that is now joinable.
            if(it != this->_map.end())
            {
                iterator out_it = it; out_it++;
                if(out_it != this->_map.end() && joinable(it, out_it))
                    joint_insert(it,out_it);
            }
        }
    }
    else
    {
        CodomainT cur_val = (*it).CONT_VALUE ;
        CodomainT cmb_val = cur_val ;
        Combiner()(cmb_val, x_val);
        interval_type interSec; 
        cur_itv.intersect(interSec, x_itv);

        this->_map.erase(it);
        if(rightResid.empty())
            fill_join_both(value_type(interSec, cmb_val));
        else
            fill_join_left(value_type(interSec, cmb_val));

        fill_join_both(value_type(rightResid, cur_val));
    }
}



//-----------------------------------------------------------------------------
// insert(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::insert_(const value_type& x)
{
    const interval_type& x_itv = x.KEY_VALUE;
    if(x_itv.empty()) 
        return;

    const CodomainT& x_val = x.CONT_VALUE;
    if(Traits::absorbs_neutrons && x_val==CodomainT()) 
        return;

    std::pair<typename ImplMapT::iterator,bool> 
        insertion = this->_map.insert(x);

    if(insertion.WAS_SUCCESSFUL)
        join_neighbours(insertion.ITERATOR);
    else
    {
        // Detect the first and the end iterator of the collision sequence
        iterator fst_it = this->_map.lower_bound(x_itv);
        iterator end_it = insertion.ITERATOR;
        if(end_it != this->_map.end())
            end_it++; 
        //assert(end_it == this->_map.upper_bound(x_itv));

        interval_type fst_itv = (*fst_it).KEY_VALUE ;
        interval_type leadGap; x_itv.left_surplus(leadGap, fst_itv);
        // this is a new Interval that is a gap in the current map

        // only for the first there can be a leftResid: a part of *it left of x
        interval_type leftResid;  fst_itv.left_surplus(leftResid, x_itv);

        // handle special case for first

        interval_type interSec;
        fst_itv.intersect(interSec, x_itv);

        iterator snd_it = fst_it; snd_it++;
        if(snd_it == end_it) 
        {
            //Fill gap after iterator compare bcause iterators are modified by joining
            if(!leadGap.empty())
                fill_join_both(value_type(leadGap, x_val));

            interval_type endGap; x_itv.right_surplus(endGap, fst_itv);
            // this is a new Interval that is a gap in the current map
            fill_join_both(value_type(endGap, x_val));
        }
        else
        {
            if(!leadGap.empty())
                fill_join_both(value_type(leadGap, x_val));

            // shrink interval
            interval_type x_rest(x_itv);
            x_rest.left_subtract(fst_itv);

            insert_rest(x_rest, x_val, snd_it, end_it);
        }
    }
}


template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::insert_rest(const interval_type& x_itv, const CodomainT& x_val, 
                  iterator& it, iterator& end_it)
{
    iterator nxt_it = it; nxt_it++;
    interval_type x_rest = x_itv, gap, common, cur_itv;

    for(; nxt_it!=end_it; ++it, ++nxt_it)
    {
        cur_itv = (*it).KEY_VALUE ;            
        x_rest.left_surplus(gap, cur_itv);

        if(!gap.empty())
        {
            fill_join_left(value_type(gap, x_val));
            // after filling that gap there may be another joining opportunity
            join_left(it);
        }

        // shrink interval
        x_rest.left_subtract(cur_itv);
    }

    insert_rear(x_rest, x_val, it);
}

template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::insert_rear(const interval_type& x_rest, const CodomainT& x_val, 
                  iterator& it)
{
    interval_type cur_itv = (*it).KEY_VALUE ;

    interval_type left_gap;
    x_rest.left_surplus(left_gap, cur_itv);

    if(!left_gap.empty())
    {
        fill_join_left(value_type(left_gap, x_val));
        // after filling that gap there may be another joining opportunity
        join_left(it);
    }

    interval_type common;
    cur_itv.intersect(common, x_rest);

    interval_type end_gap; 
    x_rest.right_surplus(end_gap, cur_itv);

    fill_join_both(value_type(end_gap, x_val));
}


//-----------------------------------------------------------------------------
// erase(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::erase_(const value_type& x)
{
    const interval_type& x_itv = x.KEY_VALUE;
    if(x_itv.empty()) 
        return;

    const CodomainT& x_val = x.CONT_VALUE;
    if(Traits::absorbs_neutrons && x_val==CodomainT()) 
        return;

    iterator fst_it = this->_map.lower_bound(x_itv);
    if(fst_it==this->_map.end()) return;
    iterator end_it = this->_map.upper_bound(x_itv);
    if(fst_it==end_it) return;

    interval_type fst_itv = (*fst_it).KEY_VALUE ;
    // must be copies because fst_it will be erased
    CodomainT fst_val = (*fst_it).CONT_VALUE ;

    // only for the first there can be a leftResid: a part of *it left of x
    interval_type leftResid;  
    fst_itv.left_surplus(leftResid, x_itv);

    // handle special case for first

    interval_type interSec;
    fst_itv.intersect(interSec, x_itv);

    iterator snd_it = fst_it; snd_it++;
    if(snd_it == end_it) 
    {
        // only for the last there can be a rightResid: a part of *it right of x
        interval_type rightResid;  (*fst_it).KEY_VALUE.right_surplus(rightResid, x_itv);

        if(!interSec.empty() && fst_val == x_val)
        {
            this->_map.erase(fst_it);
            insert_(value_type(leftResid,  fst_val));
            // erased: insert(value_type(interSec,  cmb_val));
            insert_(value_type(rightResid, fst_val));
        }
    }
    else
    {
        // first AND NOT last
        if(!interSec.empty() && fst_val == x_val)
        {
            this->_map.erase(fst_it);
            insert_(value_type(leftResid, fst_val));
            // erased: insert(value_type(interSec,  cmb_val));
        }

        erase_rest(x_itv, x_val, snd_it, end_it);
    }
}


template <typename DomainT, typename CodomainT, class Traits, template<class,ITL_COMPARE>class Interval, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_ALLOC Alloc>
void interval_map<DomainT,CodomainT,Traits,Interval,Compare,Combine,Alloc>
    ::erase_rest(const interval_type& x_itv, const CodomainT& x_val, 
                 iterator& it, iterator& end_it)
{
    iterator nxt_it=it; nxt_it++;

    // For all intervals within loop: it->KEY_VALUE are contained_in x_itv
    while(nxt_it!=end_it)
    {
        if((*it).CONT_VALUE == x_val)
            this->_map.erase(it++); 
        else it++;

        nxt_it=it; nxt_it++;
    }

    // it refers the last overlaying intervals of x_itv
    interval_type cur_itv = (*it).KEY_VALUE ;
    // Has to be a copy, cause 'it' will be erased
    CodomainT cur_val = (*it).CONT_VALUE;

    interval_type rightResid; 
    cur_itv.right_surplus(rightResid, x_itv);

    if(rightResid.empty())
    {
        if(cur_val == x_val)
            this->_map.erase(it);
    }
    else
    {
        interval_type interSec; 
        cur_itv.intersect(interSec, x_itv);

        if(!interSec.empty() && cur_val == x_val)
        {
            this->_map.erase(it);
            //erased: insert(value_type(interSec, cmb_val));
            insert_(value_type(rightResid, cur_val));
        }
    }
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------

template <class KeyT, class DataT, class Traits>
struct is_set<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = true}; };

template <class KeyT, class DataT, class Traits>
struct is_map<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = true}; };

template <class KeyT, class DataT, class Traits>
struct is_interval_container<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = true}; };

template <class KeyT, class DataT, class Traits>
struct is_interval_splitter<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = true}; };

template <class KeyT, class DataT, class Traits>
struct is_neutron_absorber<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = Traits::absorbs_neutrons}; };

template <class KeyT, class DataT, class Traits>
struct is_neutron_emitter<itl::interval_map<KeyT,DataT,Traits> >
{ enum{value = Traits::emits_neutrons}; };

template <class KeyT, class DataT, class Traits>
struct type_to_string<itl::interval_map<KeyT,DataT,Traits> >
{
    static std::string apply()
    { 
        return "itv_map<"+ type_to_string<KeyT>::apply()  + ","
                         + type_to_string<DataT>::apply() + ","
                         + type_to_string<Traits>::apply() +">"; 
    }
};

}} // namespace itl boost

#endif



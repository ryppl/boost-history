/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __interval_base_map_h_JOFA_990223__
#define __interval_base_map_h_JOFA_990223__

#include <limits>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>

#ifdef USE_CONCEPTS
#include <bits/concepts.h>
#endif

#include <boost/itl/map.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_sets.hpp>


#define const_FOR_IMPLMAP(iter) for(typename ImplMapT::const_iterator iter=_map.begin(); (iter)!=_map.end(); (iter)++)
#define FOR_IMPLMAP(iter) for(typename ImplMapT::iterator iter=_map.begin(); (iter)!=_map.end(); (iter)++)

namespace boost{namespace itl
{

template<class DomainT, class CodomainT>
struct mapping_pair
{
    DomainT   key;
    CodomainT data;

    mapping_pair(const DomainT& key_value, const CodomainT& data_value)
        :key(key_value), data(data_value){}

    mapping_pair(const std::pair<DomainT,CodomainT>& std_pair)
        :key(std_pair.first), data(std_pair.second){}
};



/** Implements a map as a map of intervals (base class) */
template
<
    class SubType,
    typename DomainT,
    typename CodomainT,
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section  = ITL_SECTION_INSTANCE(itl::inplace_et, CodomainT), 
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
>
#ifdef USE_CONCEPTS
    requires std::LessThanComparable<DomainT>
#endif
class interval_base_map
{
public:
    //==========================================================================
    //= Associated types
    //==========================================================================
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc>
                              type;

    /// The designated \e derived or \e sub_type of this base class
    typedef SubType sub_type;

    /// Auxilliary type for overloadresolution
    typedef type overloadable_type;

    /// Traits of an itl map
    typedef Traits traits;

    //--------------------------------------------------------------------------
    //- Associated types: Related types
    //--------------------------------------------------------------------------
    /// The atomized type representing the corresponding container of elements
    typedef typename itl::map<DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Alloc> atomized_type;

    //--------------------------------------------------------------------------
    //- Associated types: Data
    //--------------------------------------------------------------------------
    /// Domain type (type of the keys) of the map
    typedef DomainT   domain_type;
    /// Domain type (type of the keys) of the map
    typedef CodomainT codomain_type;
    /// Auxiliary type to help the compiler resolve ambiguities when using std::make_pair
    typedef mapping_pair<domain_type,codomain_type> domain_mapping_type;
    /// Conceptual is a map a set of elements of type \c element_type
    typedef domain_mapping_type element_type;
    /// The interval type of the map
    typedef Interval<DomainT,Compare> interval_type;
    /// Auxiliary type for overload resolution
    typedef std::pair<interval_type,CodomainT> interval_mapping_type;
    /// Type of an interval containers segment, that is spanned by an interval
    typedef std::pair<interval_type,CodomainT> segment_type;

    //--------------------------------------------------------------------------
    //- Associated types: Size
    //--------------------------------------------------------------------------
    /// The difference type of an interval which is sometimes different form the domain_type
    typedef typename interval_type::difference_type difference_type;
    /// The size type of an interval which is mostly std::size_t
    typedef typename interval_type::size_type size_type;

    //--------------------------------------------------------------------------
    //- Associated types: Functors
    //--------------------------------------------------------------------------
    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Combine functor for codomain value aggregation
    typedef ITL_COMBINE_CODOMAIN(Combine,CodomainT) codomain_combine;
    /// Inverse Combine functor for codomain value aggregation
    typedef typename inverse<codomain_combine>::type inverse_codomain_combine;
    /// Intersection functor for codomain values
    typedef ITL_SECTION_CODOMAIN(Section,CodomainT) codomain_intersect;
    /// Inverse Combine functor for codomain value intersection
    typedef typename inverse<codomain_intersect>::type inverse_codomain_intersect;

    /// Comparison functor for intervals which are keys as well
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

    //--------------------------------------------------------------------------
    //- Associated types: Implementation and stl related
    //--------------------------------------------------------------------------
    /// The allocator type of the set
    typedef Alloc<std::pair<const interval_type, codomain_type> > 
        allocator_type;

    /// Container type for the implementation 
    typedef itl::map<interval_type,codomain_type,Traits,
                     ITL_EXCLUSIVE_LESS(interval_type),Combine,Section,Alloc> ImplMapT;

    /// key type of the implementing container
    typedef typename ImplMapT::key_type   key_type;
    /// value type of the implementing container
    typedef typename ImplMapT::value_type value_type;
    /// data type of the implementing container
    typedef typename ImplMapT::value_type::second_type data_type;

    /// iterator for iteration over intervals
    typedef typename ImplMapT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplMapT::const_iterator const_iterator;
    /// iterator for reverse iteration over intervals
    typedef typename ImplMapT::reverse_iterator reverse_iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplMapT::const_reverse_iterator const_reverse_iterator;
    
public:
    inline static bool has_symmetric_difference() 
    { return is_set<codomain_type>::value || (!is_set<codomain_type>::value && !traits::is_total); }

    enum{ is_itl_container = true };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor for the empty object */
    interval_base_map(){}

    /** Copy constructor */
    interval_base_map(const interval_base_map& src): _map(src._map) {}

    /** Assignment operator */
    interval_base_map& operator = (const interval_base_map& src) 
    { 
        if(this!=&src) that()->assign(src);
        return *this; 
    }

    /** swap the content of containers */
    void swap(interval_base_map& object) { _map.swap(object._map); }

    /** Copy all elements if predicate <tt>pred</tt> holds */
    template<class Predicate>
    interval_base_map& assign_if(const interval_base_map& src, const Predicate& pred)
    { _map.assign_if(src._map, pred); return *this; }

    //==========================================================================
    //= Emptieness, containment
    //==========================================================================

    /** clear the map */
    void clear() { _map.clear(); }

    /** is the map empty? */
    bool empty()const { return _map.empty(); }

    //--- contains: set view ---------------------------------------------------
    /** Does the map contain the domain element \c key? */
    bool contains(const domain_type& key)const
    { 
        typename ImplMapT::const_iterator it_ = _map.find(interval_type(key)); 
        return it_ != _map.end(); 
    }

    /** Does the map contain the <tt>key_value_pair = (key,value)</tt>? */
    bool contains(const element_type& key_value_pair)const
    { 
        return that()->contains_(value_type(interval_type(key_value_pair.key), 
                                                          key_value_pair.data));    
    }

    /** Does the map contain all element value pairs represented by the 
        \c interval_value_pair ? */
    bool contains(const segment_type& interval_value_pair)const
    { return that()->contains_(interval_value_pair); }

    /** Does <tt>*this</tt> container contain <tt>sub</tt>? */
    bool contains(const interval_base_map& sub)const 
    { return sub.contained_in(*this); }

    /** <tt>*this</tt> is subset of <tt>super</tt>? */
    bool contained_in(const interval_base_map& super)const
    { 
        return Interval_Set::is_contained_in(*this, super); 
    }

    //==========================================================================
    //= Size
    //==========================================================================

    /** Number of elements in the map (cardinality). */
    size_type cardinality()const;

    /** An interval map's size is it's cardinality */
    size_type size()const { return cardinality(); }

    /** The length of the interval container which is the sum of 
        interval lenghts */
    difference_type length()const;

    /** Number of intervals which is also the size of the 
        iteration over the object */
    size_t interval_count()const { return _map.size(); }

    /** Size of the iteration over this container */
    size_t iterative_size()const { return _map.size(); }

    //==========================================================================
    //= Range
    //==========================================================================

    /** Lower bound of the first interval */
    DomainT lower()const 
    { return empty()? interval_type().lower() : (*(_map.begin())).KEY_VALUE.lower(); }

    /** Upper bound of the last interval */
    DomainT upper()const 
    { return empty()? interval_type().upper() : (*(_map.rbegin())).KEY_VALUE.upper(); }

    /** Smallest element of the map (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval 
        bounds. */
    DomainT first()const { return (*(_map.begin())).KEY_VALUE.first(); }

    /** Largest element of the map (wrt. the partial ordering on DomainT).
        last() does not exist for continuous datatypes and open interval
        bounds. */
    DomainT last()const { return (*(_map.rbegin())).KEY_VALUE.last(); }


    //==========================================================================
    //= Selection
    //==========================================================================

    /** Find the interval value pair, that contains \c key */
    const_iterator find(const domain_type& key)const
    { 
        const_iterator it_ = _map.find(interval_type(key)); 
        return it_; 
    }

    /** Total select function. */
    codomain_type operator()(const domain_type& key)const
    {
        const_iterator it_ = _map.find(interval_type(key)); 
        return it_==end() ? neutron<codomain_type>::value()
                         : it_->CONT_VALUE;
    }


    //==========================================================================
    //= Addition
    //==========================================================================
private:
    /** Addition of an interval value pair to the map.
        On overlap an aggregation is performed using functor \c Combiner.
        This function is not public, because the `codomain_combine` shall be
        an invariant for all itl maps.*/
    template<class Combiner>
    SubType& add(const segment_type& interval_value_pair) 
    { that()->template add_<Combiner>(interval_value_pair); return *that(); }

public:
    /** Addition of a key value pair to the map */
    SubType& add(const element_type& key_value_pair) 
    { return add( value_type(interval_type(key_value_pair.key), key_value_pair.data) ); }

    /** Addition of an interval value pair to the map. */
    SubType& add(const segment_type& interval_value_pair) 
    { that()->template add_<codomain_combine>(interval_value_pair); return *that(); }

	/** Addition of an interval value pair \c interval_value_pair to the map. 
	    Iterator \c prior_ is a hint to the position \c interval_value_pair can be 
		inserted after. */
    iterator add(iterator prior_, const segment_type& interval_value_pair) 
    { return that()->template add_<codomain_combine>(prior_, interval_value_pair); }

    //==========================================================================
    //= Subtraction
    //==========================================================================
private:
    /** Subtraction of an interval value pair from the map.
        On overlap an aggregation is performed using functor Combiner.
        This function is not public, because the `codomain_combine` shall be
        an invariant for all itl maps.*/
    template<class Combiner>
    void subtract(const segment_type& interval_value_pair)
    { that()->template subtract_<Combiner>(interval_value_pair); }

public:
    /** Subtraction of a key value pair from the map */
    SubType& subtract(const element_type& key_value_pair)
    { 
        return subtract( value_type(interval_type(key_value_pair.key), 
                                                  key_value_pair.data) ); 
    }

    /** Subtraction of an interval value pair from the map. */
    SubType& subtract(const segment_type& interval_value_pair)
    {
        if(Traits::is_total && has_inverse<codomain_type>::value)
            that()->template add_<inverse_codomain_combine>(interval_value_pair); 
        else 
            that()->template subtract_<inverse_codomain_combine>(interval_value_pair); 
    
        return *that();
    }

    //==========================================================================
    //= Insertion
    //==========================================================================

    /** Insertion of a \c key_value_pair into the map. */
    SubType& insert(const element_type& key_value_pair) 
    { 
        that()->insert(value_type(interval_type(key_value_pair.key), 
                                                key_value_pair.data)); 
        return *that();
    }

    /** Insertion of an \c interval_value_pair into the map. */
    SubType& insert(const segment_type& interval_value_pair)
    { that()->insert_(interval_value_pair); return *that(); }


    /** With <tt>key_value_pair = (k,v)</tt> set value \c v for key \c k */
    SubType& set(const element_type& key_value_pair) 
    { 
        that()->set(value_type(interval_type(key_value_pair.key), 
                                             key_value_pair.data)); 
        return *that();
    }

    /** With <tt>interval_value_pair = (I,v)</tt> set value \c v 
        for all keys in interval \c I in the map. */
    SubType& set(const segment_type& interval_value_pair)
    { 
        erase(interval_value_pair.KEY_VALUE); 
        that()->insert_(interval_value_pair); 
        return *that(); 
    }


    //==========================================================================
    //= Erasure
    //==========================================================================

    /** Erase a \c key_value_pair from the map. */
    SubType& erase(const element_type& key_value_pair) 
    { 
        that()->erase_(value_type(interval_type(key_value_pair.key), key_value_pair.data));
        return *that();
    }


    /** Erase an \c interval_value_pair from the map. */
    SubType& erase(const segment_type& interval_value_pair)
    { that()->erase_(interval_value_pair); return *that(); }


    /** Erase a key value pair for \c key. */
    SubType& erase(const domain_type& key) 
    { erase(interval_type(key)); return *that(); }


    /** Erase all value pairs within the range of the 
        interval <tt>inter_val</tt> from the map.   */
    SubType& erase(const interval_type& inter_val);

    /** Erase all value pairs for a set of intervals. */
    template<class SetSubType>
    SubType& erase(const interval_base_set<SetSubType,DomainT,Compare,Interval,Alloc>& eraser)
    {
        typedef interval_base_set<SetSubType,DomainT,Compare,Interval,Alloc> interval_base_set_type;
        for(typename interval_base_set_type::const_iterator interval_ = eraser.begin(); 
            interval_ != eraser.end(); ++interval_)
            erase(*interval_);
    
        return *that();
    }


    /** Erase all value pairs from \c *this map that are elements of map \eraser */
    SubType& erase(const interval_base_map& eraser);


    /** Remove all elements where property <tt>p</tt> holds, keep all others */
    template<class Predicate>
    interval_base_map& erase_if(const Predicate& pred)
    { _map.erase_if(pred); return *this; }


    //==========================================================================
    //= Intersection
    //==========================================================================

    /** The intersection of \c key in \c *this map is added to \c section.
        This can also be used to find \c key in \c *this map */
    void add_intersection(interval_base_map& section, const domain_type& key)const
    { add_intersection(section, interval_type(key)); }

    /** The intersection of \c key_value_pair and \c *this map is added to \c section. */
    void add_intersection(interval_base_map& section, const element_type& key_value_pair)const
    { add_intersection(section, value_type(interval_type(key_value_pair.key), key_value_pair.data)); }

    /** The intersection of \c interval_value_pair and \c *this map is added to \c section. */
    void add_intersection(interval_base_map& section, const segment_type& interval_value_pair)const;

    /** The intersection of \c inter_val with \c *this map is added to \c section. */
    void add_intersection(interval_base_map& section, const interval_type& inter_val)const;

    /** The intersection of set \c sectant with \c *this map is added to \c section. */
    template
    <
        template
        <class DomT, ITL_COMPARE Comp, template<class DomT2, ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void add_intersection
    (
              interval_base_map& section, 
        const IntervalSet<DomainT,Compare,Interval,Alloc>& sectant
    )const
    {
        typedef IntervalSet<DomainT,Compare,Interval,Alloc> sectant_type;
        if(sectant.empty()) return;

        typename sectant_type::const_iterator common_lwb;
        typename sectant_type::const_iterator common_upb;
        if(!Set::common_range(common_lwb, common_upb, sectant, *this))
            return;

        typename sectant_type::const_iterator it_ = common_lwb;
        while(it_ != common_upb)
            add_intersection(section, *it_++);
    }


    /** The intersection of map \c sectant with \c *this map is added to \c section. */
    template
    <
        template
        <    
            class DomT, class CodomT, class Trts, 
            ITL_COMPARE Comp, ITL_COMBINE Combi, ITL_SECTION Sect,
            template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc
        >
        class IntervalMap
    >
    void add_intersection
    (
        interval_base_map& intersection,
        const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& sectant
    )const;


    //==========================================================================
    //= Symmetric difference
    //==========================================================================

    /** If \c *this map contains \c key_value_pair it is erased, otherwise it is added. */
    SubType& flip(const element_type& key_value_pair)
    { return flip(value_type(interval_type(key_value_pair.key), key_value_pair.data)); }

    /** If \c *this map contains \c interval_value_pair it is erased, otherwise it is added. */
    SubType& flip(const segment_type& interval_value_pair);

    /** The intersection of \c *this and \c operand is erased from \c *this. 
        The complemenary value pairs are added to \c *this. */
    template<class SubType2>
    SubType& flip(const interval_base_map<SubType2,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand);


    //==========================================================================
    //= Iterator related
    //==========================================================================

    iterator lower_bound(const key_type& interval)
    { return _map.lower_bound(interval); }

    iterator upper_bound(const key_type& interval)
    { return _map.upper_bound(interval); }

    const_iterator lower_bound(const key_type& interval)const
    { return _map.lower_bound(interval); }

    const_iterator upper_bound(const key_type& interval)const
    { return _map.upper_bound(interval); }

    iterator begin() { return _map.begin(); }
    iterator end()   { return _map.end(); }
    const_iterator begin()const { return _map.begin(); }
    const_iterator end()const   { return _map.end(); }
    reverse_iterator rbegin() { return _map.rbegin(); }
    reverse_iterator rend()   { return _map.rend(); }
    const_reverse_iterator rbegin()const { return _map.rbegin(); }
    const_reverse_iterator rend()const   { return _map.rend(); }

    //==========================================================================
    //= Representation
    //==========================================================================
    
    /** Object as string */
    std::string as_string()const;


    //==========================================================================
    //= Morphisms
    //==========================================================================

    /** Join bounding intervals */
    interval_base_map& join();
            
    /** All value pairs \c (I,y) that have neutral elements \c y==codomain_type()
        as associated values are removed form the map.    */
    void absorb_neutrons()
    {
        //content_is_neutron<key_type, data_type> neutron_dropper;
        if(!Traits::absorbs_neutrons)
            erase_if(content_is_neutron<value_type>());
    }

    /** Set all intervals in the map to be of type <tt>bounded</tt>. 
        Requires Integral<domain_type>.

        Interval bounds of different types are created by opeations on
        interval maps. This function allows to reset them uniformly without,
        of course, changing their value. This is only possible for discrete
        domain datatypes.
    */
    void uniform_bounds(itl::bound_type bounded);


    //==========================================================================
    //= Domain, sum
    //==========================================================================

    /** Gives the domain of the map as interval set */
    template 
    <
        template
        <class DomT, ITL_COMPARE Comp, template<class DomT2, ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void domain(IntervalSet<DomainT,Compare,Interval,Alloc>& dom)const 
    { 
        dom.clear(); 
        const_FOR_IMPLMAP(it_) 
            dom += it_->KEY_VALUE; 
    } 

    /* Sum of associated elements of the map */
    void sum(codomain_type& total)const;

    /* Sum of associated elements of the map */
    codomain_type sum()const
    { codomain_type total; sum(total); return total; }


    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    template<typename IteratorT>
    static const key_type& key_value(IteratorT& value_){ return (*value_).first; }

    template<typename IteratorT>
    static const data_type& data_value(IteratorT& value_){ return (*value_).second; }

    template<typename IteratorT>
    static codomain_type codomain_value(IteratorT& value_){ return (*value_).second; }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) { return key_compare()((*lhs_).first, (*rhs_).first); }

    static value_type make_domain_element(const domain_type& dom_val, const codomain_type& codom_val)
    { return value_type(interval_type(dom_val), codom_val); }

protected:
    sub_type* that() { return static_cast<sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

public:
    sub_type& self() { return *that(); }

protected:

    iterator prior(iterator it_)
	{ return it_ == this->_map.begin() ? this->_map.end() : --it_; }

    template <class Combiner>
    bool combine(iterator& it_, const codomain_type& co_val)
    { 
        Combiner()(it_->CONT_VALUE, co_val);
        if(Traits::absorbs_neutrons && it_->CONT_VALUE == Combiner::neutron())
        { this->_map.erase(it_); it_ = _map.end(); return false; }
        return true;
    }

    template <class Combiner>
    std::pair<iterator,bool> map_insert(const interval_type& inter_val, const codomain_type& co_val)
    {
        if(Traits::is_total)
        {
            CodomainT added_val = Combiner::neutron();
            Combiner()(added_val, co_val);
            return this->_map.insert(value_type(inter_val, added_val));
        }
        else
            return this->_map.insert(value_type(inter_val, co_val));
    }

	// Insertion with hint, that does report insertion failure
    template <class Combiner>
	std::pair<iterator, bool> 
		map_insert(iterator prior_, const interval_type& inter_val, const codomain_type& co_val)
    {
		iterator inserted_ 
			= this->_map.insert(prior_, value_type(inter_val, Combiner::neutron()));

		if(inserted_->KEY_VALUE == inter_val && inserted_->CONT_VALUE == Combiner::neutron())
		{
			Combiner()(inserted_->CONT_VALUE, co_val);
			return std::pair<iterator,bool>(inserted_, true);
		}
		else
			return std::pair<iterator,bool>(inserted_, false);
    }

    template <class Combiner>
    iterator gap_insert(iterator prior_, const interval_type& inter_val, const codomain_type& co_val)
    {
		// inter_val is not conained in this map. Insertion will be successful
		BOOST_ASSERT(this->_map.find(inter_val) == this->_map.end());
		BOOST_ASSERT(!(Traits::absorbs_neutrons && co_val==Combiner::neutron()));

        if(Traits::is_total)
        {
            iterator inserted_ = this->_map.insert(prior_, value_type(inter_val, Combiner::neutron()));
            Combiner()(inserted_->CONT_VALUE, co_val);
			return inserted_;
        }
        else
            return this->_map.insert(prior_, value_type(inter_val, co_val));
    }

protected:
    ImplMapT _map;
} ;



template 
<
    class SubType, class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_map<SubType,DomainT,CodomainT,Traits,
                           Compare,Combine,Section,Interval,Alloc>::size_type 
interval_base_map<SubType,DomainT,CodomainT,Traits,
                  Compare,Combine,Section,Interval,Alloc>::cardinality()const
{
    using namespace boost::mpl;
    return if_<
                bool_<is_continuous<DomainT>::value>,
                continuous_interval_container,
                discrete_interval_container
              >
              ::type::cardinality(*this);
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_map<SubType,DomainT,CodomainT,Traits,
                           Compare,Combine,Section,Interval,Alloc>::difference_type 
interval_base_map<SubType,DomainT,CodomainT,Traits,
                  Compare,Combine,Section,Interval,Alloc>::length()const
{
    difference_type length = neutron<difference_type>::value();
    const_FOR_IMPLMAP(it_)
        length += it_->KEY_VALUE.length();
    return length;
}

//==============================================================================
//= Intersection
//==============================================================================

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
    template
    <
        template
        <    
            class DomT, class CodomT, class Trts, 
            ITL_COMPARE Comp, ITL_COMBINE Combi, ITL_SECTION Sect,
            template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc
        >
        class IntervalMap
    >
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_intersection
(
    interval_base_map& intersection,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& sectant
)const
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> sectant_type;

    if(Traits::is_total)
    {
        intersection = *this;
        intersection += sectant;
    }
    else
    {
        if(sectant.empty()) 
            return;
        typename sectant_type::const_iterator common_lwb;
        typename sectant_type::const_iterator common_upb;
        if(!Set::common_range(common_lwb, common_upb, sectant, *this))
            return;
        typename sectant_type::const_iterator it_ = common_lwb;
        while(it_ != common_upb)
            add_intersection(intersection, *it_++);
    }
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_intersection(interval_base_map& section, 
                    const typename interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
                    ::segment_type& sectant)const
{
    if(Traits::is_total)
    {
        section = *this;
        section.add(sectant);
    }
    else
    {
        interval_type sectant_interval = sectant.KEY_VALUE;
        if(sectant_interval.empty()) 
			return;

        const_iterator first_ = _map.lower_bound(sectant_interval);
        const_iterator end_   = _map.upper_bound(sectant_interval);

		const_iterator it_;
		iterator prior_ = section.end();

		if(is_set<CodomainT>::value)
			for(it_=first_; it_ != end_; it_++) 
			{
				interval_type common_interval = it_->KEY_VALUE & sectant_interval; 
				if(!common_interval.empty())
				{
					section.that()->add(value_type(common_interval, it_->CONT_VALUE));
					section.that()->template add<codomain_intersect>(value_type(common_interval, sectant.CONT_VALUE)); 
				}
			}
		else
			for(it_=first_; it_ != end_; it_++) 
			{
				interval_type common_interval = it_->KEY_VALUE & sectant_interval; 
				if(!common_interval.empty())
				{
					section.that()->add(value_type(common_interval, it_->CONT_VALUE) );
					section.that()->template add<codomain_combine>(value_type(common_interval, sectant.CONT_VALUE));
				}
			}

    }
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_intersection(interval_base_map& section, 
                    const typename interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
                    ::interval_type& sectant_interval)const
{
    if(sectant_interval.empty()) return;

    typename ImplMapT::const_iterator first_ = _map.lower_bound(sectant_interval);
    typename ImplMapT::const_iterator end_   = _map.upper_bound(sectant_interval);

	iterator prior_ = section.end();
    for(typename ImplMapT::const_iterator it_=first_; it_ != end_; it_++) 
    {
        interval_type common_interval = it_->KEY_VALUE & sectant_interval; 
        if(!common_interval.empty())
            prior_ = section.that()->add(prior_, value_type(common_interval, it_->CONT_VALUE) );
    }
}

//==============================================================================
//= Symmetric difference
//==============================================================================

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
SubType& interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::flip(const segment_type& interval_value_pair)
{
    // That which is common shall be subtracted
    // That which is not shall be added
    // So x has to be 'complementary added' or flipped

    if(Traits::is_total && Traits::absorbs_neutrons)
    {
        clear();
        return *that();
    }
    if(Traits::is_total && !Traits::absorbs_neutrons)
    {
        (*that()) += interval_value_pair;
        FORALL(typename ImplMapT, it_, _map)
            it_->CONT_VALUE = neutron<codomain_type>::value();

        if(!is_interval_splitter<SubType>::value)
            join();

        return *that();
    }

    interval_type span = interval_value_pair.KEY_VALUE;

    typename ImplMapT::const_iterator first_ = _map.lower_bound(span);
    typename ImplMapT::const_iterator end_   = _map.upper_bound(span);

    interval_type covered, left_over, common_interval;
    const codomain_type& x_value = interval_value_pair.CONT_VALUE;
    typename ImplMapT::const_iterator it_ = first_;

    interval_set<DomainT,Compare,Interval,Alloc> eraser;
    interval_base_map intersection;

    while(it_ != end_  ) 
    {
        const codomain_type& co_value = it_->CONT_VALUE;
        covered = (*it_++).KEY_VALUE; 
        //[a      ...  : span
        //     [b ...  : covered
        //[a  b)       : left_over
        left_over = right_subtract(span, covered);

        //That which is common ...
        common_interval = span & covered;
        if(!common_interval.empty())
        {
            // ... shall be subtracted
            eraser.add(common_interval);

            if(is_set<codomain_type>::value)
            {
                codomain_type common_value = x_value;
                inverse_codomain_intersect()(common_value, co_value);
                intersection.add(value_type(common_interval, common_value));
            }
            else
                intersection.add(value_type(common_interval, neutron<codomain_type>::value()));
        }

        add(value_type(left_over, x_value)); //That which is not shall be added
        // Because this is a collision free addition I don't have to distinguish codomain_types.

        //...      d) : span
        //... c)      : (*it_); span.left_subtract(*it_);
        //     [c  d) : span'
        span.left_subtract(covered);
    }

    //If span is not empty here, it is not in the set so it shall be added
    add(value_type(span, x_value));

    //finally rewrite the common segments
    erase(eraser);
    (*this) += intersection;

    return *that();
}



template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
    template<class SubType2>
SubType& interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::flip(const interval_base_map<SubType2,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand)
{
    typedef interval_base_map<SubType2,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> operand_type;

    if(Traits::is_total && Traits::absorbs_neutrons)
    {
        clear();
        return *that();
    }
    if(Traits::is_total && !Traits::absorbs_neutrons)
    {
        (*that()) += operand;
        FORALL(typename ImplMapT, it_, _map)
            it_->CONT_VALUE = neutron<codomain_type>::value();

        if(!is_interval_splitter<SubType>::value)
            join();

        return *that();
    }

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, *this))
        return *that() += operand;

    typename operand_type::const_iterator it_ = operand.begin();

    // All elements of operand left of the common range are added
    while(it_ != common_lwb)
        add(*it_++);
    // All elements of operand in the common range are symmetrically subtracted
    while(it_ != common_upb)
        flip(*it_++);
    // All elements of operand right of the common range are added
    while(it_ != operand.end())
        add(*it_++);

    if(Traits::is_total && !Traits::absorbs_neutrons)
        FORALL(typename ImplMapT, it_, _map)
            it_->CONT_VALUE = neutron<codomain_type>::value();

    return *that();
}



template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::join()
{
    iterator it_ = _map.begin();
    if(it_==_map.end()) 
        return *this;

    iterator next_ = it_; next_++;

    while(next_ != _map.end())
    {
        if(    it_->KEY_VALUE.touches(next_->KEY_VALUE)
            && it_->CONT_VALUE == next_->CONT_VALUE      )
        {
            iterator fst_mem = it_;  // hold the fist member
            
            // Go on while touching members are found
            it_++; next_++;
            while(     next_ != _map.end()
                    && it_->KEY_VALUE.touches(next_->KEY_VALUE)
                    && it_->CONT_VALUE == next_->CONT_VALUE     )
            { it_++; next_++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence            
			const_cast<interval_type&>(it_->KEY_VALUE).extend(fst_mem->KEY_VALUE);
            _map.erase(fst_mem, it_);

            it_++; next_=it_; 
			if(next_!=_map.end())
				next_++;
        }
        else { it_++; next_++; }
    }
    return *this;
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
std::string interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::as_string()const
{
    std::string res(""); 
    const_FOR_IMPLMAP(it_) {
        std::string cur("("); 
        cur += it_->KEY_VALUE.as_string();
        cur += ",";
        cur += itl::to_string<CodomainT>::apply(it_->CONT_VALUE);
        cur += ")";
        res += cur;
    }
    return res; 
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::sum(codomain_type& total)const
{
    total = codomain_combine::neutron();
    const_FOR_IMPLMAP(it_) 
        total += it_->CONT_VALUE;
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::uniform_bounds(itl::bound_type bounded)
{
    // I can do this only, because I am shure that the contents and the
    // ordering < on interval is invariant wrt. this transformation on bounds
    FOR_IMPLMAP(it_) const_cast<interval_type&>(it_->KEY_VALUE).as(bounded);
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline SubType& interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase(const interval_type& minuend)
{
    if(minuend.empty()) 
        return *that();

    iterator first_ = _map.lower_bound(minuend);
    if(first_==_map.end()) 
        return *that();
    iterator end_   = _map.upper_bound(minuend);
    if(first_==end_  )
        return *that();

    iterator last_  = end_; --last_;

    interval_type left_resid  = right_subtract(first_->KEY_VALUE, minuend);
    interval_type right_resid =  left_subtract(last_ ->KEY_VALUE, minuend);

    if(first_ == last_ )
        if(!left_resid.empty())
        {
            const_cast<interval_type&>(first_->KEY_VALUE).right_subtract(minuend);
            if(!right_resid.empty())
                this->_map.insert(first_, value_type(right_resid, first_->CONT_VALUE));
        }
        else if(!right_resid.empty())
            const_cast<interval_type&>(first_->KEY_VALUE).left_subtract(minuend);
        else
            this->_map.erase(first_);
    else
    {   //            [-------- minuend ---------)
        // [left_resid   fst)   . . . .    [lst  right_resid)
        iterator second_= first_; ++second_;

        iterator start_ = left_resid.empty()? first_: second_;
        iterator stop_  = right_resid.empty()? end_  : last_ ;
        this->_map.erase(start_, stop_); //erase [start_, stop_)

        if(!left_resid.empty())
            const_cast<interval_type&>(first_->KEY_VALUE).right_subtract(minuend);

        if(!right_resid.empty())
            const_cast<interval_type&>(last_ ->KEY_VALUE).left_subtract(minuend);
    }
    return *that();
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
SubType& 
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase(const interval_base_map& erasure)
{
    const_FORALL(typename interval_base_map, value_pair_, erasure)
        that()->erase_(*value_pair_);

    return *that();
}

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
//-----------------------------------------------------------------------------
// equality
//-----------------------------------------------------------------------------
template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator == (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                         const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    //MEMO PORT: This implemetation worked with stlport, sgi and gnu 
    // implementations of the stl. But using MSVC-implementation
    // results in runtime error! So I had to provide an independent
    // safe implemetation.
    //return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return Set::lexicographical_equal(lhs, rhs);
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator != (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                         const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return !(lhs == rhs);
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool is_protonic_equal(const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                              const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return Map::lexicographical_protonic_equal(lhs, rhs);
}


//-----------------------------------------------------------------------------
// order
//-----------------------------------------------------------------------------
template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator < (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
        Compare<std::pair<Interval<DomainT,Compare>,CodomainT> >()//NOTE DESIGN TTP: Why template template parameter Compare is needed
        );
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator > (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{ return rhs < lhs; }

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator <= (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{ return !(lhs > rhs); }

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline bool operator >= (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{ return !(lhs < rhs); }

//-----------------------------------------------------------------------------
// min, max
//-----------------------------------------------------------------------------
template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
interval_base_map<SubType,DomainT,CodomainT,
                  Traits,Compare,Combine,Section,Interval,Alloc>& 
min_assign
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> map_type;
    const_FORALL(typename map_type, elem_, operand) 
        object.template add<inplace_min >(*elem_); 

    return object; 
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
interval_base_map<SubType,DomainT,CodomainT,
                  Traits,Compare,Combine,Section,Interval,Alloc>& 
max_assign
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc>    map_type;
    const_FORALL(typename map_type, elem_, operand) 
        object.template add<inplace_max>(*elem_); 

    return object; 
}

//---------------------------------------------------------------------------------

template<class CharType, class CharTraits, 
    class SubType, class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_base_map<SubType,DomainT,CodomainT,Traits,
                           Compare,Combine,Section,Interval,Alloc>& object)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> IntervalMapT;
    stream << "{";
    const_FORALL(typename IntervalMapT, it_, object)
        stream << "(" << it_->KEY_VALUE << "->" << it_->CONT_VALUE << ")";

    return stream << "}";
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
struct is_set<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
struct is_map<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
struct has_inverse<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = has_inverse<CodomainT>::value; 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
struct is_interval_container<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    static const bool value = true; 
};



}} // namespace itl boost

#endif



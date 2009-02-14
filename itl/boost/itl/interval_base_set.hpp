/*-----------------------------------------------------------------------------+
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __interval_base_set_h_JOFA_990223__
#define __interval_base_set_h_JOFA_990223__

#include <limits>
#include <boost/itl/interval_set_algo.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/detail/notate.hpp>

#define const_FOR_IMPL(iter) for(typename ImplSetT::const_iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)
#define FOR_IMPL(iter) for(typename ImplSetT::iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)


namespace boost{namespace itl
{

/** Implements a set as a set of intervals (base class) */
template 
<
    typename             SubType,
    typename             DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class, ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
#ifdef USE_CONCEPTS
    requires {std::LessThanComparable<DomainT>}
#endif
class interval_base_set
{
public:
	//==========================================================================
	//= Associated types
	//==========================================================================
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> type;

    /// The designated \e derived or \e sub_type of this base class
    typedef SubType sub_type;

	/// Auxilliary type for overloadresolution
	typedef type overloadable_type;

	//--------------------------------------------------------------------------
	//- Associated types: Related types
	//--------------------------------------------------------------------------
    /// The atomized type representing the corresponding container of elements
    typedef typename itl::set<DomainT,Compare,Alloc> atomized_type;

	//--------------------------------------------------------------------------
	//- Associated types: Data
	//--------------------------------------------------------------------------
    /// The domain type of the set
    typedef DomainT   domain_type;
    /// The codomaintype is the same as domain_type
    typedef DomainT   codomain_type;

    /// The element type of the set
    typedef DomainT   element_type;

    /// The interval type of the set
    typedef Interval<DomainT,Compare> interval_type;
    /// The segment type of the set
    typedef interval_type   segment_type;

	//--------------------------------------------------------------------------
	//- Associated types: Size
	//--------------------------------------------------------------------------
    /// The difference type of an interval which is sometimes different form the data_type
    typedef typename interval_type::difference_type difference_type;

    /// The size type of an interval which is mostly std::size_t
    typedef typename interval_type::size_type size_type;


	//--------------------------------------------------------------------------
	//- Associated types: Order
	//--------------------------------------------------------------------------
    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

	//--------------------------------------------------------------------------
	//- Associated types: Implementation and stl related
	//--------------------------------------------------------------------------
    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// Container type for the implementation 
    typedef typename itl::set<interval_type,ITL_EXCLUSIVE_LESS(interval_type),Alloc> ImplSetT;

    /// key type of the implementing container
    typedef typename ImplSetT::key_type   key_type;
    /// data type of the implementing container
    typedef typename ImplSetT::data_type  data_type;
    /// value type of the implementing container
    typedef typename ImplSetT::value_type value_type;

    /// iterator for iteration over intervals
    typedef typename ImplSetT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_iterator const_iterator;
    /// iterator for reverse iteration over intervals
    typedef typename ImplSetT::reverse_iterator reverse_iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_reverse_iterator const_reverse_iterator;


public:
	//==========================================================================
	//= Construct, copy, destruct
	//==========================================================================
    /** Default constructor for the empty object */
    interval_base_set(){}

    /** Copy constructor */
    interval_base_set(const interval_base_set& src): _set() 
    { that()->assign(src); }

    /** Assignment operator */
    interval_base_set& operator = (const interval_base_set& src) 
    { 
        if(this!=&src) that()->assign(src);
        return *this; 
    }

    /** swap the content of containers */
    void swap(interval_base_set& x) { _set.swap(x._set); }

	//==========================================================================
	//= Emptieness, containment
	//==========================================================================

    /** sets the container empty */
    void clear() { _set.clear(); }
    /** is the container empty? */
    bool empty()const { return _set.empty(); }

    /** Does the container contain the element \c key ? */
    bool contains(const element_type& key)const
	{ return that()->contains_(interval_type(key)); }

    /** Does the container contain the interval \c inter_val ? */
    bool contains(const segment_type& inter_val)const
    { return that()->contains_(inter_val); }

    /** Does the container contain the subcontainer \c sub ? */
    bool contains(const interval_base_set& sub)const 
    { return sub.contained_in(*this); }

    /** Does <tt>*this</tt> container contain <tt>sub</tt>? */
	template
    <
        template<class DomT, ITL_COMPARE Comp, 
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    bool contains(const IntervalSet<DomainT,Compare,Interval,Alloc>& sub)const 
    { return sub.contained_in(*that()); }

    /** Is <tt>*this</tt> container contained in <tt>super</tt>? */
	template
    <
        template<class DomT, ITL_COMPARE Comp, 
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    bool contained_in(const IntervalSet<DomainT,Compare,Interval,Alloc>& super)const
	{
		return Interval_Set::is_contained_in(*this, super);
	}

	//==========================================================================
	//= Size
	//==========================================================================

    /** Number of elements in the set (cardinality). 
        Infinite for continuous domain datatyps    */
    size_type cardinality()const;

    /** An interval set's size is it's cardinality */
    size_type size()const { return cardinality(); }

    /** The length of the interval container which is the sum of interval lenghts */
    difference_type length()const;

    /** Number of intervals which is also the size of the iteration over the object */
    std::size_t interval_count()const { return _set.size(); }

    /** Size of the iteration over this container */
    std::size_t iterative_size()const { return _set.size(); }

	//==========================================================================
	//= Range
	//==========================================================================

    /** lower bound of all intervals in the object */
    DomainT lower()const 
    { return empty()? interval_type().lower() : (*(_set.begin())).lower(); }
    /** upper bound of all intervals in the object */
    DomainT upper()const 
    { return empty()? interval_type().upper() : (*(_set.rbegin())).upper(); }

    /** Smallest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval 
		bounds. */
    DomainT first()const { return (*(_set.begin())).first(); }

    /** Largest element of the set (wrt. the partial ordering on DomainT).
        last() does not exist for continuous datatypes and open interval 
		bounds. */
    DomainT last()const { return (*(_set.rbegin())).last(); }

	//==========================================================================
	//= Selection
	//==========================================================================

	/** Find the interval value pair, that contains element \c key */
    const_iterator find(const element_type& key)const
    { 
        typename ImplSetT::const_iterator it = this->_set.find(interval_type(key)); 
        return it; 
    }

	//==========================================================================
	//= Addition
	//==========================================================================

    /** Add a single element \c key to the set */
    SubType& add(const element_type& key) 
	{ that()->add_(interval_type(key)); return *that(); }

    /** Add an interval of elements \c inter_val to the set */
    SubType& add(const segment_type& inter_val) 
    { that()->add_(inter_val); return *that(); }

	//==========================================================================
	//= Subtraction
	//==========================================================================

    /** Subtract a single element \c key from the set */
    SubType& subtract(const element_type& key) 
	{ that()->subtract_(interval_type(key)); return *that(); }

    /** Subtract an interval of elements \c inter_val from the set */
    SubType& subtract(const segment_type& inter_val) 
    { that()->subtract_(inter_val); return *that(); }

	//==========================================================================
	//= Insertion, erasure
	//==========================================================================

    /** Insert an element \c key into the set */
    SubType& insert(const element_type& key) 
    { return add(interval_type(key)); }

    /** Insert an interval of elements \c inter_val to the set */
    SubType& insert(const segment_type& inter_val) 
    { return add(inter_val); }

    /** Erase an element \c key from the set */
    SubType& erase(const element_type& key) 
    { return subtract(interval_type(key)); }

    /** Erase an interval of elements \c inter_val from the set */
    SubType& erase(const segment_type& inter_val) 
    { return subtract(inter_val); }

	//==========================================================================
	//= Intersection
	//==========================================================================

	/** The intersection of \c key in \c *this set is added to \c section. 
	    The function can be used as a find function. */
    void add_intersection(interval_base_set& section, const element_type& key)const
	{ add_intersection(section, interval_type(key)); }

	/** The intersection of \c inter_val in \c *this set is added to \c section. */
    void add_intersection(interval_base_set& section, const segment_type& inter_val)const;


	/** The intersection of set \c sectant with \c *this set is added to \c section. */
	template
    <
        template<class DomT, ITL_COMPARE Comp, 
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void add_intersection
    (
        interval_base_set& intersection,
        const IntervalSet<DomainT,Compare,Interval,Alloc>& sectant
    )const;


	//==========================================================================
	//= Symmetric difference
	//==========================================================================

	/** If \c *this set contains \c key it is erased, otherwise it is added. */
    SubType& flip(const element_type& key)
	{ return flip(interval_type(key)); }

	/** If \c *this set contains \c inter_val it is erased, otherwise it is added. */
    SubType& flip(const segment_type& inter_val);

	/** The intersection of \c *this and \c operand is erased from \c *this. 
	    The complemenary elements are added to \c *this. */
	template
    <
        template<class DomT, ITL_COMPARE Comp, 
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    SubType& flip(const IntervalSet<DomainT,Compare,Interval,Alloc>& operand);

	//==========================================================================
	//= Iterator related
	//==========================================================================

    iterator begin() { return _set.begin(); }
    iterator end()   { return _set.end(); }
    const_iterator begin()const { return _set.begin(); }
    const_iterator end()const   { return _set.end(); }
    reverse_iterator rbegin() { return _set.rbegin(); }
    reverse_iterator rend()   { return _set.rend(); }
    const_reverse_iterator rbegin()const { return _set.rbegin(); }
    const_reverse_iterator rend()const   { return _set.rend(); }

    iterator lower_bound(const value_type& interval)
    { return _set.lower_bound(interval); }

    iterator upper_bound(const value_type& interval)
    { return _set.upper_bound(interval); }

    const_iterator lower_bound(const value_type& interval)const
    { return _set.lower_bound(interval); }

    const_iterator upper_bound(const value_type& interval)const
    { return _set.upper_bound(interval); }


	//==========================================================================
	//= Morphisms
	//==========================================================================
	
    /** Join bordering intervals */
    interval_base_set& join();

	/** Set interval bounds to the type <tt>bt</tt> for intervals in the set.
        Interval bounds of different types are created by opeations on
        interval sets. This function allows to reset them uniformly without,
        of course, changing their value. This is only possible for discrete
        domain datatypes.
    */
	void uniform_bounds(itl::bound_type bounded);


	//==========================================================================
	//= Representation
	//==========================================================================
	
    /** Interval container's string representation */
    const std::string as_string()const
    { std::string res(""); const_FOR_IMPL(it) res += (*it).as_string(); return res; }

    
	//==========================================================================
	//= Algorithm unifiers
	//==========================================================================

    template<typename IteratorT>
    static const key_type& key_value(IteratorT& value_){ return (*value_); }

    template<typename IteratorT>
    static codomain_type codomain_value(IteratorT& value_)
    { return (*value_).empty()? codomain_type() : (*value_).lower(); }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) 
    { return key_compare()(*lhs_, *rhs_); }

    static value_type make_domain_element(const domain_type& dom_val, const codomain_type& codom_val)
    { return value_type(interval_type(dom_val)); }

protected:
    sub_type* that() { return static_cast<sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

public:
	sub_type& self() { return *that(); }

protected:
    ImplSetT _set;
} ;



template
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::size_type 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::cardinality()const
{
    using namespace boost::mpl;
    return if_<
                bool_<is_continuous<DomainT>::value>,
                continuous_interval_container,
                discrete_interval_container
              >
              ::type::cardinality(*this);

    /*JODO BOOST: This more simple implementention fails because ptime::duration has no infinity
    size_type size = neutron<size_type>::value();
    size_type interval_size;
    const_FOR_IMPL(it)
    {
        interval_size = (*it).cardinality();
        if(interval_size == std::numeric_limits<size_type>::infinity())
            return interval_size;
        else
            size += interval_size;
    }
    return size;
    */
}

template
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename 
    interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::difference_type 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::length()const
{
    difference_type length = neutron<difference_type>::value();
    const_FOR_IMPL(it)
        length += (*it).length();
    return length;
}


/*CL
template<class SubType, 
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template
    <
        template<class DomT, ITL_COMPARE Comp,
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
bool interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::contained_in(const IntervalSet<DomainT,Compare,Interval,Alloc>& x2)const
{
    // The empty set is subset of every set
    if(empty())
        return true;
    else if (x2.empty())
        return false;
    else if(upper() < x2.lower())
        return false;
    else if(x2.upper() < lower())
        return false;
    else
    {
        // x2 should be larger than *this; so every element in this should be in x2
        const_FOR_IMPL(it) 
            if(!x2.contains(*it)) 
                return false;
        return true;
    }
}
*/

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::add_intersection(interval_base_set& section, const segment_type& inter_val)const
{
    // any intersection with the empty intervall is empty
    if(inter_val.empty()) 
        return;

    typename ImplSetT::const_iterator fst_it = _set.lower_bound(inter_val);
    typename ImplSetT::const_iterator end_it = _set.upper_bound(inter_val);

    for(typename ImplSetT::const_iterator it=fst_it; it != end_it; it++) 
        section.add((*it) & inter_val);
}


template<class SubType, 
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
    template
    <
        template<class DomT, ITL_COMPARE Comp,
		         template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::add_intersection
(
    interval_base_set& intersection,
    const IntervalSet<DomainT,Compare,Interval,Alloc>& operand
)const
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
		return;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, *this))
        return;

    typename operand_type::const_iterator it = common_lwb;
    while(it != common_upb)
        add_intersection(intersection, *it++);
}

//==============================================================================
//= Symmetric difference
//==============================================================================

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
SubType& interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
	::flip(const segment_type& inter_val)
{
    // That which is common shall be subtracted
	// That which is not shall be added
	// So x has to be 'complementary added' or flipped
	interval_type span = inter_val;
    typename ImplSetT::const_iterator fst_ = _set.lower_bound(span);
    typename ImplSetT::const_iterator end_ = _set.upper_bound(span);

	interval_type covered, left_over;
	typename ImplSetT::const_iterator it_ = fst_;
    while(it_ != end_) 
    {
		covered = *it_++; 
		//[a      ...  : span
		//     [b ...  : covered
		//[a  b)       : left_over
		span.right_subtract(left_over, covered);
		subtract(span & covered); //That which is common shall be subtracted
		add(left_over);                //That which is not shall be added

		//...      d) : span
		//... c)      : (*it); span.left_subtract(*it);
		//     [c  d) : span'
		span.left_subtract(covered);
    }

	//If span is not empty here, it is not in the set so it shall be added
	add(span);
	return *that();
}


template
<
	class SubType, 
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
    template
    <
		template<class DomT, ITL_COMPARE Comp, template<class DomT2,ITL_COMPARE>class Interv, ITL_ALLOC Allc>
		class IntervalSet
	>
SubType& interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::flip(const IntervalSet<DomainT,Compare,Interval,Alloc>& operand)
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
		return *that();

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, *this))
		return *that() += operand;

    typename operand_type::const_iterator it = operand.begin();

	// All elements of operand left of the common range are added
    while(it != common_lwb)
        add(*it++);
	// All elements of operand in the common range are symmertrically subtracted
    while(it != common_upb)
        flip(*it++);
	// All elements of operand right of the common range are added
    while(it != operand.end())
        add(*it++);

	return *that();
}


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::join()
{
    iterator it=_set.begin();
    if(it==_set.end()) 
        return *this;

    iterator nxt=it; nxt++;
    if(nxt==_set.end()) 
        return *this;

    while(nxt != _set.end())
    {
        if( (*it).touches(*nxt) )
        {
            iterator fst_mem = it;  // hold the fist member
            
            // go noodling on while touchin members found
            it++; nxt++;
            while(     nxt != _set.end()
                    && (*it).touches(*nxt) )
            { it++; nxt++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence
            iterator lst_mem = it, end_mem = nxt;
            interval_type joinedInterval(*fst_mem);
            joinedInterval.extend(*lst_mem);
            
            _set.erase(fst_mem, end_mem);
            it = _set.insert(joinedInterval).ITERATOR;

            it++; // go on for the next after the currently inserted
            nxt=it; if(nxt!=_set.end())nxt++;
        }
        else { it++; nxt++; }
    }
    return *this;
}



template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::uniform_bounds(itl::bound_type bounded)
{
    // I can do this only, because I am shure that the contents and the
    // ordering < on interval is invariant wrt. this transformation on bounds
    FOR_IMPL(it) const_cast<interval_type&>(*it).as(bounded);
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator == (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    //MEMO PORT: This implemetation worked with stlport, sgi and gnu 
    // implementations of the stl. But using MSVC-implementation
    // results in runtime error! So I had to provide an independent
    // safe implemetation.
    //return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return Set::lexicographical_equal(lhs, rhs);
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator != (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs == rhs); }


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator < (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
		Compare<Interval<DomainT,Compare> >()//NOTE DESIGN TTP: Why Compare is needs to be ttp
		);
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator > (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return rhs < lhs; }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator <= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs > rhs); }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator >= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs < rhs); }

template<class CharType, class CharTraits, 
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> IntervalSetT;
    stream << "{";
    const_FORALL(typename IntervalSetT, it, object)
        stream << (*it);

    return stream << "}";
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
	typedef is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
	static const bool value = true; 
};

template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
	typedef is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
	static const bool value = true; 
};



}} // namespace itl boost

#endif



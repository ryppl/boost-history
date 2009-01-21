/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------+
class interval_base_set
+----------------------------------------------------------------------------*/
#ifndef __interval_base_set_h_JOFA_990223__
#define __interval_base_set_h_JOFA_990223__

#include <limits>
#include <boost/itl/interval_set_algo.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/notate.hpp>

#define const_FOR_IMPL(iter) for(typename ImplSetT::const_iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)
#define FOR_IMPL(iter) for(typename ImplSetT::iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)


namespace boost{namespace itl
{

//JODO update documentation (all invterval containers; template parameters have changed)
/// Implements a set as a set of intervals (base class)
/**    
    Abstract template-class <b>interval_base_set</b> 
    implements a set as a set of intervals

    Template parameter <b>DomainT</b>: The set's domain-type. Type of the 
    set's elements.

    Suitable as domain types are all datatypes that posess a partial order.
    In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
    atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.
    
      Template parameter <b>Interval=itl::interval</b>: Type of interval used
    to implement the set. The default <b>itl::interval</b> uses the
    interval class template that comes with this library. Own implementation of interval
    classes are possible (but not trivial).

    <b>interval_base_set</b> implements a set <tt>set<DomainT></tt> as a set of intervals
    <tt>set<interval<DomainT>></tt>.
  
    interval_base_set<DomainT> can thus be used like a set. As it is known from mathematics
    the union over a set of intervls is a set itself.

    <b>Class <tt>interval_base_set</tt> yields the following benefits: </b>
  
    <ul>
        <li> 
            A set of intervals is conceived as a set. The complexity involved with
            operations on intervals and sets of intervals is encapsulated.
            The user of the class who
            wants to perform set operations on sets of intervals is no more concerned
            with questions of overlapping, joining and bordering intervals.
        <li>
            <b>interval_base_set</b> gives an efficient implementation of sets consisting
            of larger contiguous chunks. Very large, even uncountably infinite sets of
            elements can be represented in a compact way and handled efficiently.
    </ul>

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

    //A: Type definitions for the template class 
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> type;

    /// The designated \e derived or \e sub_type of this base class
    typedef SubType sub_type;

	typedef type overloadable_type;

    /// The domain type of the set
    typedef DomainT   domain_type;
    /// The codomaintype is the same as domain_type
    typedef DomainT   codomain_type;

    /// The interval type of the set
    typedef Interval<DomainT,Compare> interval_type;

    /// The difference type of an interval which is sometimes different form the data_type
    typedef typename interval_type::difference_type difference_type;

    /// The size type of an interval which is mostly std::size_t
    typedef typename interval_type::size_type size_type;


    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// The type of the set of elements that is equivalent to the set of intervals
    typedef typename itl::set<DomainT,Compare,Alloc> element_set;

    /// The corresponding atomized type representing this interval container of elements
    typedef typename itl::set<DomainT,Compare,Alloc> atomized_type;

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
    // B: Constructors, destructors, assignment
    /// Default constructor for the empty set 
    interval_base_set(){}
    /// Copy constructor
    interval_base_set(const interval_base_set& src): _set() 
    { that()->assign(src); }

    /// Assignment operator
    interval_base_set& operator = (const interval_base_set& src) 
    { 
        if(this!=&src) that()->assign(src);
        return *this; 
    }

    void swap(interval_base_set& x) { _set.swap(x._set); }

    // ------------------------------------------------------------------------
    // Basic set concept
    // C:

    /// sets the container empty
    void clear() { _set.clear(); }
    /// is the container empty
    bool empty()const { return _set.empty(); }

    /// Find the interval value pair, that contains element \c x
    const_iterator find(const DomainT& x)const
    { 
        typename ImplSetT::const_iterator it = this->_map.find(interval_type(x)); 
        return it; 
    }

    /// Does the container contain the element \c x
    bool contains(const DomainT& x)const
	{ return that()->contains_(interval_type(x)); }

    /// Does the container contain the interval x
    bool contains(const interval_type& x)const
    { return that()->contains_(x); }

    /** Does <tt>*this</tt> container contain <tt>sub</tt>? */
    bool contains(const interval_base_set& sub)const 
    { return sub.contained_in(*this); }

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool contained_in(const interval_base_set& super)const;

/** @name E: Bounds and other selectors
    */
//@{ 
    /// lower bound of all intervals in the set
    DomainT lower()const 
    { return empty()? interval_type().lower() : (*(_set.begin())).lower(); }
    /// upper bound of all intervals in the set
    DomainT upper()const 
    { return empty()? interval_type().upper() : (*(_set.rbegin())).upper(); }

    iterator lower_bound(const value_type& interval)
    { return _set.lower_bound(interval); }

    iterator upper_bound(const value_type& interval)
    { return _set.upper_bound(interval); }

    const_iterator lower_bound(const value_type& interval)const
    { return _set.lower_bound(interval); }

    const_iterator upper_bound(const value_type& interval)const
    { return _set.upper_bound(interval); }

    /// number of intervals
    std::size_t interval_count()const { return _set.size(); }
    std::size_t iterative_size()const { return _set.size(); }
//@}


//-----------------------------------------------------------------------------
/** @name G.add: Addition */
//@{

    /// Add a single element \c x to the set
    SubType& add(const DomainT& x) 
	{ that()->add_(interval_type(x)); return *that(); }

    /// Add an interval of elements \c x to the set
    SubType& add(const value_type& x) 
    { that()->add_(x); return *that(); }

//@}

//-----------------------------------------------------------------------------
/** @name G.sub: Subtraction */
//@{

    /// Subtract a single element \c x from the set
    SubType& subtract(const DomainT& x) 
	{ that()->subtract_(interval_type(x)); return *that(); }

    /// Subtract an interval of elements \c x from the set
    SubType& subtract(const value_type& x) 
    { that()->subtract_(x); return *that(); }

    ///// Subtract a single element \c x from the set
    //interval_base_set& operator -= (const DomainT& x) 
    //{ subtract(x); return *this; }

    ///// Subtract an interval of elements \c x from the set
    //interval_base_set& operator -= (const value_type& x)
    //{ that()->subtract(x); return *this; }

//@}

//-----------------------------------------------------------------------------
/** @name G.ins&ers: Insertion and erasure  */
//@{
    /// Insert an element \c x into the set
    SubType& insert(const DomainT& x) 
    { return add(interval_type(x)); }

    /// Insert an interval of elements \c x to the set
    SubType& insert(const value_type& x) 
    { return add(x); }

    /// Erase an element \c x from the set
    SubType& erase(const DomainT& x) 
    { return subtract(interval_type(x)); }

    /// Erase an interval of element \c x from the set
    SubType& erase(const value_type& x) 
    { return subtract(x); }
//@}

//-----------------------------------------------------------------------------
/** @name G.sect: Intersection */
//@{

    /*JODO DOC Intersection with interval x; The intersection is assigned to <tt>section</tt>. 
    
        Intersection also serves a generalized <tt>find</tt>-function to search
        for intervals in the set:

        <tt>ItvSetT<int> x, sec; interval<int> i; fill x; fill i;</tt>

        <tt>x.intersect(sec,i);</tt> 
        
        If <tt>i</tt> is an interval of <tt>x</tt>, then 
        
        <tt>sec.nOfIntervals()==1</tt> and <tt>*(sec.begin())==x</tt> 
    */
    //CL void intersect(interval_base_set& section, const value_type& x)const;

    //JODO DOC; welche intersect-varianten kann ich ganz los werden.
    void add_intersection(interval_base_set& section, const value_type& x)const;

    //JODO doku
    /** Perform intersection of <tt>*this</tt> and <tt>x</tt>; assign result
        to <tt>section</tt>
    */

    /** Perform intersection of <tt>*this</tt> and <tt>x</tt>; assign result
        to <tt>*this</tt>

        Aufruf <tt>x &= y</tt> bedeutet <tt>x = x geschnitten mit y </tt>
    */
//@}

//-----------------------------------------------------------------------------
/** @name G.jodo: JODO */

    /// Join bordering intervals    
    interval_base_set& join();

//@}



/** @name I: Interval iterators
    */
//@{
    ///
    iterator begin() { return _set.begin(); }
    ///
    iterator end()   { return _set.end(); }
    ///
    const_iterator begin()const { return _set.begin(); }
    ///
    const_iterator end()const   { return _set.end(); }
    ///
    reverse_iterator rbegin() { return _set.rbegin(); }
    ///
    reverse_iterator rend()   { return _set.rend(); }
    ///
    const_reverse_iterator rbegin()const { return _set.rbegin(); }
    ///
    const_reverse_iterator rend()const   { return _set.rend(); }
//@}



/** @name S: String representation
    */
//@{
    /// Interval-set as string
    const std::string as_string()const
    { std::string res(""); const_FOR_IMPL(it) res += (*it).as_string(); return res; }
//@}

    
/** @name T: For discrete domain datatypes only that implement operators <tt>++</tt> 
        and <tt>--</tt>
    */
//@{
    /** Smallest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval bounds.
    */
    DomainT first()const { return (*(_set.begin())).first(); }  // JODO NONCONT

    /** Largest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval bounds.
    */
    DomainT last()const { return (*(_set.rbegin())).last(); } // JODO NONCONT

    /** Number of elements in the set (cardinality). 
        Infinite for continuous domain datatyps    */
    size_type cardinality()const;

    /// An interval set's size is it's cardinality
    size_type size()const { return cardinality(); }

    difference_type length()const;

    /**    Set interval bounds to the type <tt>bt</tt> for intervals in the set.

        Interval bounds of different types are created by opeations on
        interval sets. This function allows to reset them uniformly without,
        of course, changing their value. This is only possible for discrete
        domain datatypes.
    */
	void uniform_bounds(itl::bound_type bounded);

//@}


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


template
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
bool interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
    ::contained_in(const interval_base_set& x2)const
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


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::add_intersection(interval_base_set& section, const value_type& x)const
{
    // any intersection with the empty intervall is empty
    if(x.empty()) 
        return;

    typename ImplSetT::const_iterator fst_it = _set.lower_bound(x);
    typename ImplSetT::const_iterator end_it = _set.upper_bound(x);

    for(typename ImplSetT::const_iterator it=fst_it; it != end_it; it++) 
    {
        interval_type isec; 
        (*it).intersect(isec, x);
        section.add(isec);
    }
}


template<class SubType,
         class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::join()
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

}} // namespace itl boost

#endif



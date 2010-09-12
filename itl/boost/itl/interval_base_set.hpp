/*-----------------------------------------------------------------------------+
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
#include <boost/itl/type_traits/interval_type_default.hpp>
#include <boost/itl/type_traits/is_interval_joiner.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/detail/interval_set_algo.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/interval.hpp>
#include <boost/itl/rightopen_interval.hpp> //JODO inclusion and customization of interval types
#include <boost/itl/continuous_interval.hpp> //JODO inclusion and customization of interval types
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/element_iterator.hpp>

#define const_FOR_IMPL(iter) for(typename ImplSetT::const_iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)
#define FOR_IMPL(iter) for(typename ImplSetT::iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)


namespace boost{namespace itl
{

//JODO Forward 4 gcc-3.4.4 -----------------------------------------------------

template<class Type, class OperandT>
typename enable_if<has_same_concept<is_interval_set, Type, OperandT>, bool>::type 
contains(const Type& super, const OperandT& sub);


template<class Type>
typename enable_if<is_interval_container<Type>, std::size_t>::type
interval_count(const Type&);

template<class Type>
typename enable_if<is_interval_container<Type>, typename Type::size_type>::type
cardinality(const Type&);

template<class Type>
typename enable_if<is_interval_container<Type>, typename Type::difference_type>::type
length(const Type&);

template<class Type>
typename enable_if<is_interval_set<Type>, bool>::type
contains(const Type& super, const typename Type::segment_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, bool>::type
contains(const Type& super, const typename Type::element_type&);

//- add ------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
add(Type&, const typename Type::element_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
add(Type&, const typename Type::segment_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, typename Type::iterator>::type
add(Type&, typename Type::iterator, 
     const typename Type::segment_type&);


template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
subtract(Type&, const typename Type::element_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
subtract(Type&, const typename Type::segment_type&);

//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, void>::type
add_intersection(Type&, const Type&, const typename Type::domain_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, void>::type
add_intersection(Type&, const Type&, const typename Type::segment_type&);

template<class Type, class OperandT>
typename enable_if<mpl::and_<is_interval_set<Type>, 
                             combines_right_to_interval_set<Type, OperandT> >,
                   void>::type
add_intersection(Type&, const Type&, const OperandT&);

//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
flip(Type&, const typename Type::element_type&);

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
flip(Type&, const typename Type::segment_type&);

template<class Type, class OperandT>
typename enable_if<is_concept_compatible<is_interval_set, Type, OperandT>, Type>::type&
flip(Type&, const OperandT&);

//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_container<Type>, Type>::type&
join(Type&);

//JODO Forward 4 gcc-3.4.4 -----------------------------------------------------


/** \brief Implements a set as a set of intervals (base class) */
template 
<
    typename             SubType,
    typename             DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_INTERVAL(ITL_COMPARE) Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, DomainT, Compare),
    ITL_ALLOC   Alloc    = std::allocator
> 
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
    typedef ITL_INTERVAL_TYPE(Interval,DomainT,Compare) interval_type;
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
    typedef exclusive_less_than<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less_than<interval_type> key_compare;

    //--------------------------------------------------------------------------
    //- Associated types: Implementation and stl related
    //--------------------------------------------------------------------------
    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// Container type for the implementation 
    typedef typename ITL_IMPL_SPACE::set<interval_type,key_compare,allocator_type> ImplSetT;

    /// key type of the implementing container
    typedef typename ImplSetT::key_type   key_type;
    /// data type of the implementing container
    typedef typename ImplSetT::key_type   data_type;
    /// value type of the implementing container
    typedef typename ImplSetT::value_type value_type;

    /// pointer type
    typedef typename ImplSetT::pointer         pointer;
    /// const pointer type
    typedef typename ImplSetT::const_pointer   const_pointer;
    /// reference type
    typedef typename ImplSetT::reference       reference;
    /// const reference type
    typedef typename ImplSetT::const_reference const_reference;

    /// iterator for iteration over intervals
    typedef typename ImplSetT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_iterator const_iterator;
    /// iterator for reverse iteration over intervals
    typedef typename ImplSetT::reverse_iterator reverse_iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_reverse_iterator const_reverse_iterator;

    /// element iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<iterator> element_iterator; 
    /// element const iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<const_iterator> element_const_iterator; 
    /// element reverse iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<reverse_iterator> element_reverse_iterator; 
    /// element const reverse iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<const_reverse_iterator> element_const_reverse_iterator; 

    enum { fineness = 4 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor for the empty object */
    interval_base_set(){}

    /** Copy constructor */
    interval_base_set(const interval_base_set& src): _set(src._set){}

    /** Assignment operator */
    interval_base_set& operator = (const interval_base_set& src) 
    { 
        this->_set = src._set;
        return *this; 
    }

    /** swap the content of containers */
    void swap(interval_base_set& operand) { _set.swap(operand._set); }

    //==========================================================================
    //= Containedness
    //==========================================================================

    /** sets the container empty */
    void clear() { itl::clear(*that()); }
    /** is the container empty? */
    bool empty()const { return itl::is_empty(*that()); }


    /** Does the set contain some object \c sub of type \c CoType */
    template<class CoType>
    bool contains(const CoType& sub)const
    {
        return itl::contains(*that(), sub);
    }

    /** Is <tt>*this</tt> container within <tt>super</tt>? */
    template<class CoType>
    bool within(const CoType& super)const
    {
        return itl::within(*that(), super);
    }



    //==========================================================================
    //= Size
    //==========================================================================

    /** Number of elements in the set (cardinality). 
        Infinite for continuous domain datatyps    */
    size_type cardinality()const
    {
        return itl::cardinality(*that());
    }

    /** An interval set's size is it's cardinality */
    size_type size()const
    {
        return itl::cardinality(*that());
    }

    /** The length of the interval container which is the sum of interval lengths */
    difference_type length()const
    {
        return itl::length(*that());
    }

    /** Number of intervals which is also the size of the iteration over the object */
    std::size_t interval_count()const 
    { 
        return itl::interval_count(*that()); 
    }

    /** Size of the iteration over this container */
    std::size_t iterative_size()const 
    { 
        return _set.size(); 
    }

    //==========================================================================
    //= Range
    //==========================================================================

    //JODO remove lower, upper, first, last from the interface of all interval containers and docs

    //==========================================================================
    //= Selection
    //==========================================================================

    /** Find the interval value pair, that contains element \c key */
    const_iterator find(const element_type& key)const
    { 
        return this->_set.find(interval_type(key)); 
    }

    const_iterator find(const segment_type& segment)const
    { 
        return this->_set.find(segment); 
    }

    //==========================================================================
    //= Addition
    //==========================================================================

    /** Add a single element \c key to the set */
    SubType& add(const element_type& key) 
    {
        return itl::add(*that(), key);
    }

    /** Add an interval of elements \c inter_val to the set */
    SubType& add(const segment_type& inter_val) 
    {
        return itl::add(*that(), inter_val);
    }

    /** Add an interval of elements \c inter_val to the set. Iterator 
        \c prior_ is a hint to the position \c inter_val can be 
        inserted after. */
    iterator add(iterator prior_, const segment_type& inter_val) 
    { 
        return itl::add(*that(), prior_, inter_val);
    }

    //==========================================================================
    //= Subtraction
    //==========================================================================

    /** Subtract a single element \c key from the set */
    SubType& subtract(const element_type& key) 
    { 
        return itl::subtract(*that(), key);
    }

    /** Subtract an interval of elements \c inter_val from the set */
    SubType& subtract(const segment_type& inter_val) 
    { 
        return itl::subtract(*that(), inter_val);
    }

    //==========================================================================
    //= Insertion, erasure
    //==========================================================================

    std::pair<iterator,bool> _insert(const value_type& value){ return this->_set.insert(value); }
    iterator _insert(iterator prior, const value_type& value){ return this->_set.insert(prior, value); }


    /** Insert an element \c key into the set */
    SubType& insert(const element_type& key) 
    { 
        return itl::add(*that(), key); 
    }

    /** Insert an interval of elements \c inter_val to the set */
    SubType& insert(const segment_type& inter_val) 
    { 
        return itl::add(*that(), inter_val); 
    }

    /** Insert an interval of elements \c inter_val to the set. Iterator 
        \c prior_ is a hint to the position \c inter_val can be 
        inserted after. */
    iterator insert(iterator prior_, const segment_type& inter_val) 
    { 
        return itl::add(*that(), prior_, inter_val); 
    }



    /** Erase an element \c key from the set */
    SubType& erase(const element_type& key) 
    { 
        return itl::subtract(*that(), key); 
    }

    /** Erase an interval of elements \c inter_val from the set */
    SubType& erase(const segment_type& inter_val) 
    { 
        return itl::subtract(*that(), inter_val); 
    }

    /** Erase the interval that iterator \c position points to. */
    void erase(iterator position)
    { 
        _set.erase(position); 
    }

    /** Erase all intervals in the range <tt>[first,past)</tt> of iterators. */
    void erase(iterator first, iterator past)
    { 
        _set.erase(first, past); 
    }


    //==========================================================================
    //= Intersection
    //==========================================================================

    /** The intersection of \c key in \c *this set is added to \c section. 
        The function can be used as a find function. */
    void add_intersection(SubType& section, const element_type& key)const
    {
        itl::add_intersection(section, *that(), key); 
    }

    /** The intersection of \c inter_val in \c *this set is added to \c section. */
    void add_intersection(SubType& section, const segment_type& inter_val)const
    {
        itl::add_intersection(section, *that(), inter_val); 
    }


    /** The intersection of set \c sectant with \c *this set is added to \c section. */
    template
    <
        template<class DomT, ITL_COMPARE Comp, 
                 ITL_INTERVAL2(ITL_COMPARE) Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void add_intersection
    (
        SubType& section,
        const IntervalSet<DomainT,Compare,Interval,Alloc>& operand
    )const
    {
        itl::add_intersection(section, *that(), operand);
    }


    /** Returns \c true, if element \c key is found in \c *this map.
        Complexity: logarithmic. */
    bool intersects(const element_type& key)const
    { 
        return itl::intersects(*that(), key); 
    }

    /** Returns \c true, if \c inter_val intersects with \c *this map.
        Complexity: logarithmic. */
    bool intersects(const interval_type& inter_val)const
    { 
        return itl::intersects(*that(), inter_val); 
    }

    //==========================================================================
    //= Symmetric difference
    //==========================================================================

    /** If \c *this set contains \c key it is erased, otherwise it is added. */
    SubType& flip(const element_type& key)
    {
        return itl::flip(*that(), key);
    }

    /** If \c *this set contains \c inter_val it is erased, otherwise it is added. */
    SubType& flip(const segment_type& inter_val)
    {
        return itl::flip(*that(), inter_val);
    }

    /** The intersection of \c *this and \c operand is erased from \c *this. 
        The complemenary elements are added to \c *this. */
    template<class SubType2>
    SubType& flip(const interval_base_set<SubType2,DomainT,Compare,Interval,Alloc>& operand)
    {
        return itl::flip(*that(), operand); //JODO remove this
    }

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

    std::pair<iterator,iterator> equal_range(const key_type& interval)
    { return _set.equal_range(interval); }

    std::pair<const_iterator,const_iterator> equal_range(const key_type& interval)const
    { return _set.equal_range(interval); }

    //==========================================================================
    //= Element iterator related
    //==========================================================================
    element_iterator elements_begin(){ return element_iterator(this->begin()); }
    element_iterator elements_end()  { return element_iterator(this->end());   }
    element_const_iterator elements_begin()const{ return element_const_iterator(this->begin()); }
    element_const_iterator elements_end()  const{ return element_const_iterator(this->end());   }

    element_reverse_iterator elements_rbegin(){ return element_reverse_iterator(this->rbegin()); }
    element_reverse_iterator elements_rend()  { return element_reverse_iterator(this->rend());   }
    element_const_reverse_iterator elements_rbegin()const{ return element_const_reverse_iterator(this->rbegin()); }
    element_const_reverse_iterator elements_rend()  const{ return element_const_reverse_iterator(this->rend());   }
    
    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    template<typename IteratorT>
    static const key_type& key_value(IteratorT value_){ return (*value_); }

    template<typename IteratorT>
    static codomain_type co_value(IteratorT value_)
    { return itl::is_empty(*value_)? codomain_type() : (*value_).lower(); }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return key_compare()(*lhs_, *rhs_); }

    static value_type make_value(const key_type& key_value, const codomain_type&)
    { return value_type(key_value); }

protected:
    sub_type* that() { return static_cast<sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

protected:
    ImplSetT _set;
} ;


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator == (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    return Set::lexicographical_equal(lhs, rhs);
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator != (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs == rhs); }


template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator < (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
        Compare<ITL_INTERVAL_TYPE(Interval,DomainT,Compare) >()//NOTE DESIGN TTP: Why Compare needs to be ttp
        );
}

template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator > (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                        const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return rhs < lhs; }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator <= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs > rhs); }

template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline bool operator >= (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& lhs,
                         const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& rhs)
{ return !(lhs < rhs); }

template<class CharType, class CharTraits, 
    class SubType, class DomainT, 
    ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> IntervalSetT;
    stream << "{";
    ITL_const_FORALL(typename IntervalSetT, it_, object)
        stream << (*it_);

    return stream << "}";
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template<class SubType,
         class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_base_set<SubType,DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};



}} // namespace itl boost

#endif



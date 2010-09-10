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
#include <boost/type_traits/ice.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/map.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/detail/interval_map_algo.hpp>


#define const_FOR_IMPLMAP(iter) for(typename ImplMapT::const_iterator iter=_map.begin(); (iter)!=_map.end(); (iter)++)
#define FOR_IMPLMAP(iter) for(typename ImplMapT::iterator iter=_map.begin(); (iter)!=_map.end(); (iter)++)

namespace boost{namespace itl
{

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif                        


template<class DomainT, class CodomainT>
struct mapping_pair
{
    DomainT   key;
    CodomainT data;

    mapping_pair():key(), data(){}

    mapping_pair(const DomainT& key_value, const CodomainT& data_value)
        :key(key_value), data(data_value){}

    mapping_pair(const std::pair<DomainT,CodomainT>& std_pair)
        :key(std_pair.first), data(std_pair.second){}
};

template<class Type>
inline typename enable_if<is_interval_map<Type>, typename Type::segment_type>::type
make_segment(const typename Type::element_type& element)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::segment_type  segment_type;
    return segment_type(interval_type(element.key), element.data);
}

/** \brief Implements a map as a map of intervals (base class) */
template
<
    class SubType,
    typename DomainT,
    typename CodomainT,
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section  = ITL_SECTION_INSTANCE(itl::inter_section, CodomainT), 
    ITL_INTERVAL(ITL_COMPARE) Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, DomainT, Compare),
    ITL_ALLOC   Alloc    = std::allocator
>
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
    typedef ITL_INTERVAL_TYPE(Interval,DomainT,Compare) interval_type;
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
    //CL typedef ITL_SECTION_CODOMAIN(Section,CodomainT) codomain_intersect;

    typedef typename mpl::if_
    <has_set_semantics<codomain_type>
    , ITL_SECTION_CODOMAIN(Section,CodomainT)     
    , codomain_combine
    >::type                                            codomain_intersect; //JODO extra metafuction?
    //JODO What, if codomain is not a set but the user want's to use a special intersection functor?


    /// Inverse Combine functor for codomain value intersection
    typedef typename inverse<codomain_intersect>::type inverse_codomain_intersect;

    /// Comparison functor for intervals which are keys as well
    typedef exclusive_less_than<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less_than<interval_type> key_compare;

    //--------------------------------------------------------------------------
    //- Associated types: Implementation and stl related
    //--------------------------------------------------------------------------
    /// The allocator type of the set
    typedef Alloc<std::pair<const interval_type, codomain_type> > 
        allocator_type;

    /// Container type for the implementation 
    typedef ITL_IMPL_SPACE::map<interval_type,codomain_type,
                                key_compare,allocator_type> ImplMapT;

    /// key type of the implementing container
    typedef typename ImplMapT::key_type   key_type;
    /// value type of the implementing container
    typedef typename ImplMapT::value_type value_type;
    /// data type of the implementing container
    typedef typename ImplMapT::value_type::second_type data_type;

    /// pointer type
    typedef typename ImplMapT::pointer         pointer;
    /// const pointer type
    typedef typename ImplMapT::const_pointer   const_pointer;
    /// reference type
    typedef typename ImplMapT::reference       reference;
    /// const reference type
    typedef typename ImplMapT::const_reference const_reference;

    /// iterator for iteration over intervals
    typedef typename ImplMapT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplMapT::const_iterator const_iterator;
    /// iterator for reverse iteration over intervals
    typedef typename ImplMapT::reverse_iterator reverse_iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplMapT::const_reverse_iterator const_reverse_iterator;

    /// element iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<iterator> element_iterator; 
    /// const element iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<const_iterator> element_const_iterator; 
    /// element reverse iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<reverse_iterator> element_reverse_iterator; 
    /// element const reverse iterator: Depreciated, see documentation.
    typedef boost::itl::element_iterator<const_reverse_iterator> element_const_reverse_iterator; 
    
public:
    enum { is_itl_container = true };
    enum { fineness = 0 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /** Default constructor for the empty object */
    interval_base_map()
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    /** Copy constructor */
    interval_base_map(const interval_base_map& src): _map(src._map)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    /** Assignment operator */
    interval_base_map& operator = (const interval_base_map& src) 
    { 
        this->_map = src._map;
        return *this; 
    }

    /** swap the content of containers */
    void swap(interval_base_map& object) { _map.swap(object._map); }

    /** Copy all elements if predicate <tt>pred</tt> holds */
    template<class Predicate>
    SubType& assign_if(const interval_base_map& src, const Predicate& pred);

    //==========================================================================
    //= Containedness
    //==========================================================================
    /** clear the map */
    void clear() { itl::clear(*that()); }

    /** is the map empty? */
    bool empty()const { return itl::is_empty(*that()); }

    
    /** Does the map contain some object \c sub of type \c CoType */
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

    /** Number of elements in the map (cardinality). */
    size_type cardinality()const
    {
        return itl::cardinality(*that());
    }

    /** An interval map's size is it's cardinality */
    size_type size()const
    {
        return itl::cardinality(*that());
    }

    /** The length of the interval container which is the sum of interval lengths */
    difference_type length()const
    {
        return itl::length(*that());
    }

    /** Number of intervals which is also the size of the 
        iteration over the object */
    size_t interval_count()const 
    { 
        return itl::interval_count(*that()); 
    }

    /** Size of the iteration over this container */
    size_t iterative_size()const 
    { 
        return _map.size(); 
    }

    //==========================================================================
    //= Range
    //==========================================================================

    //JODO remove lower, upper, first, last from the interface of all interval containers and docs

    //==========================================================================
    //= Selection
    //==========================================================================

    /** Find the interval value pair, that contains \c key */
    const_iterator find(const domain_type& key)const
    { 
        return _map.find(interval_type(key)); 
    }

    const_iterator find(const key_type& key)const
    { 
        return _map.find(key); 
    }

    /** Total select function. */
    codomain_type operator()(const domain_type& key)const
    {
        const_iterator it_ = _map.find(interval_type(key)); 
        return it_==end() ? neutron<codomain_type>::value()
                          : it_->second;
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
    SubType& _add(const segment_type& interval_value_pair)
    { 
        that()->template add_<Combiner>(interval_value_pair); 
        return *that(); 
    }

public:
    /** Addition of a key value pair to the map */
    SubType& add(const element_type& key_value_pair) 
    {
        return itl::add(*that(), key_value_pair);
    }

    /** Addition of an interval value pair to the map. */
    SubType& add(const segment_type& interval_value_pair) 
    {
        return itl::add(*that(), interval_value_pair);
    }

    /** Addition of an interval value pair \c interval_value_pair to the map. 
        Iterator \c prior_ is a hint to the position \c interval_value_pair can be 
        inserted after. */
    iterator add(iterator prior_, const segment_type& interval_value_pair) 
    {
        return itl::add(*that(), prior_, interval_value_pair);
    }

    //==========================================================================
    //= Subtraction
    //==========================================================================
private:
    /** Subtraction of an interval value pair from the map.
        On overlap an aggregation is performed using functor Combiner.
        This function is not public, because the `codomain_combine` shall be
        an invariant for all itl maps.*/
    template<class Combiner>
    SubType& _subtract(const segment_type& interval_value_pair)
    { 
        that()->template subtract_<Combiner>(interval_value_pair); 
        return *that(); 
    }

public:
    /** Subtraction of a key value pair from the map */
    SubType& subtract(const element_type& key_value_pair)
    { 
        return itl::subtract(*that(), key_value_pair);
    }

    /** Subtraction of an interval value pair from the map. */
    SubType& subtract(const segment_type& interval_value_pair)
    {
        return itl::subtract(*that(), interval_value_pair);
    }

    //==========================================================================
    //= Insertion
    //==========================================================================

    std::pair<iterator,bool> _insert(const value_type& value_pair){ return _map.insert(value_pair); }
    iterator _insert(iterator prior, const value_type& value_pair){ return _map.insert(prior, value_pair); }

    /** Insertion of a \c key_value_pair into the map. */
    SubType& insert(const element_type& key_value_pair) 
    {
        return itl::insert(*that(), key_value_pair);
    }

    /** Insertion of an \c interval_value_pair into the map. */
    SubType& insert(const segment_type& interval_value_pair)
    { 
        return itl::insert(*that(), interval_value_pair);
    }

    /** Insertion of an \c interval_value_pair into the map. Iterator \c prior_. 
        serves as a hint to insert after the element \c prior point to. */
    iterator insert(iterator prior, const segment_type& interval_value_pair)
    { 
        return itl::insert(*that(), prior, interval_value_pair);
    }

    /** With <tt>key_value_pair = (k,v)</tt> set value \c v for key \c k */
    SubType& set(const element_type& key_value_pair) 
    { 
        return itl::set_at(*that(), key_value_pair);
    }

    /** With <tt>interval_value_pair = (I,v)</tt> set value \c v 
        for all keys in interval \c I in the map. */
    SubType& set(const segment_type& interval_value_pair)
    { 
        return itl::set_at(*that(), interval_value_pair);
    }


    //==========================================================================
    //= Erasure
    //==========================================================================

    template<class CoType>
    SubType& erase(const CoType& operand) 
    {
        return itl::erase(*that(), operand);
    }

    /** Erase all value pairs within the range of the interval that iterator 
        \c position points to. */
    void erase(iterator position){ _map.erase(position); }

    /** Erase all value pairs for a range of iterators <tt>[first,past)</tt>. */
    void erase(iterator first, iterator past){ _map.erase(first, past); }

    /** Remove all elements where property <tt>p</tt> holds, keep all others */
    template<class Predicate>
    SubType& erase_if(const Predicate& pred);

    //==========================================================================
    //= Intersection
    //==========================================================================
    /** Returns \c true, if element \c key is found in \c *this map.
        Complexity: logarithmic. */
    bool intersects(const domain_type& key)const
    {
		return itl::intersects(*that(), key);
    }

    /** Returns \c true, if \c inter_val intersects with \c *this map.
        Complexity: Logarithmic in iterative size. */
    bool intersects(const interval_type& inter_val)const
    { 
		return itl::intersects(*that(), inter_val);
    }

    /** Returns \c true, if \c key_value_pair is found in \c *this map.
        Complexity: logarithmic. */
    bool intersects(const element_type& key_value_pair)const
    { 
		return itl::intersects(*that(), key_value_pair);
    }

    /** Returns \c true, if \c interval_value_pair intersects with \c *this map:
        Complexity: Linear in iterative_size. */
    bool intersects(const segment_type& interval_value_pair)const
    {
		return itl::intersects(*that(), interval_value_pair);
    }

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

    std::pair<iterator,iterator> equal_range(const key_type& interval)
    { return _map.equal_range(interval); }

    std::pair<const_iterator,const_iterator> equal_range(const key_type& interval)const
    { return _map.equal_range(interval); }

    iterator begin() { return _map.begin(); }
    iterator end()   { return _map.end(); }
    const_iterator begin()const { return _map.begin(); }
    const_iterator end()const   { return _map.end(); }
    reverse_iterator rbegin() { return _map.rbegin(); }
    reverse_iterator rend()   { return _map.rend(); }
    const_reverse_iterator rbegin()const { return _map.rbegin(); }
    const_reverse_iterator rend()const   { return _map.rend(); }

    iterator prior(iterator it_)
    { return it_ == this->_map.begin() ? this->_map.end() : --it_; }

    const_iterator prior(const_iterator it_)const
    { return it_ == this->_map.begin() ? this->_map.end() : --it_; }


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
        <class DomT, ITL_COMPARE Comp, ITL_INTERVAL2(ITL_COMPARE) Interv, ITL_ALLOC Allc>
        class IntervalSet
    >
    void domain(IntervalSet<DomainT,Compare,Interval,Alloc>& dom)const 
    { 
        dom.clear(); 
        const_FOR_IMPLMAP(it_) 
            dom += it_->first; 
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
    static const key_type& key_value(IteratorT value_){ return (*value_).first; }

    template<typename IteratorT>
    static codomain_type co_value(IteratorT value_){ return (*value_).second; }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT left_, RightIterT right_) 
    { return key_compare()((*left_).first, (*right_).first); }

    static value_type make_value(const key_type& key_value, const codomain_type& codom_val)
    { return value_type(key_value, codom_val); }

protected:
    sub_type* that() { return static_cast<sub_type*>(this); }
    const sub_type* that()const { return static_cast<const sub_type*>(this); }

public:
    sub_type& self() { return *that(); }

protected:

    template <class Combiner>
    bool combine(iterator& it_, const codomain_type& co_val)
    { 
        Combiner()(it_->second, co_val);
        if(Traits::absorbs_neutrons && it_->second == Combiner::neutron())
        { this->_map.erase(it_); it_ = _map.end(); return false; }
        return true;
    }

    template <class Combiner>
    std::pair<iterator,bool> _map_insert(const interval_type& inter_val, const codomain_type& co_val)
    {
        if(mpl::and_<is_total<type>, has_inverse<codomain_type>, is_negative<Combiner> >::value)
            return this->_map.insert(value_type(inter_val, version<Combiner>()(co_val)));
        else
            return this->_map.insert(value_type(inter_val, co_val));
    }

    // Insertion with hint, that does report insertion failure
    template <class Combiner>
    std::pair<iterator, bool> 
        _map_insert(iterator prior_, const interval_type& inter_val, const codomain_type& co_val)
    {
        iterator inserted_
            = this->_map.insert(prior_, value_type(inter_val, co_val));

        if(inserted_ == prior_)
            return std::pair<iterator,bool>(inserted_, false);
        else if(inserted_->first == inter_val)
            return std::pair<iterator,bool>(inserted_, true);
        else
            return std::pair<iterator,bool>(inserted_, false);
    }

    template <class Combiner>
    std::pair<iterator, bool> 
        _map_add(iterator prior_, const interval_type& inter_val, const codomain_type& co_val)
    {
        // Never try to insert a neutron into a neutron absorber here:
        BOOST_ASSERT(!(Traits::absorbs_neutrons && co_val==Combiner::neutron()));

        iterator inserted_ 
            = this->_map.insert(prior_, value_type(inter_val, Combiner::neutron()));

        if(inserted_->first == inter_val && inserted_->second == Combiner::neutron())
        {
            Combiner()(inserted_->second, co_val);
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

        return this->_map.insert(prior_, value_type(inter_val, version<Combiner>()(co_val)));
    }

protected:
    ImplMapT _map;
} ;



//==============================================================================
//= Symmetric difference
//==============================================================================

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
        ITL_FORALL(typename ImplMapT, it_, _map)
            it_->second = neutron<codomain_type>::value();

        if(mpl::not_<is_interval_splitter<SubType> >::value)
            join();

        return *that();
    }

    interval_type span = interval_value_pair.first;

    typename ImplMapT::const_iterator first_ = _map.lower_bound(span);
    typename ImplMapT::const_iterator end_   = _map.upper_bound(span);

    interval_type covered, left_over, common_interval;
    const codomain_type& x_value = interval_value_pair.second;
    typename ImplMapT::const_iterator it_ = first_;

    interval_set<DomainT,Compare,Interval,Alloc> eraser;
    interval_base_map intersection;

    while(it_ != end_  ) 
    {
        const codomain_type& co_value = it_->second;
        covered = (*it_++).first;
        //[a      ...  : span
        //     [b ...  : covered
        //[a  b)       : left_over
        left_over = right_subtract(span, covered);

        //That which is common ...
        common_interval = span & covered;
        if(!itl::is_empty(common_interval))
        {
            // ... shall be subtracted
            eraser.add(common_interval);

            if(has_set_semantics<codomain_type>::value)
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
        //... c)      : covered
        //     [c  d) : span'
        span = left_subtract(span, covered);
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
    ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
        ITL_FORALL(typename ImplMapT, it_, _map)
            it_->second = neutron<codomain_type>::value();

        if(mpl::not_<is_interval_splitter<SubType> >::value)
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
        ITL_FORALL(typename ImplMapT, it_, _map)
            it_->second = neutron<codomain_type>::value();

    return *that();
}



template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
        if(    touches(it_->first, next_->first)
            && it_->second == next_->second      )
        {
            iterator fst_mem = it_;  // hold the first member
            
            // Go on while touching members are found
            it_++; next_++;
            while(     next_ != _map.end()
                    && touches(it_->first, next_->first)
                    && it_->second == next_->second     )
            { it_++; next_++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence            
            const_cast<interval_type&>(it_->first) = hull(it_->first, fst_mem->first);
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
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::sum(codomain_type& total)const
{
    total = codomain_combine::neutron();
    const_FOR_IMPLMAP(it_) 
        total += it_->second;
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
void interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::uniform_bounds(itl::bound_type bounded)
{
    // I can do this only, because I am sure that the contents and the
    // ordering < on interval is invariant wrt. this transformation on bounds
    FOR_IMPLMAP(it_) const_cast<interval_type&>(it_->first).as(bounded);
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
    template<class Predicate>
SubType& 
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_if(const Predicate& pred)
{
    iterator it = _map.begin();
    while(it != _map.end())
        if(pred(*it))
            _map.erase(it++); 
        else ++it;
    return *that();
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
    template<class Predicate>
SubType& 
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::assign_if(const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& src, 
                const Predicate& pred)
{
    clear();
    const_iterator it = src.begin();
    while(it != src.end()) {
        if(Predicate()(*it)) 
            add(*it++); 
    }
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
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool operator == (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                         const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return Set::lexicographical_equal(lhs, rhs);
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool operator != (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                         const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return !(lhs == rhs);
}


template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool is_protonic_equal(const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                              const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return Map::lexicographical_protonic_equal(lhs, rhs);
}


template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE) Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, ITL_INTERVAL(ITL_COMPARE), ITL_ALLOC
    >
    class IntervalMap
>
bool is_protonic_equal
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Map::lexicographical_protonic_equal(left, right);
}



//-----------------------------------------------------------------------------
// order
//-----------------------------------------------------------------------------
template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool operator < (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
        Compare<std::pair<ITL_INTERVAL_TYPE(Interval,DomainT,Compare),CodomainT> >()//NOTE DESIGN TTP: Why template template parameter Compare is needed
        );
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool operator > (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{ return rhs < lhs; }

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
inline bool operator <= (const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& lhs,
                        const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& rhs)
{ return !(lhs > rhs); }

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
    ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
    ITL_const_FORALL(typename map_type, elem_, operand) 
        object.template _add<inplace_min >(*elem_); 

    return object; 
}

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
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
    ITL_const_FORALL(typename map_type, elem_, operand) 
        object.template _add<inplace_max>(*elem_); 

    return object; 
}

//---------------------------------------------------------------------------------

template<class CharType, class CharTraits, 
    class SubType, class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
    ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_base_map<SubType,DomainT,CodomainT,Traits,
                           Compare,Combine,Section,Interval,Alloc>& object)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> IntervalMapT;
    stream << "{";
    ITL_const_FORALL(typename IntervalMapT, it_, object)
        stream << "(" << it_->first << "->" << it_->second << ")";

    return stream << "}";
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
struct is_map<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
struct has_inverse<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
struct is_interval_container<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
struct absorbs_neutrons<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    typedef absorbs_neutrons<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::absorbs_neutrons)); 
};

template 
<
    class SubType,
    class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc
>
struct is_total<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    typedef is_total<itl::interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::is_total)); 
};



#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

}} // namespace itl boost

#endif



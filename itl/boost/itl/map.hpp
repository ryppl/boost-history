/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_MAP_HPP_JOFA_070519
#define BOOST_ITL_MAP_HPP_JOFA_070519

#include <boost/itl/impl_config.hpp>

#if defined(ITL_USE_BOOST_INTERPROCESS_IMPLEMENTATION)
#include <boost/interprocess/containers/map.hpp>
#elif defined(ITL_USE_BOOST_MOVE_IMPLEMENTATION)
#include <boost/container/map.hpp>
#else 
#include <map>
#endif

#include <string>
#include <boost/type_traits/ice.hpp>
#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/detail/concept_check.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/seqs.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/map_functions.hpp>
#include <boost/itl/detail/map_algo.hpp>
#include <boost/itl/detail/map_functors.hpp>


namespace boost{namespace itl
{

struct partial_absorber
{
    enum { absorbs_neutrons = true };
    enum { is_total = false };
};

template<> 
inline std::string type_to_string<partial_absorber>::apply() { return "@0"; }


struct partial_enricher
{
    enum { absorbs_neutrons = false };
    enum { is_total = false };

};

template<> 
inline std::string type_to_string<partial_enricher>::apply() { return "e0"; }

struct total_absorber
{
    enum { absorbs_neutrons = true };
    enum { is_total = true };
};

template<> 
inline std::string type_to_string<total_absorber>::apply() { return "^0"; }

struct total_enricher
{
    enum { absorbs_neutrons = false };
    enum { is_total = true };
};

template<> 
inline std::string type_to_string<total_enricher>::apply() { return "e^0"; }


template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION, ITL_ALLOC>
class map;

//==============================================================================
//=+ seqs
//==============================================================================

template<class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct seqs< itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{
    typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> map_type;
    typedef typename map_type::domain_type    domain_type;
    typedef typename map_type::codomain_type  codomain_type;
    typedef typename map_type::element_type   element_type;
    typedef typename map_type::size_type      size_type;
    typedef typename map_type::domain_compare domain_compare;

    typedef typename map_type::iterator       iterator;
    typedef typename map_type::const_iterator const_iterator;

    static const_iterator begin(const map_type& object){ return object.begin(); }
    static       iterator begin(      map_type& object){ return object.begin(); }
    static const_iterator end  (const map_type& object){ return object.end();   }
    static       iterator end  (      map_type& object){ return object.end();   }
    static      size_type size (const map_type& object){ return object.size();  }

    static const_iterator find(const map_type& object, const domain_type& key)
    { return object.find(key); }

    static iterator find(map_type& object, const domain_type& key)
    { return object.find(key); }

    static std::pair<iterator,bool> insert(map_type& object, const element_type& key_value_pair)
    { return object.insert(key_value_pair); }

    static iterator insert(map_type& object, iterator prior, const element_type& key_value_pair)
    { return object.insert(prior, key_value_pair); }

    static void erase(map_type& object, iterator pos)
    { object.erase(pos); }

    static void erase(map_type& object, iterator first, iterator past)
    { object.erase(first, past);  }

    static void swap(map_type& left, map_type& right)
    { left.swap(right);    }

    //--------------------------------------------------------------------------
    template<typename IteratorT>
    static const domain_type& key_value(IteratorT value_){ return value_->first; }

    template<typename IteratorT>
    static const codomain_type& co_value(IteratorT value_){ return value_->second; }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return domain_compare()((*lhs_).first,(*rhs_).first); }

    static element_type make_value(const domain_type& key_value, const codomain_type& data_value)
    { return element_type(key_value, data_value); }
};

//==============================================================================


/** \brief Addable, subractable and intersectable maps */
template 
<
    typename DomainT, 
    typename CodomainT, 
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inter_section, CodomainT), 
    ITL_ALLOC   Alloc   = std::allocator 
>
class map: private ITL_IMPL_SPACE::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), 
                                       Alloc<std::pair<const DomainT, CodomainT> > >
{
public:
    typedef Alloc<typename std::pair<const DomainT, CodomainT> >  allocator_type;

    typedef typename itl::map<DomainT,CodomainT,Traits, Compare,Combine,Section,Alloc> type;
    typedef typename ITL_IMPL_SPACE::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT),
                                         allocator_type>   base_type;
    typedef typename itl::set<DomainT, Compare, Alloc >    set_type;

    typedef Traits traits;

public:
    typedef DomainT                                     domain_type;
    typedef DomainT                                     key_type;
    typedef CodomainT                                   codomain_type;
    typedef CodomainT                                   mapped_type;
    typedef CodomainT                                   data_type;
    typedef std::pair<const DomainT, CodomainT>         element_type;
    typedef std::pair<const DomainT, CodomainT>         value_type;
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT)         domain_compare;
    typedef ITL_COMBINE_CODOMAIN(Combine,CodomainT)     codomain_combine;
    typedef domain_compare                              key_compare;
    typedef ITL_COMPARE_DOMAIN(Compare,element_type)    element_compare;
    typedef typename inverse<codomain_combine >::type   inverse_codomain_combine;
    typedef typename mpl::if_
        <is_set<codomain_type>
        , ITL_SECTION_CODOMAIN(Section,CodomainT)     
        , codomain_combine
        >::type                                         codomain_intersect; //JODO extra metafuction?
        //JODO What, if codomain is not a set but the user want's to use a special intersection functor?
    typedef typename inverse<codomain_intersect>::type  inverse_codomain_intersect;
    typedef typename base_type::value_compare           value_compare;

    BOOST_STATIC_CONSTANT(bool, _total   = (Traits::is_total));
    BOOST_STATIC_CONSTANT(bool, _absorbs = (Traits::absorbs_neutrons));
    BOOST_STATIC_CONSTANT(bool, 
        total_invertible = (mpl::and_<is_total<type>, has_inverse<codomain_type> >::value));

    typedef map_add<type,codomain_combine,_absorbs>     add_;  
    typedef map_subtract<type,total_invertible>         subtract_;  

    typedef map_insert<type,codomain_combine,_absorbs>  insert_;  
    typedef map_erase<type,codomain_combine,_absorbs>   erase_;  

    typedef map_add_intersection<type,_total>           add_intersection_;  
    typedef map_inplace_intersect<type,_total>          inplace_intersect_;  
    typedef map_intersects<type,_total>                 intersects_;  

    typedef map_flip<type,_total,_absorbs>              flip_;  

public:
    typedef typename base_type::pointer                 pointer;
    typedef typename base_type::const_pointer           const_pointer;
    typedef typename base_type::reference               reference;
    typedef typename base_type::const_reference         const_reference;
    typedef typename base_type::iterator                iterator;
    typedef typename base_type::const_iterator          const_iterator;
    typedef typename base_type::size_type               size_type;
    typedef typename base_type::difference_type         difference_type;
    typedef typename base_type::reverse_iterator        reverse_iterator;
    typedef typename base_type::const_reverse_iterator  const_reverse_iterator;
    
    enum { fineness = 4 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    map()
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    map(const key_compare& comp): base_type(comp){}

    template <class InputIterator>
    map(InputIterator first, InputIterator past)
        : base_type(first,past){} 

    template <class InputIterator>
    map(InputIterator first, InputIterator past, const key_compare& comp)
        : base_type(first,past,comp) 
    {}

    map(const map& src): base_type::map(src)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<CodomainT>));
        BOOST_CONCEPT_ASSERT((EqualComparableConcept<CodomainT>));
    }

    explicit map(const element_type& key_value_pair): base_type::map()
    { 
        insert(key_value_pair); 
    }

    map& operator=(const map& src) { base_type::operator=(src); return *this; } 
    void swap(map& src) { base_type::swap(src); }

    //==========================================================================
    using base_type::empty;
    using base_type::clear;

    using base_type::begin;
    using base_type::end;
    using base_type::rbegin;
    using base_type::rend;

    using base_type::size;
    using base_type::max_size;

    using base_type::key_comp;
    using base_type::value_comp;

    using base_type::insert;
    using base_type::erase;
    using base_type::find;
    using base_type::count;

    using base_type::lower_bound;
    using base_type::upper_bound;
    using base_type::equal_range;

    using base_type::operator[];

public:
    //==========================================================================
    //= Containedness
    //==========================================================================

    template<class SubObject>
    bool contains(const SubObject& sub)const 
    { return itl::contains(*this, sub); }

    bool within(const map& super)const 
    { return itl::contains(super, *this); }

    //==========================================================================
    //= Size
    //==========================================================================
    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    size_t iterative_size()const { return size(); } //JODO 

    //CL?? size_t cardinality()const { return size(); }

    //==========================================================================
    //= Selection
    //==========================================================================

    /** Total select function. */
    codomain_type operator()(const domain_type& key)const
    {
        const_iterator it = find(key); 
        return it==end() ? neutron<codomain_type>::value()
                         : it->second;
    }

    //==========================================================================
    //= Addition, subtraction
    //==========================================================================
    /** \c add inserts \c value_pair into the map if it's key does 
        not exist in the map.    
        If \c value_pairs's key value exists in the map, it's data
        value is added to the data value already found in the map. */
    map& add(const value_type& value_pair) 
    { 
        return itl::add(*this, value_pair); 
    }

    /** \c add add \c value_pair into the map using \c prior as a hint to
        insert \c value_pair after the position \c prior is pointing to. */
    iterator add(iterator prior, const value_type& value_pair) 
    { 
        return itl::add(*this, prior, value_pair); 
    }

    /** If the \c value_pair's key value is in the map, it's data value is
        subtraced from the data value stored in the map. */
    map& subtract(const value_type& value_pair)
    {
        return itl::subtract(*this, value_pair);
    }

    //==========================================================================
    //= Insertion, erasure
    //==========================================================================

    /** With <tt>key_value_pair = (k,v)</tt> set value \c v for key \c k */
    map& set(const element_type& key_value_pair)
    { 
        (*this)[key_value_pair.first] = key_value_pair.second; //JODO absorbs_neutrons?
        return *this; 
    }

    /** erase \c key_value_pair from the map.
        Erase only if, the exact value content \c val is stored for the given key. */
    size_type erase(const element_type& key_value_pair);

    //==========================================================================
    //= Intersection
    //==========================================================================

    /** The intersection of \c operand in \c *this map is added to \c section. */
    template<class OperandT>
    map& add_intersection(map& section, const OperandT& operand)const
    { return itl::add_intersection(section, *this, operand); }

    /** Returns true, if there is an intersection of \c operand and \c *this map. */
    template<class OperandT>
    bool intersects(const OperandT& operand)const
    {
        return itl::intersects(*this, operand);
    }

    //==========================================================================
    //= Symmetric difference
    //==========================================================================

    map& flip(const element_type& value_pair)
    { 
        return (*this) ^= value_pair; 
    }

    //==========================================================================
    //= Algorithm unifiers
    //==========================================================================

    /** \c key_value allows for a uniform access to \c key_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const key_type& key_value(IteratorT value_){ return (*value_).first; }

    /** \c codomain_value allows for a uniform access to \c codomain_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const codomain_type& codomain_value(IteratorT value_){ return (*value_).second; }

    /** \c key_less allows for a uniform notation of key comparison which
        is used for common algorithms on sets and maps. */
    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_) 
    { return key_compare()((*lhs_).first,(*rhs_).first); }

    static value_type make_value(const key_type& key_value, const codomain_type& data_value)
    { return value_type(key_value, data_value); }

    /** Copy the key values of the map to \c domain_set. Complexity: Linear. */
    void domain(set_type& domain_set)const
    {
        typename set_type::iterator prior_ = domain_set.end();
        ITL_const_FORALL_THIS(it_)
            prior_ = domain_set.insert(prior_, it_->first);
    }
};




//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (is_set<CodomainT>::value)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef absorbs_neutrons type;
    BOOST_STATIC_CONSTANT(int, value = Traits::absorbs_neutrons); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_total<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
    typedef is_total type;
    BOOST_STATIC_CONSTANT(int, value = Traits::is_total); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct type_to_string<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{
    static std::string apply()
    {
        return "map<"+ type_to_string<DomainT>::apply()  + ","
                     + type_to_string<CodomainT>::apply() + ","
                     + type_to_string<Traits>::apply() +">"; 
    }
};



}} // namespace itl boost

#endif // BOOST_ITL_MAP_HPP_JOFA_070519


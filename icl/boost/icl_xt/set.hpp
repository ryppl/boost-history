/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_XT_SET_HPP_JOFA_070519
#define BOOST_ICL_XT_SET_HPP_JOFA_070519

#include <boost/icl/impl_config.hpp>

#if defined(ICL_USE_BOOST_INTERPROCESS_IMPLEMENTATION)
#include <boost/interprocess/containers/set.hpp>
#elif defined(ICL_USE_BOOST_MOVE_IMPLEMENTATION)
#include <boost/container/set.hpp>
#else 
#include <set>
#endif

#include <string>
#include <boost/icl/detail/design_config.hpp>
#include <boost/icl/detail/concept_check.hpp>
#include <boost/icl/type_traits/to_string.hpp>
#include <boost/icl/type_traits/is_set.hpp>
#include <boost/icl/detail/subset_comparer.hpp>
#include <boost/icl/detail/set_algo.hpp>
#include <boost/icl/predicates.hpp>
#include <boost/mpl/or.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/type_traits/is_same.hpp>

#include <boost/icl/concept/container.hpp>
#include <boost/icl/concept/element_set.hpp>
#include <boost/icl/concept/element_associator.hpp>
#include <boost/icl/concept/comparable.hpp>


namespace boost{namespace icl
{

/** \brief Addable, subtractable and intersectable sets. */
template 
<
    typename DomainT, 
    ICL_COMPARE Compare = ICL_COMPARE_INSTANCE(std::less, DomainT),
    ICL_ALLOC   Alloc   = std::allocator 
>
class set: private ICL_IMPL_SPACE::set<DomainT, ICL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> >
{
public:
    typedef typename icl::set<DomainT, Compare, Alloc> type;
    typedef typename ICL_IMPL_SPACE::set<DomainT, ICL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> > base_type;
    typedef type key_object_type;

public:
    typedef DomainT     domain_type;
    typedef DomainT     codomain_type;
    typedef DomainT     element_type;
    typedef DomainT     key_type;
    typedef DomainT     value_type;
    typedef DomainT     data_type;
    typedef ICL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef domain_compare                      element_compare;
    typedef ICL_COMPARE_DOMAIN(Compare,DomainT) key_compare;
    typedef ICL_COMPARE_DOMAIN(Compare,DomainT) value_compare;
    typedef Alloc<DomainT>   allocator_type;

public:        
    typedef typename base_type::pointer                pointer;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    enum { fineness = 4 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    set()
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    explicit set(const domain_compare& comp): base_type(comp){}

    template <class InputIterator>
    set(InputIterator first, InputIterator past)
        : base_type(first,past){}

    template <class InputIterator>
    set(InputIterator first, InputIterator past, const key_compare& comp)
        : base_type(first, past, comp){}

    set(const set& src): base_type::set(src)
    {
        BOOST_CONCEPT_ASSERT((DefaultConstructibleConcept<DomainT>));
        BOOST_CONCEPT_ASSERT((LessThanComparableConcept<DomainT>));
    }

    explicit set(const element_type& key): base_type::set()
    { 
        insert(key); 
    }

    set& operator=(const set& src) { base_type::operator=(src); return *this; } 
    void swap(set& src) { base_type::swap(src); }

    /** Copy the elements in set \c src to which property \c hasProperty applies 
        into \c *this set. */
    template<class Predicate>
    set& assign_if(const set& src, const Predicate&);

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

public:
    //==========================================================================
    //= Containedness
    //==========================================================================

    /// Checks if the element \c value is in the set
    bool contains(const element_type& value)const 
    { return icl::contains(*this, value); }

    /** Does <tt>*this</tt> contain <tt>sub</tt>? */
    bool contains(const set& sub)const 
    { return icl::contains(*this, sub); }

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool within(const set& super)const 
    { return icl::within(*this, super); }

    /** <tt>*this</tt> and <tt>x2</tt> are disjoint, if their intersection is empty */
    bool disjoint(const set& x2)const 
    { return icl::disjoint(*this, x2); }

    //==========================================================================
    //= Size
    //==========================================================================
    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    std::size_t iterative_size()const { return base_type::size(); }

    size_t cardinality()const { return base_type::size(); }

    //==========================================================================
    //= Addition, subtraction
    //==========================================================================
    /** Add an \c element to the set. */
    set& add(const element_type& element) 
    { return icl::insert(*this, element); } 

    /** Add an element \c element after \c prior to the set. */
    iterator add(iterator prior, const element_type& element) 
    { return icl::insert(*this, prior, element); } 

    /** Subtract an \c element from the set. */
    set& subtract(const element_type& element)
    { return icl::subtract(*this, element); } 

    //==========================================================================
    //= Symmetric difference
    //==========================================================================
    /** If \c *this set contains \c element it is erased, otherwise it is added. */
    set& flip(const element_type& element)
    { return icl::flip(*this, element); }
};



//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class Type>
struct is_set<icl::set<Type> >
{ 
    typedef is_set<icl::set<Type> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};


template <class Type>
struct type_to_string<icl::set<Type> >
{
    static std::string apply()
    { return "set<"+ type_to_string<Type>::apply() +">"; }
};

}} // namespace icl boost

#endif // BOOST_ICL_XT_SET_HPP_JOFA_070519


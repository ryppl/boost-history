/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __split_interval_set_JOFA_990223__
#define __split_interval_set_JOFA_990223__

#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set.hpp>

namespace boost{namespace itl
{

/** \brief implements a set as a set of intervals - on insertion 
    overlapping intervals are split */
template 
<
    typename                  DomainT, 
    ITL_COMPARE               Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_INTERVAL(ITL_COMPARE) Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, DomainT, Compare),
    ITL_ALLOC                 Alloc    = std::allocator
> 
class split_interval_set: 
    public interval_base_set<split_interval_set<DomainT,Compare,Interval,Alloc>, 
                             DomainT,Compare,Interval,Alloc>
{
public:
    typedef split_interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef interval_base_set<type,DomainT,Compare,Interval,Alloc> base_type;

    typedef interval_set<DomainT,Compare,Interval,Alloc> joint_type;
    typedef type overloadable_type;
    typedef type key_object_type;

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

    /// Comparison functor for domain values
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less_than<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less_than<interval_type> key_compare;

    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

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

    enum { fineness = 3 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    split_interval_set(): base_type() {}

    /// Copy constructor
    split_interval_set(const split_interval_set& src): base_type(src) {}

    /// Copy constructor for base_type
    template<class SubType>
    split_interval_set
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { assign(src); }

    /// Constructor for a single element
    explicit split_interval_set(const interval_type& elem): base_type() { add(elem); }
    /// Constructor for a single interval
    explicit split_interval_set(const domain_type& itv): base_type() { add(itv); }

    /// Assignment operator
    template<class SubType>
    split_interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        this->clear();
        this->_set.insert(src.begin(), src.end());
    }
    
} ;


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "sp_itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};


}} // namespace itl boost

#endif




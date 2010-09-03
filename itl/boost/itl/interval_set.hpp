/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_SET_HPP_JOFA_990223
#define BOOST_ITL_INTERVAL_SET_HPP_JOFA_990223

#include <boost/assert.hpp>
#include <boost/itl/rightopen_interval.hpp> //JODO REV?
#include <boost/itl/type_traits/is_interval_joiner.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/functions/associative_interval_container.hpp>
#include <boost/itl/functions.hpp>

namespace boost{namespace itl
{

/** \brief Implements a set as a set of intervals - merging adjoining intervals */
template 
<
    typename    DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_INTERVAL(ITL_COMPARE) Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, DomainT, Compare),
    ITL_ALLOC   Alloc    = std::allocator
> 
class interval_set: 
    public interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                             DomainT,Compare,Interval,Alloc>
{
public:

    typedef interval_set<DomainT,Compare,Interval,Alloc> type;

    /// The base_type of this class
    typedef interval_base_set<type,DomainT,Compare,Interval,Alloc> base_type;

    typedef type overloadable_type;

    typedef type joint_type;

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

    enum { fineness = 1 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    interval_set(): base_type() {}
    /// Copy constructor
    interval_set(const interval_set& src): base_type(src) {}

    /// Copy constructor for base_type
    template<class SubType>
    explicit interval_set
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
    }

    /// Constructor for a single element
    explicit interval_set(const domain_type& value): base_type() 
    { add(interval_type(value)); }
    /// Constructor for a single interval
    explicit interval_set(const interval_type& itv): base_type() 
    { 
        add(itv); 
    }

    /// Assignment operator
    template<class SubType>
    interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { 
        assign(src); 
        return *this; 
    }


    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> base_set_type;
        this->clear();
        // Has to be implemented via add. there might be touching borders to be joined
        iterator prior_ = this->_set.end();
        ITL_const_FORALL(typename base_set_type, it_, src) 
            prior_ = this->add(prior_, *it_);
    }

    //std::pair<iterator,bool> _insert(const value_type& value){ return this->_set.insert(value); }
    //iterator _insert(iterator prior, const value_type& value){ return this->_set.insert(prior, value); }

private:
    friend class 
        interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                                       DomainT,Compare,Interval,Alloc>;

    /// Does the set contain the interval  <tt>sub</tt>?
    bool contains_(const interval_type& sub)const;

    /// Insertion of an interval <tt>addend</tt>
    void add_(const value_type& addend);

    iterator add_(iterator prior_, const value_type& addend);

    /// Removal of an interval <tt>minuend</tt>
    void subtract_(const value_type& minuend);

} ;


template<class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    detail::joining_add(*this, addend);
}


template<class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
typename interval_set<DomainT,Compare,Interval,Alloc>::iterator 
    interval_set<DomainT,Compare,Interval,Alloc>::add_(iterator prior_, const value_type& addend)
{
    return detail::joining_add(*this, prior_, addend);
}



template<class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
{
    detail::subtract(*this, minuend);
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_joiner<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_joiner<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};


//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};

}} // namespace itl boost

#endif



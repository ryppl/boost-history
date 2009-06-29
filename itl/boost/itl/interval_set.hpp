/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_interval_set_hpp_JOFA_990223__
#define __itl_interval_set_hpp_JOFA_990223__

#include <boost/assert.hpp>
#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_sets.hpp>

namespace boost{namespace itl
{

/** Implements a set as a set of intervals - merging adjoining intervals */
template 
<
    typename    DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
class interval_set: 
    public interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                             DomainT,Compare,Interval,Alloc>
{
public:

    /// The base_type of this class
    typedef interval_base_set<itl::interval_set<DomainT,Compare,Interval,Alloc>,
                              DomainT,Compare,Interval,Alloc> base_type;

    typedef interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef type overloadable_type;

    typedef type joint_type;

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
    { assign(src); }

    /// Constructor for a single element
    explicit interval_set(const domain_type& value): base_type() 
    { add(interval_type(value)); }
    /// Constructor for a single interval
    explicit interval_set(const interval_type& itv): base_type() 
    { add(itv); }

    /// Assignment operator
    template<class SubType>
    interval_set& operator =
        (const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    { assign(src); return *this; }


    /// Assignment from a base interval_set.
    template<class SubType>
    void assign(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& src)
    {
        typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> base_set_type;
        this->clear();
        // Has to be implemented via add. there might be touching borders to be joined
        const_FORALL(typename base_set_type, it, src) 
            this->add(*it);
    }

private:
    friend class 
        interval_base_set<interval_set<DomainT,Compare,Interval,Alloc>,
                                       DomainT,Compare,Interval,Alloc>;

    /// Does the set contain the interval  <tt>x</tt>?
    bool contains_(const interval_type& x)const;

    /// Insertion of an interval <tt>x</tt>
    void add_(const value_type& x);

    /// Removal of an interval <tt>x</tt>
    void subtract_(const value_type& x);

private:
    /// Treatment of adjoint intervals on insertion
    void handle_neighbours(const iterator& it);

    iterator join_segments(const iterator& left_it, const iterator& right_it);
} ;




template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
bool interval_set<DomainT,Compare,Interval,Alloc>::contains_(const interval_type& x)const
{ 
    // Emptiness is contained in everything
    if(x.empty()) 
        return true;
    else if (this->empty())
        return false;
    else if(x.upper() < this->lower())
        return false;
    else if(this->upper() < x.lower())
        return false;
    {
        typename ImplSetT::const_iterator it = this->_set.find(x);
        if(it == this->_set.end())
            return false;
        else
            return x.contained_in(*it);
    }
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::handle_neighbours(const iterator& it)
{
    if(it == this->_set.begin())
    {
        iterator it_nxt=it; it_nxt++;
        if(it_nxt!=this->_set.end() && (*it).touches(*it_nxt)) 
            join_segments(it, it_nxt);
    }
    else
    {
        // there is a predecessor
        iterator it_pred = it; it_pred-- ;

        if((*it_pred).touches(*it)) 
        {
            iterator it_extended = join_segments(it_pred, it);

            iterator it_succ=it_extended; it_succ++;
            if(it_succ!=this->_set.end())
            {
                // it's a non border element that might have two touching neighbours
                if((*it_extended).touches(*it_succ)) 
                    join_segments(it_extended, it_succ);
            }
        }
        else
        {
            iterator it_succ=it; it_succ++;
            if(it_succ!=this->_set.end())
            {
                // it's a non border element that might have a right touching neighbours
                if((*it).touches(*it_succ)) 
                    join_segments(it, it_succ);
            }
        }
    }
}



template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline typename interval_set<DomainT,Compare,Interval,Alloc>::iterator 
    interval_set<DomainT,Compare,Interval,Alloc>
    ::join_segments(const iterator& left_it, const iterator& right_it)
{
    // both left and right are in the set and they are neighbours
    BOOST_ASSERT((*left_it).exclusive_less(*right_it));
    BOOST_ASSERT((*left_it).touches(*right_it));

    interval_type right_itv = (*right_it);
    this->_set.erase(right_it);
    const_cast<value_type&>(*left_it).extend(right_itv);

    return left_it;
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    if(addend.empty()) return;

    std::pair<iterator,bool> insertion = this->_set.insert(addend);

    if(insertion.WAS_SUCCESSFUL)
        handle_neighbours(insertion.ITERATOR);
    else
    {
        iterator fst_it = this->_set.lower_bound(addend),
                 lst_it = insertion.ITERATOR,
                 end_it = insertion.ITERATOR; end_it++;
        //BOOST_ASSERT(end_it == this->_map.upper_bound(inter_val));
        iterator snd_it = fst_it; ++snd_it;

        interval_type leftResid  = right_subtract(*fst_it, addend);
        interval_type rightResid =  left_subtract(*lst_it, addend);

        this->_set.erase(snd_it, end_it);

        interval_type extended = addend;
        extended.extend(leftResid).extend(rightResid);

        const_cast<value_type&>(*fst_it) = extended;
        handle_neighbours(fst_it);
    }
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
{
    if(minuend.empty()) return;
    iterator fst_it = this->_set.lower_bound(minuend);
    if(fst_it==this->_set.end()) return;
    iterator end_it = this->_set.upper_bound(minuend);
    iterator lst_it = end_it; --lst_it;

    interval_type leftResid = right_subtract(*fst_it, minuend);
    interval_type rightResid; 
    if(fst_it != end_it)
        rightResid = left_subtract(*lst_it, minuend);

    this->_set.erase(fst_it, end_it);

    if(!leftResid.empty())
        this->_set.insert(leftResid);

    if(!rightResid.empty())
        this->_set.insert(rightResid);
}


//-----------------------------------------------------------------------------
// equality of elements
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool is_element_equal(const interval_set<DomainT,Compare,Interval,Alloc>& lhs,
                             const interval_set<DomainT,Compare,Interval,Alloc>& rhs)
{
    // Since both are joining interval sets we can use the simpler Set::lexicographical_equal
    return &lhs == &rhs || Set::lexicographical_equal(lhs, rhs);
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::interval_set<DomainT,Compare,Interval,Alloc> > type;
    static const bool value = true; 
};


//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};

}} // namespace itl boost

#endif



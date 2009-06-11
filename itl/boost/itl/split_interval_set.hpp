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

#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set.hpp>

namespace boost{namespace itl
{

/** implements a set as a set of intervals - on insertion overlapping intervals are split */
template 
<
    typename             DomainT, 
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class,ITL_COMPARE>class Interval = itl::interval,
    ITL_ALLOC   Alloc    = std::allocator
> 
class split_interval_set: 
    public interval_base_set<split_interval_set<DomainT,Compare,Interval,Alloc>, 
                             DomainT,Compare,Interval,Alloc>
{
public:
    typedef interval_base_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc>,
                              DomainT,Compare,Interval,Alloc> base_type;

    typedef split_interval_set<DomainT,Compare,Interval,Alloc> type;
    typedef interval_set<DomainT,Compare,Interval,Alloc> joint_type;
    typedef type overloadable_type;

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
        typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> base_set_type;
        this->clear();
        // Can be implemented via _set.insert: Interval joining not necessary.
        const_FORALL(typename base_set_type, it, src) 
            this->_set.insert(*it); 
    }
    
private:
    friend class 
        interval_base_set<split_interval_set<DomainT,Compare,Interval,Alloc>,
                                             DomainT,Compare,Interval,Alloc>;

    /// Does the set contain the interval  <tt>x</tt>?
    bool contains_(const interval_type& x)const;

    /// Insertion of an interval <tt>x</tt>
    void add_(const value_type& x);

    /// Removal of an interval <tt>x</tt>
    void subtract_(const value_type& x);

private:
    /// Treatment of adjoint intervals on insertion
    void handle_neighbours(const iterator& it){}

    void insert_rest(interval_type& x_itv, iterator& it, iterator& end_it);
    void subtract_rest(const interval_type& x_itv, iterator& it, iterator& end_it);
} ;

template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
bool split_interval_set<DomainT,Compare,Interval,Alloc>::contains_(const interval_type& interv)const
{
    if(interv.empty()) 
        return true;

    type section;
    add_intersection(section, interv);
    return is_element_equal(section, type(interv));
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_set<DomainT,Compare,Interval,Alloc>::add_(const value_type& addend)
{
    if(addend.empty()) return;

    std::pair<typename ImplSetT::iterator,bool> insertion = this->_set.insert(addend);

    if(!insertion.WAS_SUCCESSFUL)
    {
        iterator fst_it = this->_set.lower_bound(addend);
        iterator end_it = this->_set.upper_bound(addend);
		iterator lst_it = end_it; lst_it--;
		iterator pre_it = fst_it;
		if(pre_it != this->_set.begin())
			--pre_it;

		// handle the beginning of the sequence of intervals of *this
		// overlapped by insertee inverval 'addend'
		interval_type leadGap = right_subtract(addend, *fst_it);
        // this is a new interval that is a gap in the current map
		if(!leadGap.empty())
			this->_set.insert(pre_it, leadGap);
		else
		{
			interval_type leftResid = right_subtract(*fst_it, addend);
			if(!leftResid.empty())
			{
				fst_it->left_subtract(leftResid);
				this->_set.insert(pre_it, leftResid);
			}
		}

		// handle the ending of the sequence of intervals of *this
		// overlapped by insertee inverval 'addend'
		interval_type endGap = left_subtract(addend, *lst_it);

		if(!endGap.empty())
			this->_set.insert(lst_it, endGap);
		else
		{
			//CL lst_it->left_subtract(rightResid, x);
			interval_type rightResid = left_subtract(*lst_it, addend);
			if(!rightResid.empty())
			{
				lst_it->right_subtract(rightResid);
				this->_set.insert(lst_it, rightResid);
			}
		}
        
        iterator snd_it = fst_it; snd_it++;
        interval_type addend_rest = left_subtract(addend, *fst_it);
        insert_rest(addend_rest, snd_it, end_it);
    }
}


template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_set<DomainT,Compare,Interval,Alloc>::insert_rest(interval_type& addend, iterator& it, iterator& end_it)
{
	interval_type left_gap, cur_itv;
	iterator pred_it = it; --pred_it;
	while(it != end_it)
	{
        cur_itv = *it;
		left_gap = right_subtract(addend, cur_itv);
		if(!left_gap.empty())
			this->_set.insert(pred_it, left_gap);

		pred_it = it;
		addend.left_subtract(cur_itv);
		++it;
	}
}


template<class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
void split_interval_set<DomainT,Compare,Interval,Alloc>::subtract_(const value_type& minuend)
{
    if(minuend.empty()) return;
    iterator fst_it = this->_set.lower_bound(minuend);
    if(fst_it==this->_set.end()) return;
    iterator end_it = this->_set.upper_bound(minuend);
    iterator snd_it = fst_it; ++snd_it;
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


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Equality on discrete interval sets. Discrete interval sets are equal, if
    their elements are equal and their fragmentation is equal.
    NOTE: This is not inline with the mathematical view.
    We have a distiction between 'element equality' and 'lexicographical 
    equality'.    */
template <typename DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
inline bool operator == (const split_interval_set<DomainT,Compare,Interval,Alloc>& lhs,
                         const split_interval_set<DomainT,Compare,Interval,Alloc>& rhs)
{
    //MEMO PORT: This implemetation worked with stlport, sgi and gnu 
    // implementations of the stl. But using MSVC-implementation
    // results in runtime error! So I had to provide an independent
    // safe implemetation.
    //return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return Set::lexicographical_equal(lhs, rhs);
}

//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_set<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    static const bool value = true; 
};

template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_set<DomainT,Compare,Interval,Alloc> > type;
    static const bool value = true; 
};

//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_set<DomainT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { return "sp_itv_set<"+ type_to_string<DomainT>::apply() +">"; }
};


}} // namespace itl boost

#endif




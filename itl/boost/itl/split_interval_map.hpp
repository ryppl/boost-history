/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SPLIT_INTERVAL_MAP_HPP_JOFA_000706
#define BOOST_ITL_SPLIT_INTERVAL_MAP_HPP_JOFA_000706

#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/interval_base_map.hpp>
#include <boost/itl/split_interval_set.hpp>

namespace boost{namespace itl
{

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif                        

/** \brief implements a map as a map of intervals - on insertion 
    overlapping intervals are split and associated values are combined. */
template
<
    typename DomainT,
    typename CodomainT,
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare  = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine  = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
    ITL_SECTION Section  = ITL_SECTION_INSTANCE(itl::inter_section, CodomainT), 
    ITL_INTERVAL(ITL_COMPARE) Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, DomainT, Compare),
    ITL_ALLOC   Alloc    = std::allocator
>
class split_interval_map:
    public interval_base_map<split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                             DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
{
public:
    typedef Traits traits;
    typedef split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> type;
    typedef       interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> joint_type;
    typedef type overloadable_type;

    typedef interval_base_map <type, 
                               DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> base_type;

    typedef DomainT domain_type;
    typedef CodomainT codomain_type;
    typedef ITL_INTERVAL_TYPE(Interval,DomainT,Compare) interval_type;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::element_type element_type;
    typedef typename base_type::segment_type segment_type;
    typedef typename base_type::domain_mapping_type    domain_mapping_type;
    typedef typename base_type::interval_mapping_type  interval_mapping_type;
    typedef typename base_type::ImplMapT ImplMapT;

    typedef typename base_type::codomain_combine codomain_combine;

    typedef interval_set<DomainT,Compare,Interval,Alloc> interval_set_type;
    typedef interval_set_type set_type;
    typedef set_type          key_object_type;

    enum { fineness = 3 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    split_interval_map(): base_type() {}
    /// Copy constructor
    split_interval_map(const split_interval_map& src): base_type(src) {}

    explicit split_interval_map(domain_mapping_type& base_pair): base_type()
    { add(base_pair); }

    explicit split_interval_map(const value_type& value_pair): base_type()
    { add(value_pair); }

    /// Assignment operator
    template<class SubType>
    split_interval_map& operator =
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Compare,Combine,Section,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_map.
    template<class SubType>
    void assign(const interval_base_map<SubType,DomainT,CodomainT,
                                        Traits,Compare,Combine,Section,Interval,Alloc>& src)
    {
        this->clear();
        this->_map.insert(src.begin(), src.end());
    }

    //==========================================================================
    //= Selection
    //==========================================================================

    //MEMO DESIGN DECISION: split_interval_map::operator[](interval): 
    // If used for selection will deliver a set of associated
    // values. It could only implemented for a single key value. But this 
    // would mislead the unexperienced user to hash a split_interval_map into 
    // singular intervals by inapt usage of op[]. So op[] will not be implemented
    // codomain_type& operator[](const interval_type& interval_of_keys)

private:
    // The following _suffixed function templates funx_ are implementations
    // correspoding unsuffixed function templates funx of the base class.
    // For funx_ want to be used in the base class via derived SubType pointer:
    // that()->funx_. So they must be either public or made accessible by a
    // friend declaration:
    friend class  
        interval_base_map
        <split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                            DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc  >;

    bool contains_(const value_type& value)const;

    template<class Combiner>
    void add_(const value_type&);

    template<class Combiner>
    iterator add_(iterator, const value_type&);

    template<class Combiner>
    void subtract_(const value_type&);

    void insert_(const value_type& value);
    iterator insert_(iterator, const value_type& value);
    void erase_(const value_type& value);

private:
    void handle_neighbours(const iterator& it_){}
    
    template<class Combiner>
    void subtract_main(const CodomainT& co_val, iterator& it_, iterator& end_  );

    void subtract_front(const interval_type& inter_val, iterator& it_);

    template<class Combiner>
    void subtract_rear(const interval_type& inter_val, const CodomainT& co_val, iterator& it_);

    void insert_range(const interval_type& inter_val, const CodomainT& co_val, iterator& it_, iterator& end_  );
    void erase_rest(const interval_type& inter_val, const CodomainT& co_val, iterator& it_, iterator& last_);
} ;

//-----------------------------------------------------------------------------
// add<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_(const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    std::pair<iterator,bool> insertion 
        = this->template _map_insert<Combiner>(inter_val, co_val);

    if(!insertion.second)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = this->_map.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert(end_ == this->_map.upper_bound(inter_val));

        iterator it_ = first_;
        interval_type rest_interval = inter_val;

        detail::add_front<type>         (*this, rest_interval,         it_);
        detail::add_main <type,Combiner>(*this, rest_interval, co_val, it_, last_);
        detail::add_rear <type,Combiner>(*this, rest_interval, co_val, it_);
    }    
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline typename split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::iterator
    split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::add_(iterator prior_, const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return prior_;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return prior_;

    std::pair<iterator,bool> insertion 
        = this->template _map_add<Combiner>(prior_, inter_val, co_val);

    if(insertion.second)
        return insertion.first;
    else
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = this->_map.equal_range(inter_val);
        iterator it_   = overlap.first,
                 last_ = overlap.second;
                 --last_;
        interval_type rest_interval = inter_val;

        detail::add_front<type>         (*this, rest_interval,         it_);
        detail::add_main <type,Combiner>(*this, rest_interval, co_val, it_, last_);
        detail::add_rear <type,Combiner>(*this, rest_interval, co_val, it_);

        return it_;
    }
}


//-----------------------------------------------------------------------------
// subtract<Combinator>(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
    template<class Combiner>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::subtract_(const value_type& minuend)
{
    interval_type inter_val = minuend.first;

    if(itl::is_empty(inter_val)) 
        return;

    const CodomainT& co_val = minuend.second;
    if(Traits::absorbs_neutrons && co_val==Combiner::neutron()) 
        return;

    iterator first_ = this->_map.lower_bound(inter_val);
    if(first_==this->_map.end()) 
        return;
    iterator end_   = this->_map.upper_bound(inter_val);
    if(first_==end_  ) 
        return;

    iterator last_  = end_; --last_;
    iterator it_    = first_;
    detail::subtract_front<type>         (*this, inter_val,         it_       );
    detail::subtract_main <type,Combiner>(*this,            co_val, it_, last_);
    detail::subtract_rear <type,Combiner>(*this, inter_val, co_val, it_       );
}

//-----------------------------------------------------------------------------
// insert(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_(const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    std::pair<iterator,bool> insertion = this->_map.insert(addend);

    if(!insertion.second)
    {
        // Detect the first and the end iterator of the collision sequence
        iterator first_ = this->_map.lower_bound(inter_val),
                 last_  = insertion.first;
        //assert((++last_) == this->_map.upper_bound(inter_val));
        iterator it_ = first_;
        detail::insert_main(*this, inter_val, co_val, it_, last_);
    }
}

template <typename DomainT, typename CodomainT, class Traits,
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline typename split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::iterator
    split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::insert_(iterator prior_, const value_type& addend)
{
    interval_type inter_val = addend.first;
    if(itl::is_empty(inter_val)) 
        return prior_;

    const CodomainT& co_val = addend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return prior_;

    std::pair<iterator,bool> insertion 
        = this->template _map_insert<codomain_combine>(prior_, inter_val, co_val);

    if(insertion.second)
        return insertion.first;
    {
        // Detect the first and the end iterator of the collision sequence
        std::pair<iterator,iterator> overlap = this->_map.equal_range(inter_val);
        iterator it_    = overlap.first,
                 last_  = overlap.second;
                 --last_;
        detail::insert_main(*this, inter_val, co_val, it_, last_);
        return it_;
    }
}

//-----------------------------------------------------------------------------
// erase(pair(interval,value)):
//-----------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_(const value_type& minuend)
{
    interval_type inter_val = minuend.first;
    if(itl::is_empty(inter_val)) 
        return;

    const CodomainT& co_val = minuend.second;
    if(Traits::absorbs_neutrons && co_val==codomain_combine::neutron()) 
        return;

    iterator first_ = this->_map.lower_bound(inter_val);
    if(first_==this->_map.end()) 
        return;
    iterator end_   = this->_map.upper_bound(inter_val);
    if(first_==end_  ) 
        return;

    iterator last_  = end_; --last_;

    iterator second_= first_; ++second_;
    if(first_ == last_) 
    {
        // only for the last there can be a right_resid: a part of *it_ right of minuend
        interval_type right_resid = left_subtract(first_->first, inter_val);

        if(first_->second == co_val)
        {
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(!itl::is_empty(left_resid))
            {
                const_cast<interval_type&>(first_->first) = left_resid;
                if(!itl::is_empty(right_resid))
                    this->_map.insert(first_, value_type(right_resid, co_val));
            }
            else if(!itl::is_empty(right_resid))
                const_cast<interval_type&>(first_->first) = right_resid;
            else
                this->_map.erase(first_);
        }
    }
    else
    {
        // first AND NOT last
        if(first_->second == co_val)
        {
            interval_type left_resid = right_subtract(first_->first, inter_val);
            if(itl::is_empty(left_resid))
                this->_map.erase(first_);
            else
                const_cast<interval_type&>(first_->first) = left_resid;
        }

        erase_rest(inter_val, co_val, second_, last_);
    }
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
inline void split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
    ::erase_rest(const interval_type& inter_val, const CodomainT& co_val, 
                 iterator& it_, iterator& last_)
{
    // For all intervals within loop: it_->first are contained_in inter_val
    while(it_ != last_)
        if((*it_).second == co_val)
            this->_map.erase(it_++); 
        else it_++;

    //erase_rear:
    if(it_->second == co_val)
    {
        interval_type right_resid = left_subtract(it_->first, inter_val);
        if(itl::is_empty(right_resid))
            this->_map.erase(it_);
        else
            const_cast<interval_type&>(it_->first) = right_resid;
    }
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_container<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_interval_splitter<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef absorbs_neutrons<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::absorbs_neutrons)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_total<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::is_total)); 
};


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    static std::string apply()
    { 
        return "sp_itv_map<"+ type_to_string<DomainT>::apply()   + ","
                            + type_to_string<CodomainT>::apply() + ","
                            + type_to_string<Traits>::apply()    +">"; 
    }
};

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

}} // namespace itl boost

#endif



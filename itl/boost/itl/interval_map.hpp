/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_MAP_HPP_JOFA_080705
#define BOOST_ITL_INTERVAL_MAP_HPP_JOFA_080705

#include <boost/assert.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_base_map.hpp>

namespace boost{namespace itl
{

template<class DomainT, class CodomainT, class Traits, 
         ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
         ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
class split_interval_map;

/** \brief implements a map as a map of intervals - on insertion 
    overlapping intervals are split and associated values are combined.*/
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
class interval_map:

    public interval_base_map<interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                             DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
{
public:
    typedef Traits traits;
    typedef       interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> type;
    typedef split_interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> split_type;
    typedef type overloadable_type;
    typedef type joint_type;
    typedef interval_base_map<type, 
                              DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> base_type;

    typedef ITL_INTERVAL_TYPE(Interval,DomainT,Compare) interval_type;
    typedef typename base_type::iterator      iterator;
    typedef typename base_type::value_type    value_type;
    typedef typename base_type::element_type  element_type;
    typedef typename base_type::segment_type  segment_type;
    typedef typename base_type::domain_type   domain_type;
    typedef typename base_type::codomain_type codomain_type;
    typedef typename base_type::domain_mapping_type domain_mapping_type;
    typedef typename base_type::interval_mapping_type interval_mapping_type;
    typedef typename base_type::ImplMapT ImplMapT;

    typedef typename base_type::codomain_combine codomain_combine;

    typedef interval_set<DomainT,Compare,Interval,Alloc> interval_set_type;
    typedef interval_set_type set_type;
    typedef set_type          key_object_type;

    enum { fineness = 1 };

public:
    //==========================================================================
    //= Construct, copy, destruct
    //==========================================================================
    /// Default constructor for the empty object
    interval_map(): base_type() {}
    /// Copy constructor
    interval_map(const interval_map& src): base_type(src) {}


    /// Copy constructor for base_type
    template<class SubType>
    explicit interval_map
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Compare,Combine,Section,Interval,Alloc>& src)
    { assign(src); }

    explicit interval_map(domain_mapping_type& base_pair): base_type()
    { add(base_pair); }

    explicit interval_map(const value_type& value_pair): base_type()
    { add(value_pair); }

    /// Assignment operator
    template<class SubType>
    interval_map& operator =
        (const interval_base_map<SubType,DomainT,CodomainT,
                                 Traits,Compare,Combine,Section,Interval,Alloc>& src)
    { assign(src); return *this; }

    /// Assignment from a base interval_map.
    template<class SubType>
    void assign(const interval_base_map<SubType,DomainT,CodomainT,
                                        Traits,Compare,Combine,Section,Interval,Alloc>& src)
    {
        typedef interval_base_map<SubType,DomainT,CodomainT,
                                  Traits,Compare,Combine,Section,Interval,Alloc> base_map_type;
        this->clear();
        // Can be implemented via _map.insert: Interval joining not necessary.
        iterator prior_ = this->_map.end();
        ITL_const_FORALL(typename base_map_type, it_, src) 
            prior_ = this->add(prior_, *it_); 
    }

private:
    // Private functions that shall be accessible by the baseclass:
    friend class
        interval_base_map <interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>, 
                                        DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>;

    iterator handle_inserted(iterator it_)
    {
        return segmental::join_neighbours(*this, it_); 
    }

    void handle_inserted(iterator prior_, iterator it_)
    {
        if(prior_ != this->_map.end() && segmental::joinable(*this, prior_, it_))
            segmental::join_on_right(*this, prior_, it_);
    }

    template<class Combiner>
    void handle_left_combined(iterator it_)
    {
        if(on_neutric<type,Combiner,Traits::absorbs_neutrons>::is_absorbable(it_->second))
            this->_map.erase(it_);
        else
            segmental::join_left(*this, it_);
    }

    template<class Combiner>
    void handle_combined(iterator it_)
    {
        if(on_neutric<type,Combiner,Traits::absorbs_neutrons>::is_absorbable(it_->second))
            this->_map.erase(it_);
        else
            segmental::join_neighbours(*this, it_);
    }

    template<class Combiner>
    void handle_preceeded_combined(iterator prior_, iterator& it_)
    {
        if(on_neutric<type,Combiner,Traits::absorbs_neutrons>::is_absorbable(it_->second))
        {
            this->_map.erase(it_);
            it_ = prior_;
        }
        else // After a new combination (e.g. combiner=max) joining neighbours may be possible
            segmental::join_neighbours(*this, it_);
    }

    template<class Combiner>
    void handle_succeeded_combined(iterator it_, iterator next_)
    {
        if(on_neutric<type,Combiner,Traits::absorbs_neutrons>::is_absorbable(it_->second))
        {
            this->_map.erase(it_);
            segmental::join_right(*this, next_);
        }
        else
        {
            segmental::join_left(*this, it_);
            segmental::join_neighbours(*this, next_);
        }
    }



    void handle_reinserted(iterator insertion_)
    { 
        segmental::join_right(*this, insertion_); 
    }


    template<class Combiner>
    void gap_insert_at(iterator& it_, iterator prior_, 
                       const interval_type& end_gap, const codomain_type& co_val)
    {
        if(on_neutric<type,Combiner,Traits::absorbs_neutrons>::is_absorbable(it_->second))
        {
            this->_map.erase(it_);
            it_ = this->template gap_insert<Combiner>(prior_, end_gap, co_val);
            segmental::join_right(*this, it_);
        }
        else 
        {
            segmental::join_left(*this, it_);
            iterator inserted_ = this->template gap_insert<Combiner>(it_, end_gap, co_val);
            it_ = segmental::join_neighbours(*this, inserted_);
        }
    }

} ;


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_map<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef is_map<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct has_inverse<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{ 
    typedef has_inverse<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (has_inverse<CodomainT>::value)); 
};


template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_interval_container<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    typedef is_interval_container<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    typedef absorbs_neutrons<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::absorbs_neutrons)); 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct is_total<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    typedef is_total<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> > type;
    BOOST_STATIC_CONSTANT(bool, value = (Traits::is_total)); 
};


//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_INTERVAL(ITL_COMPARE)  Interval, ITL_ALLOC Alloc>
struct type_to_string<itl::interval_map<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> >
{
    static std::string apply()
    { 
        return "itv_map<"+ type_to_string<DomainT>::apply()   + ","
                         + type_to_string<CodomainT>::apply() + ","
                         + type_to_string<Traits>::apply()    + ">"; 
    }
};

}} // namespace itl boost

#endif



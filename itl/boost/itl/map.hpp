/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_map_h_JOFA_070519__
#define __itl_map_h_JOFA_070519__

#include <string>
#include <boost/itl/notate.hpp>
#include <boost/itl/type_traits/is_map.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/itl/set.hpp>
#include <boost/itl/map_algo.hpp>
#include <map>


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


/// adds addition, subtraction and intersection to std::maps
template 
<
    typename DomainT, 
    typename CodomainT, 
    class Traits = itl::partial_absorber,
    ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
    ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, CodomainT),
	ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, CodomainT), 
    ITL_ALLOC   Alloc   = std::allocator 
>
class map: private std::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), 
                            Alloc<std::pair<const DomainT, CodomainT> > >
{
public:
    typedef Alloc<typename std::pair<const DomainT, CodomainT> >  allocator_type;

    typedef typename itl::map<DomainT,CodomainT,Traits, Compare,Combine,Section,Alloc> type;
    typedef typename std::map<DomainT, CodomainT, ITL_COMPARE_DOMAIN(Compare,DomainT),
                              allocator_type>              base_type;
    typedef typename itl::set<DomainT, Compare, Alloc >       set_type;

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
	typedef typename inverse<codomain_combine >::type   inverse_codomain_combine;
	typedef ITL_SECTION_CODOMAIN(Section,CodomainT)     codomain_intersect;
	typedef typename inverse<codomain_intersect>::type  inverse_codomain_intersect;
	typedef typename base_type::value_compare           value_compare;

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
    
public:
	//==========================================================================
	//= Construct, copy, destruct
	//==========================================================================
    map(){}
    map(const key_compare& comp): base_type(comp){}

    template <class InputIterator>
    map(InputIterator f, InputIterator l): base_type(f,l) {}

    template <class InputIterator>
    map(InputIterator f, InputIterator l, const key_compare& comp): base_type(f,l,comp) {}

    map(const map& src): base_type::map(src){}

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
    inline static bool has_symmetric_difference() 
	{ return is_set<codomain_type>::value || (!is_set<codomain_type>::value && !traits::is_total); }

public:
	//==========================================================================
	//= Emptieness, containment
	//==========================================================================

    /** Checks if a key element is in the map */
    bool contains(const DomainT& x)const { return !(find(x) == end()); }

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool contained_in(const map& super)const 
    { return Map::contained_in(*this, super); }

    /** Does <tt>*this</tt> contain <tt>sub</tt>? */
    bool contains(const map& sub)const 
    { return Map::contained_in(sub, *this); }

	//==========================================================================
	//= Size
	//==========================================================================
    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    size_t iterative_size()const { return size(); }

    size_t cardinality()const { return size(); }

    std::pair<iterator,bool> insert(const value_type& value_pair)
    {
        if(Traits::absorbs_neutrons && value_pair.CONT_VALUE == codomain_combine::neutron()) 
            return std::pair<iterator,bool>(end(),true);
        else
            return base_type::insert(value_pair);
    }

    /** \c add inserts \c value_pair into the map if it's key does 
        not exist in the map.    
        If \c value_pairs's key value exists in the map, it's data
        value is added to the data value already found in the map. */
    map& add(const value_type& value_pair) { return add<codomain_combine>(value_pair); }

    /** If the \c value_pair's key value is in the map, it's data value is
        subtraced from the data value stored in the map. */
    map& subtract(const value_type& value_pair)
	{
		if(Traits::is_total && has_inverse<codomain_type>::value)
			this->template add<inverse_codomain_combine>(value_pair); 
		else 
			this->template subtract<inverse_codomain_combine>(value_pair); 
    
		return *this;
	}

	void add_intersection(map& section, const domain_type& key_value)const;

	void add_intersection(map& section, const value_type& value_pair)const;

	void add_intersection(map& section, const set_type& sectant)const;

	void add_intersection(map& section, const map& sectant)const;

    //JODO 
    /** erase the value pair \c pair(key,val) from the map.
        Erase only if, the exact value content \c val is stored at key \key. */
    size_type erase(const value_type& value);


	//==========================================================================
	//= Morphisms
	//==========================================================================
	
	void absorb_neutrons()
    {
        //content_is_neutron<key_type, data_type> neutron_dropper;
        if(!Traits::absorbs_neutrons)
            erase_if(content_is_neutron<value_type>());
    }

	//==========================================================================
	//= Representaion
	//==========================================================================
	
    /** Represent this map as string */
    std::string as_string()const;

	//==========================================================================
	//= Algorithm unifiers
	//==========================================================================

    /** \c key_value allows for a uniform access to \c key_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const key_type& key_value(IteratorT& value_)
    { return (*value_).first; }

    /** \c data_value allows for a uniform access to \c data_values which is
        is used for common algorithms on sets and maps. */
    template<typename IteratorT>
    static const data_type& data_value(IteratorT& value_){ return (*value_).second; }

    /** \c key_less allows for a uniform notation of key comparison which
        is used for common algorithms on sets and maps. */
    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) 
    { return key_compare()((*lhs_).first,(*rhs_).first); }

    /** \c iterative_size() yields the number of elements that is visited
        throu complete iteration. For interval sets \c iterative_size() is
        different from \c size(). */
    static value_type make_element(const key_type& key_val, const data_type& data_val)
    { return value_type(key_val, data_val); }

    /** Erase the elements in *this map to which property \c hasProperty applies. 
    Keep all the rest. */
    template<class Predicate>
    map& erase_if(const Predicate&);

    /** Copy the elements in map \c src to which property \c hasProperty applies 
    into \c *this map. */
    template<class Predicate>
    map& assign_if(const map& src, const Predicate&);

private:
    template<class Combiner>
    map& add(const value_type& value_pair);

    template<class Combiner>
    map& subtract(const value_type& value_pair);
};


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template <class Combiner>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::add(const value_type& val)
{
    if(Traits::absorbs_neutrons && val.CONT_VALUE == Combiner::neutron())
        *this;

    std::pair<iterator, bool> insertion;
    if(Traits::is_total)
    {
        CodomainT added_val = Combiner::neutron();
        Combiner()(added_val, val.CONT_VALUE);
        insertion = insert(value_type(val.KEY_VALUE, added_val));
    }
    else // Existential case
        insertion = insert(val);

    if( insertion.WAS_SUCCESSFUL )
        return *this;
    else
    {
        iterator it = insertion.ITERATOR;
        Combiner()((*it).CONT_VALUE, val.CONT_VALUE);

        if(Traits::absorbs_neutrons && (*it).CONT_VALUE == Combiner::neutron())
            erase(it);

		return *this;
    }
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
typename map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::size_type 
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::erase(const value_type& value_pair)
{
    if(Traits::absorbs_neutrons && value_pair.CONT_VALUE == codomain_combine::neutron())
        return 0; // neutrons are never contained 'substantially' 
                  // only 'virually'.

    iterator it_ = find(value_pair.KEY_VALUE);
    if(it_ != end() && value_pair.CONT_VALUE == it_->CONT_VALUE)
    {
        erase(it_);
        return 1;
    }

    return 0;
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template <class Combiner>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
    map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::subtract(const value_type& val)
{
    iterator it_ = find(val.KEY_VALUE);
    if(it_ != end())
    {
        Combiner()((*it_).CONT_VALUE, val.CONT_VALUE);
        if(Traits::absorbs_neutrons && (*it_).CONT_VALUE == codomain_combine::neutron())
            erase(it_);
    }
    return *this;
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
	::add_intersection(map& section, const value_type& sectant)const
{
	if(Traits::is_total)
	{
		section = *this;
		section.add(sectant);
	}
	else
	{
		const_iterator it_ = find(sectant.KEY_VALUE);
		if(it_ != end())
		{
			section.add(*it_);
			if(is_set<CodomainT>::value)
				section.template add<codomain_intersect>(sectant); 
			else
				section.template add<codomain_combine>(sectant);
		}
	}
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
	::add_intersection(map& section, const domain_type& sectant)const
{
	const_iterator it_ = find(sectant);
	if(it_ != end()) 
		section.add(*it_);
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
	::add_intersection(map& section, const typename map::set_type& sectant)const
{
    typename set_type::const_iterator common_lwb_;
    typename set_type::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sectant, *this))
        return;

	typename set_type::const_iterator sec_ = common_lwb_;
	while(sec_ != common_upb_)
		add_intersection(section, *sec_++);
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
void map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
	::add_intersection(map& section, const map& sectant)const
{
    const_iterator common_lwb_;
    const_iterator common_upb_;
	if(!Set::common_range(common_lwb_, common_upb_, sectant, *this))
        return;

	const_iterator sec_ = common_lwb_;
	while(sec_ != common_upb_)
	{
		const_iterator it_ = find(sec_->KEY_VALUE);
		if(it_ != end())
		{
			section.add(*it_);
			if(is_set<CodomainT>::value)
				section.template add<codomain_intersect>(*sec_); 
			else
				section.template add<codomain_combine>(*sec_);
		}
		++sec_;
	}
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
std::string map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::as_string()const
{ 
    std::string repr;
    const_FORALL_THIS(it) {
        std::string elem("(");
        elem += to_string<DomainT>::apply((*it).KEY_VALUE);
        elem += "->";
        elem += to_string<CodomainT>::apply((*it).CONT_VALUE);
        elem += ")";

        repr += elem;
    }
    return repr;
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template<class Predicate>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::erase_if(const Predicate& pred)
{
    iterator it = begin();
    while(it != end())
        if(Predicate()(*it))
            erase(it++); 
        else ++it;
    return *this;
}


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
    template<class Predicate>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
    ::assign_if(const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& src, const Predicate& pred)
{
    clear();
    const_iterator it = src.begin();
    while(it != src.end()) {
        if(Predicate()(*it)) 
            add(*it++); 
    }
    return *this;
}
//-------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& 
    insert(map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object, 
           const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& insertee) 
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> map_type;

    const_FORALL(typename map_type, elem_, insertee) 
        object.insert(*elem_); 

    return object; 
}

template <typename DomainT, typename CodomainT, class Traits, 
          ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& 
    erase(map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object, 
          const map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& erasee) 
{
    typedef map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> map_type;

    const_FORALL(typename map_type, elem_, erasee) 
        object.erase(*elem_); 

    return object; 
}


//-----------------------------------------------------------------------------
// non member functions
//-----------------------------------------------------------------------------

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Standard equality, which is lexicographical equality of the sets
    as sequences, that are given by their Compare order. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator == (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                         const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator != (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                         const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs == rhs); }

//JODO comment... 
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool is_element_equal(const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator==((const base_type&)lhs, (const base_type&)rhs);
}

/** Protonic equality is equality on all elements that do not carry a neutron as content. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool is_protonic_equal (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
                               const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    //JODO: Efficient implementation.
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;

    itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> lhs0 = lhs;
    itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> rhs0 = rhs;
    lhs0.absorb_neutrons();
    rhs0.absorb_neutrons();
    return operator==((const base_type&)lhs0, (const base_type&)rhs0);
}

/** Strict weak less ordering which is given by the Compare order */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator < (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{
    typedef std::map<DomainT,CodomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
    return operator<((const base_type&)lhs, (const base_type&)rhs);
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator > (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return rhs < lhs; }

/** Partial ordering which is induced by Compare */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator <= (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs > rhs); }

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline bool operator >= (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& lhs,
    const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& rhs)
{ return !(lhs < rhs); }

//--------------------------------------------------------------------------
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator += (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{ return object.add(operand); }

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator + ( const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}


/** Add a map \c operand to map \c object. If an element of \c operand already exists
    in \c object, add up the contents. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator += (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ Set::add(object, operand); return object; }


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator +  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}

//--------------------------------------------------------------------------

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator |= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{ return object.add(operand); }

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator | ( const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::value_type& operand)
{
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) |= operand; 
}


/** Add a map \c operand to map \c object. If an element of \c operand already exists
    in \c object, add up the contents. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator |= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ Set::add(object, operand); return object; }


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator |  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) |= operand; 
}

//--------------------------------------------------------------------------

/** Subtract a map \c x2 from this map. If an element of \c x2 already exists
    in \c *this, subtract the contents using <tt>operator -=</tt>. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator -= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
    const_FORALL(typename ObjectT, it_, operand)
		object.subtract(*it_);

    return object; 
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator -  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}

/** Subtract a set \c x2 from this map. Every element of \c this map that
    has a key that is element of \c x2 is deleted from the map. */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator -= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ Set::erase(object, operand); return object; }


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator -  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}


/** Intersect map \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator &= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
	typedef typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> object_type;
	if(Traits::is_total) return object += operand;
	else
	{
		object_type section;
		object.add_intersection(section, operand);
		object.swap(section);
		return object;
	}
	//CL{ Map::intersect(object, operand); return object; }
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator &  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) &= operand; 
}

/** Intersect set \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator &= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ Map::intersect(object, operand); return object; } //JODO TEST 


template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator &  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
    const typename itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>::set_type& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) &= operand; 
}


/** Symmetric subtract map \c x2 and \c *this.
    So \c *this becomes the symmetric difference of \c *this and \c x2 */
template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
inline itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>&
operator ^= (      itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{
	Map::flip(object, operand); 
	return object; 
}

template <typename DomainT, typename CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>
operator ^  (const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& object,
             const itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc>& operand)
{ 
	typedef itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> ObjectT;
	return ObjectT(object) ^= operand; 
}


//-----------------------------------------------------------------------------
// type traits
//-----------------------------------------------------------------------------
template<class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
	typedef is_set<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
	static const bool value = true; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
	typedef is_map<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
	static const bool value = true; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
	typedef has_inverse<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
	static const bool value = has_inverse<CodomainT>::value; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_interval_container<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ 
	typedef is_interval_container<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> > type;
	enum{value = true}; 
};

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_interval_splitter<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ enum{value = false}; };

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct absorbs_neutrons<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ enum{value = Traits::absorbs_neutrons}; };

template <class DomainT, class CodomainT, class Traits, ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, ITL_ALLOC Alloc>
struct is_total<itl::map<DomainT,CodomainT,Traits,Compare,Combine,Section,Alloc> >
{ enum{value = Traits::is_total}; };

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

#endif // __itl_map_h_JOFA_070519__


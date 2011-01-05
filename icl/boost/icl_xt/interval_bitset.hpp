/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_XT_INTERVAL_BITSET_HPP_JOFA_091023
#define BOOST_ICL_XT_INTERVAL_BITSET_HPP_JOFA_091023

#include <iostream>                   // to organize output
#include <boost/cstdint.hpp>          // portable boost integers
#include <boost/operators.hpp>        // to define operators with minimal effort
#include <boost/icl/interval_map.hpp> // base of large bitsets
#include <boost/icl_xt/meta_log.hpp>  // a meta logarithm
#include <boost/icl_xt/bits.hpp>      // a bitset implementation
#include <boost/icl_xt/detail/bit_element_iterator.hpp>

#include <boost/icl/detail/interval_morphism.hpp> //JODO Separate aspects and concerns



namespace boost{namespace icl
{

/** \brief Large bitsets that combine interval compression and bitcompression. 

    Class interval_bitset implements a set as an interval_map of bitsets.
    An interval_bitset is selfcompressing in two ways.
    Large contiguous runs of bits can be represented by intervals in the
    same way as for icl::interval_set or icl::interval_map (interval compression).
    Individual bits that spread over narrow ranges are represented in 
    machine words as bitsets (bit compression).

    There is a datailed description on how an interval_bitset is
    implemented using icl::interval_map in the project section
    of the boost book documentation here:
    http://www.joachim-faulhaber.de/boost_itl/doc/libs/icl/doc/html/boost_itl/projects.html
*/
template 
<
    typename    DomainT = unsigned long, 
    typename    BitSetT = icl::bits<unsigned long>, 
    ICL_COMPARE Compare = ICL_COMPARE_INSTANCE(std::less, DomainT),
    ICL_INTERVAL(ICL_COMPARE) Interval = ICL_INTERVAL_INSTANCE(ICL_INTERVAL_DEFAULT, DomainT, Compare),
    ICL_ALLOC   Alloc   = std::allocator
> 
class interval_bitset
    : boost::equality_comparable < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::less_than_comparable< interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>

    , boost::addable       < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::orable        < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::subtractable  < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::andable       < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::xorable       < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>

    , boost::addable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::orable2       < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::subtractable2 < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::andable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::xorable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT

    , boost::addable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, ICL_INTERVAL_TYPE(Interval,DomainT,Compare)
    , boost::orable2       < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, ICL_INTERVAL_TYPE(Interval,DomainT,Compare)
    , boost::subtractable2 < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, ICL_INTERVAL_TYPE(Interval,DomainT,Compare)
    , boost::andable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, ICL_INTERVAL_TYPE(Interval,DomainT,Compare)
    , boost::xorable2      < interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, ICL_INTERVAL_TYPE(Interval,DomainT,Compare)
      > > > > > > > > > > > > > > > > >
    //^ & - | + ^ & - | + ^ & - | + < == 
    //segment   element   container
{
public:
    typedef boost::icl::interval_map
    <
        DomainT, BitSetT, boost::icl::partial_absorber, 
        std::less, boost::icl::inplace_bit_add, boost::icl::inplace_bit_and,
        Interval, Alloc
    > 
    interval_bitmap_type;

    typedef interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc> type;

    typedef DomainT                                       domain_type;
    typedef DomainT                                       codomain_type;
    typedef DomainT                                       element_type;   // 0 ..  2^digits(element_type)-1
    typedef DomainT                                       condensed_type; // 0 .. (2^digits(element_type)-1)/digits(word_type)
    typedef typename BitSetT::word_type                   word_type;      // 0 ..  2^digits(word_type)-1
    typedef typename BitSetT::bit_type                    bit_type;       // 0 .. digits(word_type)
    typedef short                                         narrow_type;    // small signed type that includes bit_type
    typedef BitSetT                                       bitset_type;
    typedef BitSetT                                       data_type;
    typedef typename std::size_t                          size_type;
    typedef typename interval_bitmap_type::interval_type  interval_type;
    typedef interval_type                                 segment_type;

    typedef typename interval_bitmap_type::key_type       key_type;
    typedef typename interval_bitmap_type::value_type     value_type;

    typedef typename interval_bitmap_type::domain_compare domain_compare;      
    typedef typename std::set<DomainT,domain_compare,Alloc<DomainT> > atomized_type;

    typedef typename difference_type_of<domain_type>::type difference_type;


    typedef typename interval_bitmap_type::iterator               iterator;
    typedef typename interval_bitmap_type::const_iterator         const_iterator;
    typedef typename interval_bitmap_type::reverse_iterator       reverse_iterator;
    typedef typename interval_bitmap_type::const_reverse_iterator const_reverse_iterator;

    /// element iterator: Depreciated, see documentation.
    typedef boost::icl::bit_element_iterator<iterator> element_iterator; 
    /// element const iterator: Depreciated, see documentation.
    typedef boost::icl::bit_element_iterator<const_iterator> element_const_iterator; 
    /// element reverse iterator: Depreciated, see documentation.
    typedef boost::icl::bit_element_iterator<reverse_iterator> element_reverse_iterator; 
    /// element const reverse iterator: Depreciated, see documentation.
    typedef boost::icl::bit_element_iterator<const_reverse_iterator> element_const_reverse_iterator; 

    typedef typename interval_bitmap_type::pointer         pointer;
    typedef typename interval_bitmap_type::const_pointer   const_pointer;
    typedef typename interval_bitmap_type::reference       reference;
    typedef typename interval_bitmap_type::const_reference const_reference;

public:
	interval_bitset(): _map() {}
	explicit interval_bitset(const element_type& element)   : _map() { this->add(element); } 
	explicit interval_bitset(const interval_type& inter_val): _map() { this->add(inter_val); } 

public:
    bool operator ==(const interval_bitset& rhs)const{ return _map == rhs._map; }
    bool operator < (const interval_bitset& rhs)const{ return _map <  rhs._map; }

    interval_bitset& operator +=(const interval_bitset& rhs) {_map += rhs._map; return *this;}
    interval_bitset& operator |=(const interval_bitset& rhs) {_map |= rhs._map; return *this;}
    interval_bitset& operator -=(const interval_bitset& rhs) {_map -= rhs._map; return *this;}
    interval_bitset& operator &=(const interval_bitset& rhs) {_map &= rhs._map; return *this;}
    interval_bitset& operator ^=(const interval_bitset& rhs) {_map ^= rhs._map; return *this;}

    interval_bitset& operator +=(const element_type& rhs) {return add(interval_type(rhs));      }
    interval_bitset& operator |=(const element_type& rhs) {return add(interval_type(rhs));      }
    interval_bitset& operator -=(const element_type& rhs) {return subtract(interval_type(rhs)); }
    interval_bitset& operator &=(const element_type& rhs) {return intersect(interval_type(rhs));}
    interval_bitset& operator ^=(const element_type& rhs) {return flip(interval_type(rhs));     }

    interval_bitset& operator +=(const interval_type& rhs){return add(rhs);      }
    interval_bitset& operator |=(const interval_type& rhs){return add(rhs);      }
    interval_bitset& operator -=(const interval_type& rhs){return subtract(rhs); }
    interval_bitset& operator &=(const interval_type& rhs){return intersect(rhs);}
    interval_bitset& operator ^=(const interval_type& rhs){return flip(rhs);     }

    interval_bitset& operator +=(const value_type& rhs) {_map += rhs; return *this;}

    interval_bitset& add      (const interval_type& rhs){return segment_apply(&interval_bitset::add_,      rhs);}
    interval_bitset& subtract (const interval_type& rhs){return segment_apply(&interval_bitset::subtract_, rhs);}
    interval_bitset& intersect(const interval_type& rhs){return segment_apply(&interval_bitset::intersect_,rhs);}
    interval_bitset& flip     (const interval_type& rhs){return segment_apply(&interval_bitset::flip_,     rhs);}
    interval_bitset& insert   (const interval_type& rhs){return add(rhs);}
    interval_bitset& erase    (const interval_type& rhs){return subtract(rhs);}

    interval_bitset& add      (const element_type& rhs) {return segment_apply(&interval_bitset::add_,      interval_type(rhs));}
    interval_bitset& subtract (const element_type& rhs) {return segment_apply(&interval_bitset::subtract_, interval_type(rhs));}
    interval_bitset& intersect(const element_type& rhs) {return segment_apply(&interval_bitset::intersect_,interval_type(rhs));}
    interval_bitset& flip     (const element_type& rhs) {return segment_apply(&interval_bitset::flip_,     interval_type(rhs));}
    interval_bitset& insert   (const element_type& rhs) {return add(rhs);}
    interval_bitset& erase    (const element_type& rhs) {return subtract(rhs);}

    void clear()                    { icl::clear(_map); }
    bool empty()const               { return icl::is_empty(_map); }
    void swap(interval_bitset& operand) { _map.swap(operand._map); }

    size_type cardinality()const;
    size_type size()const           { return cardinality(); }
    size_type interval_count()const { return interval_count(_map); }
    size_type iterative_size()const { return _map.iterative_size(); }

    bool contains(element_type element)const{ return _map(element>>shift).contains(element & mask); }
    bool contains(const segment_type& segment)const;
	//CL bool contains(const element_type& element)const     { return contains(segment_type(element)); };
    bool contains(const interval_bitset& sub)const      { return icl::contains(_map, sub._map); }
    bool contained_in(const interval_bitset& super)const{ return icl::within(_map, super._map); }

    void show_segments()const;
    void show_matrix(const char off_on[2] = " 1")const;
    std::string as_string()const{ return _map.as_string(); }

    //==========================================================================
    //= Iterator related
    //==========================================================================

    iterator       begin()      { return _map.begin(); }
    const_iterator begin()const { return _map.begin(); }
    iterator       end()        { return _map.end(); }
    const_iterator end()const   { return _map.end(); }

    reverse_iterator rbegin()            { return _map.rbegin(); }
    reverse_iterator rend()              { return _map.rend(); }
    const_reverse_iterator rbegin()const { return _map.rbegin(); }
    const_reverse_iterator rend()const   { return _map.rend(); }

    //==========================================================================
    //= Element iterator related
    //==========================================================================
    element_iterator elements_begin()
    {
        if(this->begin()==this->end())
            return element_iterator(this->begin(), element_iterator::beyond);
        else 
            return element_iterator(this->begin(), element_iterator::ante  );
    }

    element_iterator elements_end()  
    { return element_iterator(this->end(), element_iterator::beyond);   }

    element_const_iterator elements_begin()const
    {
        if(this->begin()==this->end())
            return element_const_iterator(this->begin(), element_iterator::beyond);
        else 
            return element_const_iterator(this->begin(), element_iterator::ante  );
    }

    element_const_iterator elements_end()const
    { return element_const_iterator(this->end(), element_iterator::beyond);   }

    element_reverse_iterator elements_rbegin()
    {
        if(this->rbegin()==this->rend())
            return element_reverse_iterator(this->rbegin(), element_iterator::before);
        else 
            return element_reverse_iterator(this->rbegin(), element_iterator::past  );
    }

    element_reverse_iterator elements_rend()  
    { return element_reverse_iterator(this->rend(), element_iterator::before); }

    element_const_reverse_iterator elements_rbegin()const
    {
        if(this->rbegin()==this->rend())
            return element_const_reverse_iterator(this->rbegin(), element_iterator::before);
        else 
            return element_const_reverse_iterator(this->rbegin(), element_iterator::past  );
    }

    element_const_reverse_iterator elements_rend()const
    { return element_const_reverse_iterator(this->rend(), element_iterator::before); }

private:                                       
    typedef typename interval_bitmap_type::segment_type seg_type;

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4245) //'initializing' : conversion from 'int' to 'const unsigned char', signed/unsigned mismatch
#endif                        

    BOOST_STATIC_CONSTANT( word_type, digits    = bitset_type::digits       );
    BOOST_STATIC_CONSTANT( word_type, divisor   = digits                    );
    BOOST_STATIC_CONSTANT( word_type, last      = digits-1                  );
    BOOST_STATIC_CONSTANT( word_type, shift     = log2_<divisor>::value     );
    BOOST_STATIC_CONSTANT( word_type, w1        = static_cast<word_type>(1) );
    BOOST_STATIC_CONSTANT( word_type, mask      = divisor - w1              );
    BOOST_STATIC_CONSTANT( word_type, all       = ~static_cast<word_type>(0));
    BOOST_STATIC_CONSTANT( word_type, top       = w1 << (digits-w1)         );

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

    typedef icl::bits<unsigned char> PartsT;
    enum { inner_part = 0, sub_part = 1, super_part = 2, inter_part = 4 };

    typedef void (interval_bitset::*segment_combiner)(element_type, element_type, bitset_type);

    static word_type from_lower_to(word_type bit){return bit==last ? all : (w1<<(bit+w1))-w1;}
    static word_type to_upper_from(word_type bit){return bit==last ? top : ~((w1<<bit)-w1); }

    interval_bitset& segment_apply(segment_combiner combine, const interval_type& operand)
    {
        if(icl::is_empty(operand))
            return *this;
                                                            // same as
        condensed_type base = icl::first(operand) >> shift, // first(operand) / divisor
                       ceil = icl::last (operand) >> shift; // last (operand) / divisor
        word_type base_rest = icl::first(operand) &  mask , // first(operand) % divisor
                  ceil_rest = icl::last (operand) &  mask ; // last (operand) % divisor  

        if(base == ceil) // [first, last] are within one bitset (chunk)
            (this->*combine)(base, base+1, bitset_type(  to_upper_from(base_rest)
                                                       & from_lower_to(ceil_rest)));
        else // [first, last] spread over more than one bitset (chunk)
        {
            element_type mid_low = base_rest == 0   ? base   : base+1, // begin of mid part 
                         mid_up  = ceil_rest == all ? ceil+1 : ceil  ; // end   of mid part

            if(base_rest > 0)    // Bitset of base interval has to be filled from base_rest to last
                (this->*combine)(base, base+1, bitset_type(to_upper_from(base_rest)));
            if(ceil_rest < all)  // Bitset of ceil interval has to be filled from first to ceil_rest
                (this->*combine)(ceil, ceil+1, bitset_type(from_lower_to(ceil_rest)));
            if(mid_low < mid_up) // For the middle part all bits have to set.
                (this->*combine)(mid_low, mid_up, bitset_type(all));
        }
        return *this;
    }

    PartsT partition(bitset_type& sub,  condensed_type& lo,
                     bitset_type& inter,
                     condensed_type& up, bitset_type& super,
                     const segment_type& segment)const;

    void       add_(DomainT lo, DomainT up, BitSetT bits){_map += value_type(interval_type::right_open(lo,up), bits);}
    void  subtract_(DomainT lo, DomainT up, BitSetT bits){_map -= value_type(interval_type::right_open(lo,up), bits);}
    void intersect_(DomainT lo, DomainT up, BitSetT bits){_map &= value_type(interval_type::right_open(lo,up), bits);}
    void      flip_(DomainT lo, DomainT up, BitSetT bits){_map ^= value_type(interval_type::right_open(lo,up), bits);}

private:
    interval_bitmap_type _map;
};



// For a given interval like e.g. [5,27] we have to split up the intervals into
// partitions
//
//    [0,1)->   [1,2)->   [2,3)->   [3,4)->   
//    [00101100][11001011][11101001][11100000]
// +       [111  11111111  11111111  1111]      [5,27] as bitset
//          a                           b
//          
// => [0,1)->   [1,3)->   [3,4)->
//    [00101111][11111111][11110000]
//
// in preparation to an operation like "+" above.
//
//    [0,1)->   [1,2)->   [2,3)->   [3,4)->   
//    [00101100][11001011][11101001][11100000]
//         [111  11111111  11111111  1111]      [5,27] as bitset
//     00000111  11111111            11110000
//         subs  ^ inters  ^         supers
//               lo=1      up=2      
template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::PartsT
interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::partition
(
    bitset_type& sub,  condensed_type& lo,
    bitset_type& inter,
    condensed_type& up, bitset_type& super,
    const segment_type& segment
)const
{
	if(icl::is_empty(segment))
        return PartsT();

    condensed_type base = icl::first(segment) >> shift, // segment.first()/ divisor
                   ceil = icl::last (segment) >> shift; // segment.last() / divisor
    word_type base_rest = icl::first(segment) &  mask , // segment.first()% divisor
              ceil_rest = icl::last (segment) &  mask ; // segment.last() % divisor  

    if(base == ceil) // [first, last] are within one bitset
    {
        inter = bitset_type(to_upper_from(base_rest) & from_lower_to(ceil_rest));
        lo = base; up = ceil;
        return PartsT(inner_part);
    }
    else // [first, last] spread over more than one bitset (chunk)
    {
        PartsT parts;
        lo = base_rest == 0    ? base   : base+1, // begin of mid part 
        up = ceil_rest == mask ? ceil+1 : ceil  ; // end   of mid part

        if(base_rest > 0)    // Bitset of base interval has to be filled from base_rest to last
        {
            sub    = bitset_type(to_upper_from(base_rest));
            parts.add(sub_part);
        }
        if(ceil_rest < mask) // Bitset of ceil interval has to be filled from first to ceil_rest
        {
            super  = bitset_type(from_lower_to(ceil_rest));
            parts += super_part;
        }
        if(lo < up) // For the middle part all bits have to be set.
        {
            inter  = bitset_type(all);
            parts |= inter_part;
        }
        return parts;
    }
}



template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::contains(const segment_type& segment)const
{ 
	if(icl::is_empty(segment))
        return true; // Emptieness is contained in everything

    condensed_type                 lo,         up        ;
    bitset_type              subs,     inters,     supers;
    PartsT parts = partition(subs, lo, inters, up, supers, segment);
    
    if(parts.contains(inner_part))
        return _map(lo).contains(inters);
    else
    {
        if(parts.contains(sub_part)   && !_map(lo-1).contains(subs))
            return false;
        if(parts.contains(super_part) && !_map(up).contains(supers))
            return false;
        if(parts.contains(inter_part) && !icl::contains(_map, seg_type(interval_type::right_open(lo,up), inters)) ) 
            return false;

        return true;
    }
}


template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::size_type 
    interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::cardinality()const    
{
    size_type cardinality = 0;
    ICL_const_FORALL(typename interval_bitmap_type, it_, _map)
        cardinality += (it_->second.cardinality() * icl::cardinality(it_->first));
    return cardinality; 
}

//JODO move ...
template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::size_type 
    cardinality(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{
    typedef typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::type object_type;
    typedef typename object_type::size_type size_type;
    size_type cardinality = 0;
    ICL_const_FORALL(typename object_type, it_, object)
        cardinality += (it_->second.cardinality() * icl::cardinality(it_->first));//JODO icl::bits
    return cardinality; 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::size_type 
    interval_count(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{
    return object.iterative_size(); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::interval_type 
hull(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{
	typedef typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::interval_type interval_type;
    return 
        icl::is_empty(object) 
            ? identity_element<interval_type>::value()
			: interval_type::closed(*object.elements_begin(), *object.elements_rbegin());
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
void interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::show_segments()const
{
    for(typename interval_bitmap_type::const_iterator it_ = _map.begin();
        it_ != _map.end(); ++it_)
    {
        interval_type   itv  = it_->first;
        bitset_type     bits = it_->second;
        std::cout << itv << "->" << bits.as_string("01") << std::endl;
    }
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
void interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::show_matrix(const char off_on[2])const
{
    typename interval_bitmap_type::const_iterator iter = _map.begin();
    while(iter != _map.end())
    {
        element_type fst = iter->first.first(), lst = iter->first.last();
        for(element_type chunk = fst; chunk <= lst; chunk++)
            std::cout << iter->second.as_string(off_on) << std::endl;
        ++iter;
    }
}

//------------------------------------------------------------------------------
template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_iterator
elements_begin(interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_begin(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_iterator
elements_end(interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_end(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_const_iterator
elements_begin(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_begin(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_const_iterator
elements_end(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_end(); }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_reverse_iterator
elements_rbegin(interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_rbegin(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_reverse_iterator
elements_rend(interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_rend(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_const_reverse_iterator
elements_rbegin(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_rbegin(); }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_const_reverse_iterator
elements_rend(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{ return object.elements_rend(); }

//------------------------------------------------------------------------------

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool is_element_equal(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& left, 
                      const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& right)
{ return left == right; }

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
void clear(interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object) 
{ 
    object.clear(); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool is_empty(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object) 
{ 
    return object.empty(); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool contains(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& super, 
           typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::element_type sub) 
{ 
    return super.contains(sub); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool contains(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& super, 
     const typename interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>::segment_type& sub  ) 
{ 
    return super.contains(sub); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool contains(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& super, 
              const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& sub  ) 
{ 
    return super.contains(sub); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool within(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& sub, 
            const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& super) 
{ 
    return super.contains(sub); 
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool intersects(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& x1, 
                const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& x2  ) 
{ 
    return !(x1 & x2).empty(); //JODO
}

template<class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc> 
bool disjoint(const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& x1, 
              const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& x2  ) 
{ 
    return !intersects(x1, x2); //JODO
}


template<class CharType, class CharTraits, 
    class DomainT, class BitSetT, 
    ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc>& object)
{
    typedef interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc> ObjectT;
    stream << "{";
    ICL_const_FORALL(typename ObjectT, it_, object)
        stream << "(" << it_->first << "->" << it_->second << ")";

    return stream << "}";
}


//-----------------------------------------------------------------------------
// type representation
//-----------------------------------------------------------------------------
template <class DomainT, class BitSetT, ICL_COMPARE Compare, ICL_INTERVAL(ICL_COMPARE) Interval, ICL_ALLOC Alloc>
struct type_to_string<icl::interval_bitset<DomainT,BitSetT,Compare,Interval,Alloc> >
{
    static std::string apply()
    { 
        return "itv_bitset<"+ type_to_string<DomainT>::apply() + ","
                            + type_to_string<BitSetT>::apply() + ">";
    }
};

namespace segmental
{
    template <typename DomainT, typename BitSetT>
    struct atomizer<std::set<DomainT>, interval_bitset<DomainT, BitSetT> >
    {
        void operator()(                      std::set<DomainT>& atomized, 
                        const interval_bitset<DomainT, BitSetT>& clustered)
        {
            typedef interval_bitset<DomainT, BitSetT> InterBitsetT;
            typename InterBitsetT::element_const_iterator bit_ = elements_begin(clustered);
            while(bit_ != elements_end(clustered))
                atomized.insert(*bit_++);
        }
    };

} // namespace segmental


}} // namespace icl boost

#endif // BOOST_ICL_XT_INTERVAL_BITSET_HPP_JOFA_091023




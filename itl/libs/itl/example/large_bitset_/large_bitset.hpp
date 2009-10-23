/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_LIBS_ITL_EXAMPLE_LARGE_BITSET__LARGE_BITSET_HPP_JOFA_091019
#define BOOST_LIBS_ITL_EXAMPLE_LARGE_BITSET__LARGE_BITSET_HPP_JOFA_091019
//[large_bitset_includes
#include <iostream>                   // to organize output
#include <boost/cstdint.hpp>          // portable long integers
#include <boost/operators.hpp>        // to define operators with minimal effort
#include "bits.hpp"                   // a minimal bitset implementation
#include "meta_log.hpp"               // a meta logarithm
#include <boost/itl/interval_map.hpp> // base of large bitsets

namespace mini // minimal implementations for example projects
{
//]

//[large_bitset_natural_typedefs
typedef unsigned int    nat; 
typedef unsigned char   nat0; // nati i: number of duplications of a byte
typedef unsigned short  nat1;
typedef unsigned long   nat2; 
typedef boost::uint64_t nat3; 

typedef bits<nat0> bits8;
typedef bits<nat1> bits16;
typedef bits<nat2> bits32;
typedef bits<nat3> bits64;
//]

//[large_bitset_class_template_header
template 
<
    typename    DomainT = nat3, 
    typename    BitSetT = mini::bits<nat3>, 
    ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
    template<class, ITL_COMPARE>class Interval = boost::itl::interval,
    ITL_ALLOC   Alloc   = std::allocator
> 
class large_bitset
    : boost::equality_comparable < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::less_than_comparable< large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>

    , boost::addable       < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::orable        < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::subtractable  < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::andable       < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>
    , boost::xorable       < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>

    , boost::addable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::orable2       < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::subtractable2 < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::andable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT
    , boost::xorable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, DomainT

    , boost::addable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, Interval<DomainT,Compare>
    , boost::orable2       < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, Interval<DomainT,Compare>
    , boost::subtractable2 < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, Interval<DomainT,Compare>
    , boost::andable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, Interval<DomainT,Compare>
    , boost::xorable2      < large_bitset<DomainT,BitSetT,Compare,Interval,Alloc>, Interval<DomainT,Compare>
      > > > > > > > > > > > > > > > > >
    //^ & - | + ^ & - | + ^ & - | + < == 
    //segment   element   container
//]
{
public:
    //[large_bitset_associated_types
    typedef boost::itl::interval_map
        <DomainT, BitSetT, boost::itl::partial_absorber, 
         std::less, boost::itl::inplace_bit_add, boost::itl::inplace_bit_and> interval_bitmap_type;

    typedef DomainT                                      domain_type;
    typedef DomainT                                      element_type;
    typedef BitSetT                                      bitset_type;
    typedef typename BitSetT::chunk_type                 chunk_type;
    typedef typename interval_bitmap_type::interval_type interval_type;
    typedef typename interval_bitmap_type::value_type    value_type;
    //]
//[large_bitset_operators
public:
    bool          operator ==(const large_bitset& rhs) { return _map == rhs._map; }
    bool          operator < (const large_bitset& rhs) { return _map <  rhs._map; }

    large_bitset& operator +=(const large_bitset& rhs) {_map += rhs._map; return *this;}
    large_bitset& operator |=(const large_bitset& rhs) {_map |= rhs._map; return *this;}
    large_bitset& operator -=(const large_bitset& rhs) {_map -= rhs._map; return *this;}
    large_bitset& operator &=(const large_bitset& rhs) {_map &= rhs._map; return *this;}
    large_bitset& operator ^=(const large_bitset& rhs) {_map ^= rhs._map; return *this;}

    large_bitset& operator +=(const element_type& rhs) {return add(interval_type(rhs, rhs));      }
    large_bitset& operator |=(const element_type& rhs) {return add(interval_type(rhs, rhs));      }
    large_bitset& operator -=(const element_type& rhs) {return subtract(interval_type(rhs, rhs)); }
    large_bitset& operator &=(const element_type& rhs) {return intersect(interval_type(rhs, rhs));}
    large_bitset& operator ^=(const element_type& rhs) {return flip(interval_type(rhs, rhs));     }

    large_bitset& operator +=(const interval_type& rhs){return add(rhs);      }
    large_bitset& operator |=(const interval_type& rhs){return add(rhs);      }
    large_bitset& operator -=(const interval_type& rhs){return subtract(rhs); }
    large_bitset& operator &=(const interval_type& rhs){return intersect(rhs);}
    large_bitset& operator ^=(const interval_type& rhs){return flip(rhs);     }
    //]
    //[large_bitset_fundamental_functions
    large_bitset& add      (const interval_type& rhs){return segment_apply(&large_bitset::add_,      rhs);}
    large_bitset& subtract (const interval_type& rhs){return segment_apply(&large_bitset::subtract_, rhs);}
    large_bitset& intersect(const interval_type& rhs){return segment_apply(&large_bitset::intersect_,rhs);}
    large_bitset& flip     (const interval_type& rhs){return segment_apply(&large_bitset::flip_,     rhs);}
    //]

    //[large_bitset_demo_functions
    size_t interval_count()const { return _map.interval_count(); }

    void show_segments()const
    {
        for(typename interval_bitmap_type::const_iterator it_ = _map.begin();
            it_ != _map.end(); ++it_)
        {
            interval_type   itv  = it_->first;
            bitset_type     bits = it_->second;
            std::cout << itv << "->" << bits.as_string("01") << std::endl;
        }
    }

    void show_matrix(const char off_on[2] = " 1")const
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
    //]

//[large_bitset_impl_constants
private:
    BOOST_STATIC_CONSTANT( chunk_type, bit_count = sizeof(chunk_type)*CHAR_BIT );
    BOOST_STATIC_CONSTANT( chunk_type, divisor   = bit_count                   );
    BOOST_STATIC_CONSTANT( chunk_type, shift     = log2_<divisor>::value       );
    BOOST_STATIC_CONSTANT( chunk_type, c1        = static_cast<chunk_type>(1)  );
    BOOST_STATIC_CONSTANT( chunk_type, mask      = divisor - c1                );
    BOOST_STATIC_CONSTANT( chunk_type, all       = ~static_cast<chunk_type>(0) );
    BOOST_STATIC_CONSTANT( chunk_type, top       = c1 << (bit_count-c1)        );
    //]
    //[large_bitset_segment_modifier
    typedef void (large_bitset::*segment_modifier)(element_type, element_type, bitset_type);
    //]

    //[large_bitset_bitset_filler
    chunk_type from_lower_to(chunk_type bit){return bit==bit_count-c1 ? all : (1<<(bit+1))-1;}
    chunk_type to_upper_from(chunk_type bit){return bit==bit_count-c1 ? top : ~((1<<bit)-1); }
    //]

    //[large_bitset_segment_apply
    large_bitset& segment_apply(segment_modifier modify, const interval_type& operand)
    {
        // Binary division: [base, ceil] == [first/divisor, last/divisor]
        element_type base       = operand.first() >> shift;
        element_type ceil       = operand.last()  >> shift;
        // Binary modulo: [base_rest, ceil_rest] == [first%divisor, last%divisor]
        element_type base_rest  = operand.first() &  mask;
        element_type ceil_rest  = operand.last()  &  mask; 

        if(base == ceil) // [first, last] are within one bitset (chunk)
        {
            //CL dbg_shift(base_rest, ceil_rest);
            (this->*modify)(base, base+1, bitset_type(  to_upper_from(base_rest)
                                                      & from_lower_to(ceil_rest)));
        }
        else // [first, last] spread over more than one bitset (chunk)
        {
            element_type lower, upper;

            if(base_rest == 0)
                lower = base;
            else
            {
                (this->*modify)(base, base+1, bitset_type(to_upper_from(base_rest)));
                lower = base+1;
            }

            if(ceil_rest == all)
                upper = ceil+1;
            else
            {
                (this->*modify)(ceil, ceil+1, bitset_type(from_lower_to(ceil_rest)));
                upper = ceil;
            }

            if(lower < upper)
                (this->*modify)(lower, upper, bitset_type(all));
        }
        return *this;
    }
    //]

    //[large_bitmap_combiners
    void add_(element_type lower, element_type upper, bitset_type bits)
    { _map += value_type(interval_type::rightopen(lower, upper), bits);    }

    void subtract_(element_type lower, element_type upper, bitset_type bits)
    { _map -= value_type(interval_type::rightopen(lower, upper), bits);    }

    void intersect_(element_type lower, element_type upper, bitset_type bits)
    { _map &= value_type(interval_type::rightopen(lower, upper), bits);    }

    void flip_(element_type lower, element_type upper, bitset_type bits)
    { _map ^= value_type(interval_type::rightopen(lower, upper), bits);    }
    //]

//[large_bitmap_impl_map
private:
    interval_bitmap_type _map;
//]
};

} // mini
#endif



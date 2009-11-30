/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_BITS_HPP_JOFA_091023
#define BOOST_ITL_XT_BITS_HPP_JOFA_091023

#include <limits>
#include <boost/operators.hpp>
#include <boost/cstdint.hpp>
#include <boost/itl_xt/meta_log.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/is_set.hpp>
#include <boost/itl/detail/relation_state.hpp>

namespace boost{namespace itl
{

// A bit_range_type is supposed to cover the range of bits including endings.
// rend()              end()
//     -1   <=  b  <=  BitsT::digits
typedef short bit_range_type;

namespace bitcount
{
    template<typename WordT, bit_range_type Digits>
    struct count
    {
        typedef WordT                   word_type;
        typedef count<word_type,Digits> type;

        static bit_range_type apply(WordT value);
    };
}


template<class NaturalT> class bits
    : equality_comparable  < bits<NaturalT>
    , less_than_comparable < bits<NaturalT>
    , incrementable        < bits<NaturalT>
    , decrementable        < bits<NaturalT>
    , addable              < bits<NaturalT>
    , orable               < bits<NaturalT>
    , subtractable         < bits<NaturalT>
    , andable              < bits<NaturalT>
    , xorable              < bits<NaturalT>

    , addable2             < bits<NaturalT>, bit_range_type
    , orable2              < bits<NaturalT>, bit_range_type
    , subtractable2        < bits<NaturalT>, bit_range_type
    , andable2             < bits<NaturalT>, bit_range_type
    , xorable2             < bits<NaturalT>, bit_range_type
      > > > > > > > > > > >  >  >  >
    //^ & - | + ^ & - | + ++ -- < ==
    //element   container
{
public:
    typedef NaturalT         word_type;
    typedef bit_range_type   bit_type;
    typedef bit_type         element_type; // pretty short: 0 <= max < digits
    typedef bit_type         size_type;
    BOOST_STATIC_CONSTANT(bit_type,  digits = std::numeric_limits<NaturalT>::digits);
    BOOST_STATIC_CONSTANT(word_type, w1     = static_cast<word_type>(1)            );

    bits():_bits(){}
    explicit bits(word_type value):_bits(value){}

    word_type word()const{ return _bits; }

    bits&       add(bit_type value){_bits |=  (1 << value); return *this; }
    bits&  subtract(bit_type value){_bits &= ~(1 << value); return *this; }
    bits& intersect(bit_type value){_bits &=  (1 << value); return *this; }
    bits&      flip(bit_type value){_bits ^=  (1 << value); return *this; }

    bits& operator += (bit_type value){ return       add(value); }
    bits& operator |= (bit_type value){ return       add(value); }
    bits& operator -= (bit_type value){ return  subtract(value); }
    bits& operator &= (bit_type value){ return intersect(value); }
    bits& operator ^= (bit_type value){ return      flip(value); }

    bits& operator += (const bits& value){_bits |= value._bits; return *this;}
    bits& operator |= (const bits& value){_bits |= value._bits; return *this;}
    bits& operator &= (const bits& value){_bits &= value._bits; return *this;}
    bits& operator ^= (const bits& value){_bits ^= value._bits; return *this;}
    bits  operator ~  ()const { return bits(~_bits); }
    bool operator  <  (const bits& value)const{return _bits < value._bits;}
    bool operator  == (const bits& value)const{return _bits == value._bits;}

    bool empty()const { return _bits == 0; }
    bool contains(element_type element)const { return 0 != ((w1 << element) & _bits); }
    bool contains(const bits& sub)const { return inclusion::subset==inclusion_compare(sub,*this); }
    size_type cardinality()const{ return bitcount::count<word_type, digits>::apply(_bits); }
    size_type size()const       { return cardinality(); }

	std::string as_string(const char off_on[2] = " 1")const;

private:
    word_type _bits;
};

typedef unsigned int    nat; 
typedef boost::uint8_t  nat8;
typedef boost::uint16_t nat16;
typedef boost::uint32_t nat32; 
typedef boost::uint64_t nat64; 

typedef bits<nat8>  bits8;
typedef bits<nat16> bits16;
typedef bits<nat32> bits32;
typedef bits<nat64> bits64;

template<class NaturalT>
int inclusion_compare(itl::bits<NaturalT> left, itl::bits<NaturalT> right)
{
    if(0 ==(left.word() & right.word())) return inclusion::unrelated;
    else if(left.word() < right.word() ) return inclusion::subset;
    else if(left.word() > right.word() ) return inclusion::superset;
    else                                 return inclusion::equal;
}

template<class NaturalT>
std::string bits<NaturalT>::as_string(const char off_on[2])const
{
    std::string sequence;
    for(int bit=0; bit < digits; bit++)
        sequence += contains(bit) ? off_on[1] : off_on[0];
    return sequence;
}


template<class CharType, class CharTraits, class NaturalT>
std::basic_ostream<CharType, CharTraits>& operator <<
(std::basic_ostream<CharType, CharTraits>& stream, const itl::bits<NaturalT>& object)
{
    return stream << object.word();
}

namespace bitcount
{

static unsigned char table[] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};


template<>
struct count<nat8, 8>
{
    typedef nat8               word_type;
    typedef count<word_type,8> type;

    static bit_range_type apply(word_type value){ return table[value];}
};

template<>
struct count<nat16, 16>
{
    typedef nat16               word_type;
    typedef count<word_type,16> type;

    static bit_range_type apply(word_type value)
    { 
        return table[ value        & 0xff]
             + table[(value >>  8) & 0xff];
    }
};

/*JODO redefinition for gcc.
template<>
struct count<nat32, 32>
{
    typedef nat32               word_type;
    typedef count<word_type,32> type;

    static bit_range_type apply(word_type value)
    { 
        return table[ value        & 0xfful]
             + table[(value >>  8) & 0xfful]
             + table[(value >> 16) & 0xfful]
             + table[(value >> 24) & 0xfful];
    }

};
*/

template<>
struct count<unsigned int, 32>
{
    typedef unsigned int            word_type;
    typedef count<word_type,32> type;

    static bit_range_type apply(word_type value)
    { 
        return table[ value        & 0xfful]
             + table[(value >>  8) & 0xfful]
             + table[(value >> 16) & 0xfful]
             + table[(value >> 24) & 0xfful];
    }

};

template<>
struct count<nat64, 64>
{
    typedef nat64               word_type;
    typedef count<word_type,64> type;

    static bit_range_type apply(word_type value)    
    { 
        return table[ value        & 0xffull]
             + table[(value >>  8) & 0xffull]
             + table[(value >> 16) & 0xffull]
             + table[(value >> 24) & 0xffull]
             + table[(value >> 32) & 0xffull]
             + table[(value >> 40) & 0xffull]
             + table[(value >> 48) & 0xffull]
             + table[(value >> 56) & 0xffull];
             ;
    }
};

} // namespace bitcount


template<class NaturalT>
struct is_set<bits<NaturalT> >
{ 
    typedef is_set<bits<NaturalT> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};



template <>struct type_to_string<itl::bits<unsigned char > >{static std::string apply(){ return "bit8";  }};
template <>struct type_to_string<itl::bits<unsigned short> >{static std::string apply(){ return "bit16"; }};
template <>struct type_to_string<itl::bits<unsigned int  > >{static std::string apply(){ return "bit32"; }};
template <>struct type_to_string<itl::bits<unsigned long > >{static std::string apply(){ return "bitl32"; }};
template <>struct type_to_string<itl::bits<unsigned long long> >{static std::string apply(){ return "bit64"; }};


}} // namespace itl boost

#endif // BOOST_ITL_XT_BITS_HPP_JOFA_091023

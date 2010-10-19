/*-----------------------------------------------------------------------------+    
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_LIBS_VALIDATE_TEST_BITS_HPP_JOFA_091009
#define BOOST_ICL_LIBS_VALIDATE_TEST_BITS_HPP_JOFA_091009

#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/is_set.hpp>
#include <boost/itl/detail/relation_state.hpp>

namespace boost{namespace icl
{

template<class NaturalT> class bits
{
public:
    typedef NaturalT    chunk_type;
    typedef NaturalT    element_type;
    typedef std::size_t size_type;

    bits():_bits(){}
    explicit bits(NaturalT value):_bits(value){}

    NaturalT number()const{ return _bits; }
    bits& operator |= (const bits& value){_bits |= value._bits; return *this;}
    bits& operator &= (const bits& value){_bits &= value._bits; return *this;}
    bits& operator ^= (const bits& value){_bits ^= value._bits; return *this;}
    bits  operator ~  ()const { return bits(~_bits); }
    bool operator  <  (const bits& value)const{return _bits < value._bits;}
    bool operator  == (const bits& value)const{return _bits == value._bits;}

    bool contains(element_type element)const{return 0 !=(_bits & element);} 
    bool contains(const bits& value)const; //JODO
    bool contained_in(const bits& value)const; //JODO

    size_type cardinality()const{}

private:
    NaturalT _bits;
};

typedef bits<unsigned char>       bits8;
typedef bits<unsigned short>      bits16;
typedef bits<unsigned long>       bits32;
typedef bits<unsigned long long>  bits64;

template<class NaturalT>
int inclusion_compare(icl::bits<NaturalT> left, icl::bits<NaturalT> right)
{
    if(0 ==(left.number() & right.number())) return inclusion::unrelated;
    else if(left.number() < right.number() ) return inclusion::subset;
    else if(left.number() > right.number() ) return inclusion::superset;
    else                                     return inclusion::equal;
}


template<class CharType, class CharTraits, class NaturalT>
std::basic_ostream<CharType, CharTraits>& operator <<
(std::basic_ostream<CharType, CharTraits>& stream, const icl::bits<NaturalT>& object)
{
    return stream << object.number();
}

template<class NaturalT>
struct is_set<bits<NaturalT> >
{ 
    typedef is_set<bits<NaturalT> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};

template <>struct type_to_string<icl::bits<unsigned char > >{static std::string apply(){ return "bit8";  }};
template <>struct type_to_string<icl::bits<unsigned short> >{static std::string apply(){ return "bit16"; }};
template <>struct type_to_string<icl::bits<unsigned int  > >{static std::string apply(){ return "bit32"; }};
template <>struct type_to_string<icl::bits<unsigned long > >{static std::string apply(){ return "bitl32"; }};
template <>struct type_to_string<icl::bits<unsigned long long> >{static std::string apply(){ return "bit64"; }};


}} // namespace icl boost

#endif // BOOST_ICL_LIBS_VALIDATE_TEST_BITS_HPP_JOFA_091009

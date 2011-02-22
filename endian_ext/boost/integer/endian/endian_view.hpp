//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-20111

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_INTEGER_ENDIAN_VIEW_HPP
#define BOOST_INTEGER_ENDIAN_VIEW_HPP

#include <boost/integer/endian/endian_pack.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace integer {
namespace endianness {

    template <typename EndianSource, typename T>
    void convert_from(T& r);
    template <typename EndianTarget, typename T>
    void convert_to(T& r);
    template <typename EndianTarget, typename EndianSource, typename T>
    void convert_to_from(T& r);
}
    template <typename Endian, typename T,
                    bool IsFundamental  = is_fundamental<T>::value,
                    bool IsSeq          = fusion::traits::is_sequence<T>::value
    >
    class endian_view;

    template <typename Endian, typename T >
    class endian_view<Endian,T,true,false>
    {
        //~ typedef typename Endian::value_type T;
        //~ template friend
    public:
        typedef T value_type;
        value_type &ref_;
        
        typedef endian_pack<Endian,value_type> endian_t;
        endian_view(value_type& ref) : ref_(ref) {};
        operator value_type() const { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            return v; 
        }
        endian_view& operator=(value_type val) { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            v=val;
            return *this; 
        }
        endian_view& operator=(endian_view const& rhs) { 
            if (this!=&rhs) {
                ref_=rhs.ref_;
            }
            return *this; 
        }
        template <typename Endian2 >
        endian_view& operator=(endian_view<Endian2,T,true,false> const& rhs) { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            endian_pack<Endian2,value_type>& val=reinterpret_cast<endian_pack<Endian2,value_type>&>(rhs.ref_);
            v=val;
            return *this; 
        }
    };

    template <typename Endian, typename T >
    class endian_view<Endian,T,false,true>
    {
    public:
        typedef T value_type;
        value_type &ref_;
        
        endian_view(value_type& ref) : ref_(ref) {};
        operator value_type() const { 
            value_type res=ref_;
            endianness::convert_from<Endian>(res);
            return res; 
        }
        endian_view& operator=(value_type val) {             
            ref_=val;
            endianness::convert_to<Endian>(ref_);
            return *this; 
        }
        endian_view& operator=(endian_view const& rhs) { 
            if (this!=&rhs) {
                ref_=rhs.ref_;
            }
            return *this; 
        }
        template <typename Endian2 >
        endian_view& operator=(endian_view<Endian2,T,true,false> const& rhs) { 
            ref_=rhs.ref_;
            endianness::convert_to_from<Endian2,Endian>(ref_);
            return *this; 
        }
    };
    
    template <typename E, typename T>    
    endian_view<E,T> as_endian(T& v) {
        return endian_view<E,T>(v);
    }
 
    template <typename T>
    endian_view<native_endian,T> as(T& v) 
    { 
        return as_endian<native_endian>(v); 
    }
    template <typename T>
    endian_view<little_endian,T> as_little(T& v)
    { 
        return as_endian<little_endian>(v); 
    }
    
    template <typename T>
    endian_view<big_endian,T> as_big(T& v)
    { 
        return as_endian<big_endian>(v); 
    }
    

} // namespace integer
} // namespace boost

#include <boost/integer/endian/endian_conversion.hpp>


#endif // BOOST_INTEGER_ENDIAN_VIEW_HPP

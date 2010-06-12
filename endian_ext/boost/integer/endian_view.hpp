//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_INTEGER_ENDIAN_VIEW_HPP
#define BOOST_INTEGER_ENDIAN_VIEW_HPP

#include <boost/integer/endian_pack.hpp>

namespace boost {

namespace integer {

    template <typename Endian>
    class endian_view {
        typedef typename Endian::value_type T;
        T &ref_;
    public:
        typedef Endian endian_t;
        endian_view(T& ref) : ref_(ref) {};
        operator T() const { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            return v; 
        }
        endian_view& operator=(T val) { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            v=val;
            return *this; 
        }
    };

    template <typename E, typename T>    
    endian_view<endian_pack<E,T> > as_endian(T& v) {
        return endian_view<endian_pack<E,T> >(v);
    }
 
    template <typename T>
    endian_view<endian_pack<native_endian,T> > as(T& v) 
    { 
        return as_endian<native_endian>(v); 
    }
    template <typename T>
    endian_view<endian_pack<little_endian,T> > as_little(T& v)
    { 
        return as_endian<little_endian>(v); 
    }
    
    template <typename T>
    endian_view<endian_pack<big_endian,T> > as_big(T& v)
    { 
        return as_endian<big_endian>(v); 
    }
    

} // namespace integer
} // namespace boost


#endif // BOOST_ENDIAN_HPP

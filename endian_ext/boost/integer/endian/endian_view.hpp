//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-20111

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//

/*!
 \file
 \brief This file provides the @c endian_view<> class template as well as some factory helper functions.
 */


#ifndef BOOST_INTEGER_ENDIAN_VIEW_HPP
#define BOOST_INTEGER_ENDIAN_VIEW_HPP

#include <boost/integer/endian/endian_pack.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/type_traits/is_fundamental.hpp>

namespace boost {
namespace integer {
#if !defined(BOOST_ENDIAN_DOXYGEN_INVOKED)
  namespace endianness {

    template <typename EndianSource, typename T>
    void convert_from(T& r);
    template <typename EndianTarget, typename T>
    void convert_to(T& r);
    template <typename EndianTarget, typename EndianSource, typename T>
    void convert_to_from(T& r);
  }
#endif
    template <typename Endian, typename T,
                    bool IsFundamental  = is_fundamental<T>::value,
                    bool IsSeq          = fusion::traits::is_sequence<T>::value
    >
#if !defined(BOOST_ENDIAN_DOXYGEN_INVOKED)
    class endian_view;
#else
    class endian_view {
      typedef T value_type;

      //! @Effects Constructs an object of type @c endian_view<E, T, n_bits, A>.
      endian_view(value_type& ref);

      //! @Returns The converted @c value_type of the referenced type as it was seen as an endian aware type.
      operator value_type() const;

      //! @Postcondition <c>value_type(this->ref_) == v</c>.
      //! @Returns @c *this.
      endian_view& operator=(value_type val);
      //! @Postcondition <c>this->ref_ == rhs.ref_</c>.
      //! @Returns @c *this.
      endian_view& operator=(endian_view const& rhs);

      //! @Postcondition <c>value_type(this->ref_) == value_type(rhs.ref_)</c>.
      //! @Returns @c*this.
      template <typename Endian2 >
      endian_view& operator=(endian_view<Endian2,T,true,false> const& rhs);
  };
#endif
#if !defined(BOOST_ENDIAN_DOXYGEN_INVOKED)

    template <typename Endian, typename T >
    class endian_view<Endian,T,true,false>
    {
        //~ typedef typename Endian::value_type T;
        //~ template friend
    public:
        typedef T value_type;
        value_type &ref_;
        
        typedef endian_pack<Endian,value_type> endian_t;

        //! @Effects Constructs an object of type @c endian_view<E, T, n_bits, A>.
        endian_view(value_type& ref) : ref_(ref) {};

        //! @Returns The converted @c value_type of the referenced type as it was seen as an endian aware type.
        operator value_type() const { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            return v; 
        }

        //! @Postcondition <c>value_type(this->ref_) == v</c>.
        //! @Returns @c *this.
        endian_view& operator=(value_type val) { 
            endian_t& v=reinterpret_cast<endian_t&>(ref_);
            v=val;
            return *this; 
        }
        //! @Postcondition <c>this->ref_ == rhs.ref_</c>.
        //! @Returns @c *this.
        endian_view& operator=(endian_view const& rhs) { 
            if (this!=&rhs) {
                ref_=rhs.ref_;
            }
            return *this; 
        }

        //! @Postcondition <c>value_type(this->ref_) == value_type(rhs.ref_)</c>.
        //! @Returns @c*this.
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
#endif
    
    //! @Returns An @c endian_view<> that depend on the endianness parameter @c E referencing the pararameter @c v.
    template <typename E, typename T>    
    endian_view<E,T> as_endian(T& v) {
        return endian_view<E,T>(v);
    }
 
    //! @Returns A native endian @c endian_view<native_endian> referencing the pararameter @c v.
    template <typename T>
    endian_view<native_endian,T> as(T& v) 
    { 
        return as_endian<native_endian>(v);
    }

    //! @Returns A little endian @c endian_view<little_endian> referencing the pararameter @c v.
    template <typename T>
    endian_view<little_endian,T> as_little(T& v)
    { 
        return as_endian<little_endian>(v); 
    }
    
    //! @Returns A little endian @c endian_view<big_endian> referencing the pararameter @c v.
    template <typename T>
    endian_view<big_endian,T> as_big(T& v)
    { 
        return as_endian<big_endian>(v); 
    }
    

} // namespace integer
} // namespace boost

#include <boost/integer/endian/endian_conversion.hpp>


#endif // BOOST_INTEGER_ENDIAN_VIEW_HPP

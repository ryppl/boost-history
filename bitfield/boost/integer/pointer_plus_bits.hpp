//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Joaquín M Lopez Munoz 2008.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTEGER_POINTER_PLUS_BITS__HPP
#define BOOST_INTEGER_POINTER_PLUS_BITS__HPP

#include <cstddef>
#include <boost/static_assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/alignment_of.hpp>

namespace boost { namespace integer {

template<int N>struct uintptr_candidates;
template<>struct uintptr_candidates<-1>{typedef unsigned int           type;};
template<>struct uintptr_candidates<0> {typedef unsigned int           type;};
template<>struct uintptr_candidates<1> {typedef unsigned short         type;};
template<>struct uintptr_candidates<2> {typedef unsigned long          type;};

#if defined(BOOST_HAS_LONG_LONG)
template<>struct uintptr_candidates<3> {typedef boost::ulong_long_type type;};
#else
template<>struct uintptr_candidates<3> {typedef unsigned int           type;};
#endif

#if defined(BOOST_HAS_MS_INT64)
template<>struct uintptr_candidates<4> {typedef unsigned __int64       type;};
#else
template<>struct uintptr_candidates<4> {typedef unsigned int           type;};
#endif

struct uintptr_aux
{
  BOOST_STATIC_CONSTANT(int,index=
    sizeof(void*)==sizeof(uintptr_candidates<0>::type)?0:
    sizeof(void*)==sizeof(uintptr_candidates<1>::type)?1:
    sizeof(void*)==sizeof(uintptr_candidates<2>::type)?2:
    sizeof(void*)==sizeof(uintptr_candidates<3>::type)?3:
    sizeof(void*)==sizeof(uintptr_candidates<4>::type)?4:-1);

  BOOST_STATIC_CONSTANT(bool,has_uintptr_type=(index>=0));

  typedef uintptr_candidates<index>::type type;
};

typedef boost::mpl::bool_<uintptr_aux::has_uintptr_type> has_uintptr_type;
typedef uintptr_aux::type                                uintptr_type;


template <std::size_t ALIGN>
struct num_low_bits_available_for_alignement {
    static const std::size_t value=0;
};

template <>
struct num_low_bits_available_for_alignement<1> {
    static const std::size_t value=0;
};

template <>
struct num_low_bits_available_for_alignement<2> {
    static const std::size_t value=1;
};

template <>
struct num_low_bits_available_for_alignement<4> {
    static const std::size_t value=2;
};

template <>
struct num_low_bits_available_for_alignement<8> {
    static const std::size_t value=3;
};

template <typename T>
struct num_low_bits_available {
};

template <typename T>
struct num_low_bits_available<T*> {
    static const std::size_t value=num_low_bits_available_for_alignement<boost::alignment_of<T>::value>::value;
};

namespace detail {

// Selects between a compressed and a regular class depending on whether
// * the user wants the compression,
// * the compilers has a unsigned integer type with the size of the the pointer and
// * the alignement of the structure let unsued the BITS

template <bool Compression, typename Compressed, typename Regular>
struct select_compressed_if_required_aux {
  typedef  Compressed    type;
};

template <typename Compressed, typename Regular>
struct select_compressed_if_required_aux<false, Compressed, Regular> {
  typedef  Regular type;
};

template <typename Pointer, typename Compressed, typename Regular, std::size_t BITS, bool Compression=true>
struct select_compressed_if_required {
    static const bool cond = Compression && has_uintptr_type::value && (num_low_bits_available<Pointer>::value>=BITS);
  typedef  typename select_compressed_if_required_aux<
    cond,
    Compressed,
    Regular
  >::type type;
};



// compressed version
template <typename Pointer, std::size_t BITS, typename Int>
class compressed_pointer_plus_bits {
    uintptr_type ui_;
    enum {
        /// pointer_bit_mask - The bits that come from the pointer.
        pointer_bit_mask =
            ~uintptr_type((uintptr_type(1) << num_low_bits_available<Pointer>::value)-1),

        /// int_shift - The number of low bits that we reserve for other uses, and
        /// keep zero.
        int_shift = num_low_bits_available<Pointer>::value-BITS,

        /// int_mask - This is the unshifted mask for valid bits of the int type.
        int_mask = uintptr_type((uintptr_type(1) << BITS)-1),

        // shifted_int_mask - This is the bits for the integer shifted in place.
        shifted_int_mask = uintptr_type(int_mask << BITS)
    };

    struct pointer_ref {
        uintptr_type& ui_;
        pointer_ref(uintptr_type& ui): ui_(ui) {}
        operator Pointer() const {
            return reinterpret_cast<Pointer>(ui_ & pointer_bit_mask);
        }

        operator Pointer() {
            return reinterpret_cast<Pointer>(ui_ & pointer_bit_mask);
        }

        pointer_ref& operator=(Pointer p) {
            uintptr_type ptr_val = reinterpret_cast<uintptr_type>(p);
            assert((ptr_val & ((1 << num_low_bits_available<Pointer>::value)-1)) == 0 &&
                    "Pointer is not sufficiently aligned");
            // Preserve all low bits, just update the pointer.
            ui_ = ptr_val | (ui_ & ~pointer_bit_mask);
            return *this;
        }
        pointer_ref& operator=(const pointer_ref& x)    {
            return operator=(x.operator Pointer());
        }
        // behaves like a pointer
        Pointer operator->() const    {
            return operator Pointer();
        }
    };

    struct small_int_ref {
        uintptr_type& ui_;
        small_int_ref(uintptr_type& ui): ui_(ui) {}

        operator Int() const {
            return (Int)((ui_ >> int_shift) & int_mask);
        }


        small_int_ref& operator=(Int i) {
            uintptr_type int_val = i;
            assert(int_val < (1 << BITS) && "Integer too large for field");

            // Preserve all bits other than the ones we are updating.
            ui_ &= ~shifted_int_mask;     // Remove integer field.
            ui_ |= int_val << int_shift;  // Set new integer.
            return *this;
        }
        small_int_ref& operator=(const small_int_ref& x)    {
            return operator=(x.operator Int());
        }
    };

public:
    compressed_pointer_plus_bits (): ui_(0){}
    compressed_pointer_plus_bits (uintptr_type ui): ui_(ui){}
    compressed_pointer_plus_bits (Pointer p, Int i): ui_(0){
        set_pointer(p);
        set_int(i);
    }

    Pointer get_pointer() const {
        //return reinterpret_cast<Pointer>(ui_ & pointer_bit_mask);
        return Pointer(ui_ & pointer_bit_mask);
    }

    Int get_int() const {
        return (Int)((ui_ >> int_shift) & int_mask);
    }

    void set_pointer(Pointer p) {
        uintptr_type ptr_val = reinterpret_cast<uintptr_type>(p);
        assert((ptr_val & ((1 << num_low_bits_available<Pointer>::value)-1)) == 0 &&
                "Pointer is not sufficiently aligned");
        // Preserve all low bits, just update the pointer.
        ui_ = ptr_val | (ui_ & ~pointer_bit_mask);
    }

    void set_int(Int i) {
        uintptr_type int_val = i;
        assert(int_val < (1 << BITS) && "Integer too large for field");

        // Preserve all bits other than the ones we are updating.
        ui_ &= ~shifted_int_mask;     // Remove integer field.
        ui_ |= int_val << int_shift;  // Set new integer.
    }

    void *get_opaque_value() const { return reinterpret_cast<void*>(ui_); }
    void set_from_opaque_value(void *val) { ui_ = reinterpret_cast<uintptr_type>(val);}

    static compressed_pointer_plus_bits get_from_opaque_value(void *p) {
        compressed_pointer_plus_bits ppb; ppb.set_from_opaque_value(p); return ppb;
    }

    pointer_ref pointer() {return pointer_ref(ui_);}
    small_int_ref small_int() {return small_int_ref(ui_);}
    Pointer pointer() const {return get_pointer();}
    Int small_int() const {return get_int(ui_);}
};

// regular version
template <typename Pointer, typename Int>
class regular_pointer_plus_bits {
    Pointer pointer_;
    Int i_;
public:
    regular_pointer_plus_bits ():pointer_(0),i_(0){}
    regular_pointer_plus_bits (Pointer p, Int b):pointer_(p),i_(b){}

    Pointer get_pointer() const {return pointer_;}
    Int get_int() const {return i_;}
    void set_pointer(Pointer p) {pointer_=p;}
    void set_int(Int i) {return i_=i;}

    Pointer& pointer() {return pointer_;}
    Pointer pointer() const {return pointer_;}
    Int& small_int() {return i_;}
    Int small_int() const     {return i_;}
};

}

// This class implements a pair of a pointer and small integer represented by the given nomber of BITS.
// It is designed to represent this in the space required by one pointer by bitmangling the integer
// into the low part of the pointer.  This can only be done for small integers: typically up to 3 bits,
// but it depends on the number of bits available according to the trait num_low_bits_available_for_alignement for the
// type.
//
// Note that pointer_plus_bits always puts the Int part in the highest bits
// possible.  For example, pointer_plus_bits<uint_64*, 1, bool> will put the bit for
// the bool into bit #2, not bit #0, which allows the low two bits to be used
// for something else.  For example, this allows:
//   pointer_plus_bits<pointer_plus_bits<uint_64*, 1, bool>, 1, bool>
// ... and the two bools will land in different bits.
//

// selects the best of the compressed or the regular version
template <typename Pointer, std::size_t BITS, typename Int, bool Compression=true>
struct pointer_plus_bits {
    typedef typename detail::select_compressed_if_required<Pointer,
            detail::compressed_pointer_plus_bits<Pointer, BITS, Int>, //num_low_bits_available<Pointer>::value>,
            detail::regular_pointer_plus_bits<Pointer, Int>,
            BITS, Compression
        >::type type;
};

template <typename Pointer, std::size_t BITS, typename Int, bool Compression>
struct num_low_bits_available<typename pointer_plus_bits<Pointer,BITS,Int,Compression>::type > {
    static const std::size_t value= num_low_bits_available<Pointer>::value - BITS;
};

template <typename Pointer, std::size_t BITS, typename Int>
struct num_low_bits_available<detail::compressed_pointer_plus_bits<Pointer,BITS,Int> > {
    static const std::size_t value= num_low_bits_available<Pointer>::value - BITS;
};

namespace detail{
template <typename T> 
struct max_bits {
    static const std::size_t value=3;
};

template <> 
struct max_bits<bool> {
    static const std::size_t value=1;
};
}

template <typename T, typename Tag, std::size_t BITS=detail::max_bits<T>::value>
struct member {
    typedef T type;
    typedef Tag tag_type;
    static const std::size_t bits=BITS;
};

template <typename Tag>
struct boolean : member<bool, Tag> {};

template <typename Tag, std::size_t BITS=detail::max_bits<Tag>::value>
struct signed_integer : member<signed int, Tag, BITS> {};
    
template <typename Tag, std::size_t BITS=detail::max_bits<Tag>::value>
struct unsigned_integer : member<unsigned int, Tag, BITS> {};

template <typename Tag, std::size_t BITS=detail::max_bits<Tag>::value>
struct signed_short : member<signed short, Tag, BITS> {};
    
template <typename Tag, std::size_t BITS=detail::max_bits<Tag>::value>
struct unsigned_short : member<unsigned short, Tag, BITS> {};
    
template <typename Pointer, bool Compression, typename Member_1, typename Member_2=mpl::void_, typename Member_3=mpl::void_>
struct pointer_plus_bitfields {
    
};

}}
#endif


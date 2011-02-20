//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Emile Cormier 2006.
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTEGER_BITFIELD_BITFIELD_HPP
#define BOOST_INTEGER_BITFIELD_BITFIELD_HPP

#include <cstddef>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <limits>

namespace boost { namespace integer {

    template <typename T>
    struct bitfield_value_type {
        typedef T type;
    };

    //-----------------------------------------------------------------------------
    template <class, std::size_t, std::size_t, class, class> class bitfield;

    namespace detail {
        //~ // allows the user to make an explicit call to the copy constructor when
        //~ // the types that are convertible conflict with one another.
        //~ struct foce_copy { };
        template <bool is_signed, typename value_type
                , typename storage_type, unsigned int WIDTH>
        struct bitfield_complete_signed;

        template <typename value_type, typename storage_type, unsigned int WIDTH>
        struct bitfield_complete_signed<true, value_type, storage_type, WIDTH> {
            static value_type convert(storage_type val, storage_type SIGN_MASK) {
                if( (val>>(WIDTH-1))!=0) {
                    return static_cast<value_type>(val | SIGN_MASK);
                } else {
                    return static_cast<value_type>(val);
                }
            }
        };

        template <typename value_type, typename storage_type, unsigned int WIDTH>
        struct bitfield_complete_signed<false, value_type, storage_type, WIDTH> {
            static value_type convert(storage_type val, storage_type) {
                return static_cast<value_type>(val);
            }
        };
        template <typename storage_type, unsigned int WIDTH>
        struct bitfield_complete_signed<false, bool, storage_type, WIDTH> {
            static bool convert(storage_type val, storage_type) {
                return val!=0;
            }
        };
    }

    //------------------------------------------------------------------------------
    /*!
        Used to easily manipulate groups of bits the same way as does C bitfields,
        but in a portable manner.
        \par Example declaration:
        \code
        struct Rgb565
        {
            typedef volatile uint16_t storage_type;
            storage_type storage;
            typedef volatile uint16_t value_type;
            BOOST_BITFIELD_DCL(storage_type, storage, unsigned char, red, 0, 4);
            BOOST_BITFIELD_DCL(storage_type, storage, unsigned char, green, 5, 10);
            BOOST_BITFIELD_DCL(storage_type, storage, unsigned char, blue, 11,15);
        };
        \endcode
        \par Example usage:
        \code
        Rgb565 r;
        r.storage= 0xffff;

        // Write to a bitfield. Note that parenthesis are needed around the bitfield so
        r.red() = 23;

        // Read from a bitfield
        Rgb565::value_type b = r.blue();

        // Access a bit within a bit field
        bool bit = r.blue()[3];
        \endcode
        \note Bitfields within the same word may overlap.
        \param STORAGE_TYPE    Unsigned integer type of the word occupied by the bitfield
        \param F    Position of the first bit (0 is the least significant bit)
        \param L    Position of the last bit (0 is the least significant bit)
        \param VALUE_TYPE    Value type of the bitfield itself
        \param REFERENCE_TYPE    Reference type of the word occupied by the bitfield
    */


    /* concept ReferenceType {
    *   typedef value_type;
    *   operator value_type();
    *   ReferenceType operator=(ReferenceType rhs);
    *   ReferenceType(ReferenceType rhs);
    * };
    *  */

    template <typename STORAGE_TYPE, std::size_t F, std::size_t L
        , typename VALUE_TYPE=typename bitfield_value_type<typename remove_const<STORAGE_TYPE>::type>::type
        , typename REFERENCE_TYPE=VALUE_TYPE&>
    class bitfield {
        typedef bitfield<STORAGE_TYPE, F, L, VALUE_TYPE, REFERENCE_TYPE> this_type;
    public:
        //! ret storage type of the bitfield support
        typedef typename remove_const<STORAGE_TYPE>::type ret_storage_type;
        //! storage type of the bitfield support
        typedef STORAGE_TYPE storage_type;
        //! Value type of the bitfield itself
        typedef VALUE_TYPE value_type;
        //! Reference type of the bitfield itself
        typedef REFERENCE_TYPE reference_type;

    private:

        // the storage type must be unsigned
        // BOOST_STATIC_ASSERT( std::numeric_limits<storage_type>::is_signed==false );
        // first and last index must be on the range corresponding to the storage_type
        BOOST_STATIC_ASSERT( F < 8*sizeof(storage_type) );
        BOOST_STATIC_ASSERT( L < 8*sizeof(storage_type) );
        BOOST_STATIC_ASSERT( F <= L );
        // The bitfield width can be stored on the value_type
        BOOST_STATIC_ASSERT( (L - F + 1) <= 8*sizeof(value_type) );

        static ret_storage_type value_to_storage(value_type val) {
            ret_storage_type res =ret_storage_type(val);
            return ret_storage_type((res & VAL_MASK) << LASTD);
        }

        static value_type storage_to_value(storage_type field) {
            storage_type val = storage_type((field & FIELD_MASK) >> LASTD);
            return detail::bitfield_complete_signed<std::numeric_limits<value_type>::is_signed, value_type, ret_storage_type, WIDTH>::convert(val,SIGN_MASK);
        }

        void set_bit(std::size_t index, bool state) {
            field_ &= ~value_to_storage(1<<index);
            if (state) field_ |= value_to_storage(1<<index);
        }

        bool get_bit(std::size_t index) {
            return (field_ & value_to_storage(1<<index)) != 0;
        }

        template <typename BITFIELD>
        class bitfield_bit_proxy {
        public:
            bitfield_bit_proxy& operator=(bitfield_bit_proxy const& rhs) {
                field_=rhs.field_;
                index_=rhs.index_;
            }
            bitfield_bit_proxy& operator=(bool state) {
                field_.set_bit(index_, state);
            }

            operator bool() const {
                return field_.get_bit(index_);
            }

        private:
            BITFIELD& field_;
            std::size_t index_;

            bitfield_bit_proxy(BITFIELD& f, std::size_t index) : field_(f), index_(index) {}

            template <class, std::size_t, std::size_t, class, class> friend class bitfield;
        };
        typedef bitfield_bit_proxy<this_type> bit_reference_type;
        typedef bitfield_bit_proxy<const this_type> bit_const_reference_type;


        //! private because a reference is nedeed
        bitfield();

    public:
        //! Useful constants
        static const std::size_t FIRST      = F;                    //!< Position of the first bit
        static const std::size_t LAST       = L;                    //!< Position of the last bit
    private:
        static const std::size_t STS        = 8*sizeof(storage_type);
    public:
        static const std::size_t LASTD      = STS-LAST-1;
        static const std::size_t WIDTH      = LAST - FIRST + 1;     //!< Width in bits of the bitfield

        static const ret_storage_type VAL_MASK;
        static const ret_storage_type FIELD_MASK;
    private:
        static const ret_storage_type SIGN_MASK;

        static const value_type  MIN_VAL    = std::numeric_limits<value_type>::is_signed?value_type((1<<(WIDTH-1))-1):0;  //!< min value that can be represented with the bitfield
        static const value_type  MAX_VAL    = std::numeric_limits<value_type>::is_signed?value_type(1<<(WIDTH-1)):(1<<(WIDTH))-1;  //!< max value that can be represented with the bitfield
    public:
        //! explicit constructor from a reference
        explicit bitfield(storage_type& field) : field_(field) {
        }

        //! setter from a value type
        void set(value_type val) {
            field_ = (field_ & ~FIELD_MASK) | value_to_storage(val);
        }
        //! Assignment from a value type
        bitfield& operator=(value_type val) {
            field_ = (field_ & ~FIELD_MASK) | value_to_storage(val);
            return *this;
        }

        //! Returns the bitfield value.
        operator value_type() const {
            return storage_to_value(field_);
        }

        value_type get() const {
   	        return storage_to_value(field_);
        }

        //! Returns the negative of the bitfield value.
        // this must be modified on the case of signed value_type
        value_type operator~() const {return storage_to_value(~field_);}

        bit_reference_type operator[](std::size_t index) {
            return bit_reference_type(*this,index);
        }

        bit_const_reference_type operator[](std::size_t index) const {
            BOOST_ASSERT(index < WIDTH);
            return bit_const_reference_type(*this, index);
        }

        //! Returns the current bitfield value as bit flags.
        /*! The returned bit flags can be ORed with other bit flags. */
        ret_storage_type flags() const {return field_ & FIELD_MASK;}

        //! Returns the given bitfield value as bit flags.
        /*! The returned bit flags can be ORed with other bit flags. */
        static ret_storage_type get_flags(value_type val) {return value_to_storage(val);}

        static std::size_t first() {return FIRST;}
        static std::size_t last() {return LAST;}
        static std::size_t width() {return WIDTH;}
        static ret_storage_type val_mask() {return VAL_MASK;}
        static ret_storage_type field_mask()  {return FIELD_MASK;}

        bitfield& operator=(bitfield const& rhs) {
            if (this == &rhs) return;
            field_ = rhs.field_;
        }

        //! Arithmetic-assign operators
        bitfield& operator++() {*this += 1; return *this;}
        value_type operator++(int) {value_type ret(*this); ++*this; return ret;}

        bitfield& operator--() {*this -= 1; return *this;}
        value_type operator--(int) {value_type ret(*this); --*this; return ret;}

        bitfield& operator+=(value_type rhs) {*this = value_type(*this) + rhs; return *this;}
        bitfield& operator-=(value_type rhs) {*this = value_type(*this) - rhs; return *this;}

        bitfield& operator*=(value_type rhs) {*this = value_type(*this) * rhs; return *this;}
        bitfield& operator/=(value_type rhs) {*this = value_type(*this) / rhs; return *this;}
        bitfield& operator%=(value_type rhs) {*this = value_type(*this) % rhs; return *this;}

        bitfield& operator<<=(int rhs) {*this = value_type(*this) << rhs; return *this;}
        bitfield& operator>>=(int rhs) {*this = value_type(*this) >> rhs; return *this;}

        bitfield& operator&=(value_type rhs) {*this = value_type(*this) & rhs; return *this;}
        bitfield& operator|=(value_type rhs) {*this = value_type(*this) | rhs; return *this;}
        bitfield& operator^=(value_type rhs) {*this = value_type(*this) ^ rhs; return *this;}

    private:
        storage_type& field_;
    };


    template <typename STORAGE_TYPE, std::size_t F, std::size_t L
        , typename VALUE_TYPE=typename bitfield_value_type<STORAGE_TYPE>::type
    >
    struct bitfield_traits {
        typedef bitfield<STORAGE_TYPE, F, L, VALUE_TYPE> reference;
        typedef bitfield<const STORAGE_TYPE, F, L, VALUE_TYPE> value;
    };

    template <typename OSTREAM, typename ST, std::size_t F, std::size_t L
        , typename VT
        , typename RT>
    OSTREAM& operator<<(OSTREAM& os, bitfield<ST, F, L, VT, RT> const& r) {
        os << r.value();
        return os;
    }
    template <typename ISTREAM, typename ST, std::size_t F, std::size_t L
        , typename VT
        , typename RT>
    ISTREAM& operator>>(ISTREAM& is, bitfield<ST, F, L, VT, RT> & r) {
        VT v;
        is>> v;
        r.set(v);
        return is;
    }

template <
    typename STORAGE_TYPE,
    std::size_t F,
    std::size_t L,
    typename VALUE_TYPE,
    typename REFERENCE_TYPE
>
const typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type
bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::VAL_MASK =
    typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type(
    (1 << bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::WIDTH) - 1);

template <
    typename STORAGE_TYPE,
    std::size_t F,
    std::size_t L,
    typename VALUE_TYPE,
    typename REFERENCE_TYPE
>
const typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type
bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::FIELD_MASK =
    typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type(
        ((1 << bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::WIDTH) - 1)
          <<
        bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::LASTD
    );

template <
    typename STORAGE_TYPE,
    std::size_t F,
    std::size_t L,
    typename VALUE_TYPE,
    typename REFERENCE_TYPE
>
const typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type
bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::SIGN_MASK =
    typename bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::ret_storage_type(~
            ((1 << bitfield<STORAGE_TYPE,F,L,VALUE_TYPE,REFERENCE_TYPE>::WIDTH) - 1)

    );


template <typename Bitfield, typename Bitfield::value_type V>
struct bitfield_value_to_storage {
    static const typename Bitfield::ret_storage_type value;
};

template <
    typename Bitfield, typename Bitfield::value_type V
>
const typename Bitfield::ret_storage_type bitfield_value_to_storage<Bitfield, V>::value = ((V & Bitfield::VAL_MASK) << Bitfield::LASTD);


}}
#endif // BOOST_INTEGER_BITFIELD_BITFIELD_HPP


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

#ifndef BOOST_INTEGER_BITFIELD_POLICY_HPP
#define BOOST_INTEGER_BITFIELD_POLICY_HPP

#include <cstddef>
#include <boost/static_assert.hpp>

#include <cassert>
#include <limits>
namespace boost { namespace integer {
    

    //-----------------------------------------------------------------------------
    template <class, std::size_t, std::size_t, class, class> class bitfield_policy;

    #if 0
    template <class R, class V>
    class bitfield_bit_proxy {
    public:
        bitfield_bit_proxy& operator=(bool state) {
            field_ &= ~mask_;
            if (state) field_ |= mask_;
            return *this;
        }

        operator bool() const {
            return (field_ & mask_) != 0;
        }

        bool operator~() const {return !(static_cast<bool>(*this));}

    private:
        R field_;
        V mask_;

        bitfield_bit_proxy(R f, V mask) : field_(f), mask_(mask) {
            std::cout << "field_=" << std::hex << field_ << std::endl;
            std::cout << "mask_=" << mask_ << std::endl;
        }

        template <class, std::size_t, std::size_t, class, class> friend class bitfield;
    };

    template <typename T> struct reference_traits;

    template <typename T> struct reference_traits<const T&> {
      typedef T value_type;
    };
    template <typename T> struct reference_traits<T&> {
      typedef T value_type;
    };

    template <typename C> struct reference_traits {
      typedef typename C::value_type value_type;
    };
    #endif

    namespace detail { namespace policy {
        template <bool is_signed, typename value_type
                , typename storage_type, unsigned int WIDTH, unsigned int SIGN_MASK>
        struct bitfield_complete_signed;

        template <typename value_type, typename storage_type, unsigned int WIDTH, unsigned int SIGN_MASK>
        struct bitfield_complete_signed<true, value_type, storage_type, WIDTH, SIGN_MASK> {
            static value_type convert(storage_type val) {
                if( (val>>(WIDTH-1))!=0) {
                    return value_type(val | SIGN_MASK);
                } else {
                    return value_type(val);
                }
            }
        };

        template <typename value_type, typename storage_type, unsigned int WIDTH, unsigned int SIGN_MASK>
        struct bitfield_complete_signed<false, value_type, storage_type, WIDTH, SIGN_MASK> {
            static value_type convert(storage_type val) {
                return value_type(val);
            }
        };
#ifdef BOOST_MSVC
        template <typename value_type, typename storage_type, unsigned int WIDTH, unsigned int SIGN_MASK>
        struct bitfield_complete_signed<false, value_type, bool, WIDTH, SIGN_MASK> {
            static value_type convert(storage_type val) {
                return value_type(val) > 0;
            }
        };
#endif
    }}
    
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

    template <typename STORAGE_TYPE, std::size_t F, std::size_t L,
        typename VALUE_TYPE = typename bitfield_value_type<STORAGE_TYPE>::type,
        typename REFERENCE_TYPE = VALUE_TYPE&>
    class bitfield_policy {
        typedef bitfield_policy<STORAGE_TYPE, F, L, VALUE_TYPE, REFERENCE_TYPE> this_type;
    public:
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
        // BOOST_STATIC_ASSERT( F < 8*sizeof(storage_type) );
        // BOOST_STATIC_ASSERT( L < 8*sizeof(storage_type) );
        // BOOST_STATIC_ASSERT( F <= L );
        // The bitfield width can be stored on the value_type
        // BOOST_STATIC_ASSERT( (L - F + 1) <= 8*sizeof(value_type) );

        static storage_type value_to_storage(value_type val) {
            storage_type res =val;
            return ((res & VAL_MASK) << LASTD);
        }

        static value_type storage_to_value(storage_type field) {
            storage_type val = (field & FIELD_MASK) >> LASTD;
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4309)
#endif
            return detail::policy::bitfield_complete_signed<
                std::numeric_limits<value_type>::is_signed,
                value_type,
                storage_type,
                WIDTH,
                SIGN_MASK
            >::convert(val);
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
        }

        //! private because a reference is nedeed
        bitfield_policy();

    public:


        //! Useful constants
        static const std::size_t FIRST      = F;                    //!< Position of the first bit
        static const std::size_t LAST       = L;                    //!< Position of the last bit
        static const std::size_t STS        = 8 * sizeof(storage_type);
        static const std::size_t LASTD      = STS-LAST-1;
        static const std::size_t WIDTH      = LAST - FIRST + 1;     //!< Width in bits of the bitfield
        static const storage_type VAL_MASK   = (STORAGE_TYPE(1) << WIDTH) - 1;    //!< Mask applied against assigned values
        static const storage_type FIELD_MASK = (STORAGE_TYPE(VAL_MASK) << LASTD); //!< Mask of the field's bit positions
        static const storage_type SIGN_MASK  = storage_type(~VAL_MASK);            //!< Sign mask applied against assigned values


        //! explicit constructor from a reference
        explicit bitfield_policy(storage_type& field)
            :field_(field)
        { }

        //! setter from a value type
        void set(value_type val) {
            field_ = (field_ & ~FIELD_MASK) | value_to_storage(val);
        }


        //! Returns the bitfield value.
        value_type get() const {
   	        return storage_to_value(field_);
        }

    private:
        storage_type& field_;
    };
}}
#endif


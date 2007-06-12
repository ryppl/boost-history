//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ATOMIC_DETAIL_ACTOMIC_OPERATORS_HPP
#define BOOST_ACT_ATOMIC_DETAIL_ACTOMIC_OPERATORS_HPP

// ToDo: Move this to the openmp file and only activate is openmp is supported

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>

#include "../../type_traits/is_atomic.hpp"

#include <boost/typeof/typeof.hpp>

// ToDo: Account for const qualification (disable during assignment)

#include <boost/config.hpp>

#ifdef BOOST_MSVC

#define BOOST_ACT_DETAIL_TYPEOF_TYPEDEF( expression )                          \
private:                                                                       \
  BOOST_TYPEOF_NESTED_TYPEDEF_TPL( impl, expression );                         \
public:                                                                        \
  typedef typename impl::type type;

#else

#define BOOST_ACT_DETAIL_TYPEOF_TYPEDEF( expression )                          \
  typedef BOOST_TYPEOF_TPL( expression ) type;

#endif

namespace boost
{
namespace act
{
namespace detail
{
template< typename Type, typename Enabler = void >
struct atomic_value_type
{
  typedef Type type;
};

// ToDo: Use enable_if to account for cv qualification
template< typename Type >
struct atomic_value_type< Type
                        , typename enable_if< is_atomic< Type > >::type
                        >
{
  typedef typename give_cv_to< Type, typename Type::value_type >::type type;
};

template< typename Type0, typename Type1 >
struct either_is_atomic
  : mpl::or_< is_atomic< Type0 >, is_atomic< Type1 > >
{
};

template< typename Type >
struct is_arithmetic_or_enum
  : mpl::or_< is_arithmetic< Type >, is_enum< Type > >
{
};

template< typename Type >
struct is_integral_or_enum
  : mpl::or_< is_integral< Type >, is_enum< Type > >
{
};


template< typename Type >
struct derferenced_type_is_object
  : is_object< typename remove_pointer< Type >::type >
{
};

template< typename Type >
struct is_object_pointer
  : mpl::and_< is_pointer< Type >
             , derferenced_type_is_object< Type >
             >
{
};

template< typename Type0, typename Type1 >
struct dereferenced_unqualified_types_are_same
  : is_same< typename remove_cv
             <
               typename remove_pointer< Type0 >::type
             >
             ::type
           , typename remove_cv
             <
               typename remove_pointer< Type1 >::type
             >
             ::type
           >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_add_assign_operands_impl
  : mpl::or_
   <
     mpl::and_< is_arithmetic_or_enum< LeftOperand >
              , is_arithmetic_or_enum< RightOperand >
              >
   , mpl::and_< is_object_pointer< LeftOperand >
              , is_integral_or_enum< RightOperand >
              >
   >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_add_assign_operands
  : mpl::and_
    <
      either_is_atomic< LeftOperand, RightOperand >
    , are_valid_fundamental_add_assign_operands_impl
      < typename atomic_value_type< LeftOperand >::type
      , typename atomic_value_type< RightOperand >::type
      >
    >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_subtract_assign_operands_impl
  : mpl::or_
    <
      mpl::and_< is_arithmetic_or_enum< LeftOperand >
               , is_arithmetic_or_enum< RightOperand >
               >
    , mpl::and_< is_object_pointer< LeftOperand >
               , is_object_pointer< RightOperand >
               , dereferenced_unqualified_types_are_same< LeftOperand
                                                        , RightOperand
                                                        >
               >
    , mpl::and_< is_object_pointer< LeftOperand >
               , is_integral_or_enum< RightOperand >
               >
    >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_subtract_assign_operands
  : mpl::and_
    <
      either_is_atomic< LeftOperand, RightOperand >
    , are_valid_fundamental_subtract_assign_operands_impl
      < typename atomic_value_type< LeftOperand >::type
      , typename atomic_value_type< RightOperand >::type
      >
    >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_addition_operands
  : mpl::or_< are_valid_fundamental_add_assign_operands< LeftOperand
                                                       , RightOperand
                                                       >
            , are_valid_fundamental_add_assign_operands< RightOperand
                                                       , LeftOperand
                                                       >
            >
{
};

template< typename LeftOperand, typename RightOperand >
struct are_valid_fundamental_subtraction_operands
  : mpl::or_< are_valid_fundamental_subtract_assign_operands< LeftOperand
                                                            , RightOperand
                                                            >
            , are_valid_fundamental_subtract_assign_operands< RightOperand
                                                            , LeftOperand
                                                            >
            >
{
};

template< typename Left, typename Right >
struct lazy_type_of_add
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   + typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_subtract
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   - typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_multiply
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   * typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_divide
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   / typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_mod
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   % typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_and
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   & typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_or
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   | typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_xor
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                   ^ typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_left_shift
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                  << typename atomic_value_type< Right >::type()
                                 );
};

template< typename Left, typename Right >
struct lazy_type_of_right_shift
{
  BOOST_ACT_DETAIL_TYPEOF_TYPEDEF(   typename atomic_value_type< Left >::type()
                                  >> typename atomic_value_type< Right >::type()
                                 );
};

template< typename Type >
struct atomic_is_arithmetic
  : is_arithmetic< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_integral
  : is_integral< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_scalar
  : is_scalar< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_pointer
  : is_pointer< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_object_pointer
  : is_object_pointer< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_enum
  : is_enum< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_floating_point
  : is_floating_point< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_integral_or_enum
  : is_integral_or_enum< typename atomic_value_type< Type >::type >
{
};

template< typename Type >
struct atomic_is_arithmetic_or_enum
  : is_arithmetic_or_enum< typename atomic_value_type< Type >::type >
{
};

// ToDo: Make more strict condition
template< typename Type0, typename Type1 >
struct atomic_are_valid_relational_operands
  : mpl::and_< either_is_atomic< Type0, Type1 >
             , mpl::or_< atomic_is_pointer< Type0 >
                       , atomic_is_arithmetic< Type0 >
                       , atomic_is_enum< Type0 >
                       >
             , mpl::or_< atomic_is_pointer< Type1 >
                       , atomic_is_arithmetic< Type1 >
                       , atomic_is_enum< Type1 >
                       >
             >
{
};

template< typename Type >
Type& detail_raw_value( Type& target )
{
  return target;
}

////////////////////////
// Additive operators //
////////////////////////

// ToDo: Only use omp atomic if left operand is atomic

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  detail::are_valid_fundamental_addition_operands< LeftType, RightType >
, detail::lazy_type_of_add< LeftType, RightType >
>
::type
operator +( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) + detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::are_valid_fundamental_add_assign_operands< LeftType, RightType >
, LeftType&
>
::type
operator +=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) += detail_raw_value( right );
  return left;
}

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  detail::are_valid_fundamental_subtraction_operands< LeftType, RightType >
, detail::lazy_type_of_subtract< LeftType, RightType >
>
::type
operator -( LeftType& left, RightType const& right )
{
  return detail_raw_value( left ) - detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::are_valid_fundamental_subtract_assign_operands< LeftType, RightType >
, LeftType&
>
::type
operator -=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) -= detail_raw_value( right );
  return left;
}

//////////////////////////////
// Multiplicative operators //
//////////////////////////////

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::is_arithmetic_or_enum< LeftType >
           , detail::is_arithmetic_or_enum< RightType >
           >
, detail::lazy_type_of_multiply< LeftType, RightType >
>
::type
operator *( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) * detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_arithmetic_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator *=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) *= detail_raw_value( right );
  return left;
}

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::is_arithmetic_or_enum< LeftType >
           , detail::is_arithmetic_or_enum< RightType >
           >
, detail::lazy_type_of_divide< LeftType, RightType >
>
::type
operator /( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) / detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_arithmetic_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator /=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) /= detail_raw_value( right );
  return left;
}

/*
template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_mod< LeftType, RightType >
>
::type
operator %( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) % detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator %=( LeftType& left, RightType const& right )
{
  detail_raw_value( left ) %= detail_raw_value( right );
  return left;
}
*/

/////////////////////
// Shift Operators //
/////////////////////

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::either_is_atomic< LeftType, RightType >
           , detail::atomic_is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_left_shift< LeftType, RightType >
>
::type
operator <<( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) << detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator <<=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) <<= detail_raw_value( right );
  return left;
}

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::either_is_atomic< LeftType, RightType >
           , detail::is_integral_or_enum< LeftType >
           , detail::is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_right_shift< LeftType, RightType >
>
::type
operator >>( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) >> detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator >>=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) >>= detail_raw_value( right );
  return left;
}

///////////////////////
// Bitwise Operators //
///////////////////////

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::either_is_atomic< LeftType, RightType >
           , detail::is_integral_or_enum< LeftType >
           , detail::is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_and< LeftType, RightType >
>
::type
operator &( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) & detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator &=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) &= detail_raw_value( right );
  return left;
}

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::either_is_atomic< LeftType, RightType >
           , detail::is_integral_or_enum< LeftType >
           , detail::is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_or< LeftType, RightType >
>
::type
operator |( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) | detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator |=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) |= detail_raw_value( right );
  return left;
}

template< typename LeftType, typename RightType >
typename lazy_enable_if
<
  mpl::and_< detail::either_is_atomic< LeftType, RightType >
           , detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, detail::lazy_type_of_xor< LeftType, RightType >
>
::type
operator ^( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) ^ detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  mpl::and_< detail::is_integral_or_enum< LeftType >
           , detail::atomic_is_integral_or_enum< RightType >
           >
, LeftType&
>
::type
operator ^=( LeftType& left, RightType const& right )
{
  #pragma omp atomic
  detail_raw_value( left ) ^= detail_raw_value( right );
  return left;
}

//////////////////////////
// Relational Operators //
//////////////////////////

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator <( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) < detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator <=( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) <= detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator >( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) > detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator >=( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) >= detail_raw_value( right );
}

////////////////////////
// Equality Operators //
////////////////////////

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator ==( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) == detail_raw_value( right );
}

template< typename LeftType, typename RightType >
typename enable_if
<
  detail::atomic_are_valid_relational_operands< LeftType, RightType >
, bool
>
::type
operator !=( LeftType const& left, RightType const& right )
{
  return detail_raw_value( left ) != detail_raw_value( right );
}

///////////////////////////////////
// Increment/Decrement Operators //
///////////////////////////////////

template< typename Type >
typename enable_if
<
  mpl::and_
  <
    is_atomic< Type >
  , mpl::or_< detail::atomic_is_arithmetic< Type >
            , detail::atomic_is_object_pointer< Type >
            >
  >
, Type&
>
::type
operator ++( Type& target )
{
  #pragma omp atomic
  ++detail_raw_value( target );
  return target;
}

template< typename Type >
typename enable_if
<
  mpl::and_
  <
    is_atomic< Type >
  , mpl::or_< detail::atomic_is_arithmetic< Type >
            , detail::atomic_is_object_pointer< Type >
            >
  >
, Type&
>
::type
operator ++( Type& target, int )
{
  #pragma omp atomic
  detail_raw_value( target )++;
  return target;
}

template< typename Type >
typename enable_if
<
  mpl::and_
  <
    is_atomic< Type >
  , mpl::or_< detail::atomic_is_arithmetic< Type >
            , detail::atomic_is_object_pointer< Type >
            >
  >
, Type&
>
::type
operator --( Type& target )
{
  #pragma omp atomic
  --detail_raw_value( target );
  return target;
}

template< typename Type >
typename enable_if
<
  mpl::and_
  <
    is_atomic< Type >
  , mpl::or_< detail::atomic_is_arithmetic< Type >
            , detail::atomic_is_object_pointer< Type >
            >
  >
, Type&
>
::type
operator --( Type& target, int )
{
  #pragma omp atomic
  detail_raw_value( target )--;
  return target;
}

/////////////////////////
// Derference Operator //
/////////////////////////

template< typename Type >
typename lazy_enable_if
<
  mpl::and_
  <
    is_atomic< Type >
  , detail::atomic_is_pointer< Type >
  >
, remove_pointer< Type >
>
::type&
operator *( Type const& target )
{
  return *detail_raw_value( target );
}

}
}
}

#endif

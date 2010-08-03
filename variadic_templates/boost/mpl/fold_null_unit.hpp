#ifndef BOOST_MPL_FOLD_NULL_UNIT_HPP_INCLUDED
#define BOOST_MPL_FOLD_NULL_UNIT_HPP_INCLUDED
#include <boost/mpl/null_unit.hpp>

namespace boost
{
namespace mpl
{
      template
      < class Null
      , class Unit
      , template<class C>class Converter
      , typename... T
      >
    struct fold_null_unit
    /**@brief
     *  Fold of T... where each, for each T,
     *  Converter<T>::type is either Null or Unit,
     *  and where the operation, Op, obeys laws:
     *
     *    Unit Op T == T.
     *    Null Op T == Null.
     *
     *  Actually, Op is boost::mpl::null_unit<Null,Unit>::op.
     *
     */
    ;
    
      template
      < class Null
      , class Unit
      , template<class C>class Converter
      >
    struct fold_null_unit
      < Null
      , Unit
      , Converter
      >
    : Unit
    {
          template
          < typename... U
          >
        struct apply
        : fold_null_unit
          < Null
          , Unit
          , Converter
          , U...
          >
        {};
    };
    
      template
      < class Null
      , class Unit
      , template<class C>class Converter
      , typename Head
      , typename... Tail
      >
    struct fold_null_unit
      < Null
      , Unit
      , Converter
      , Head
      , Tail...
      >
    : aux::null_unit_impl
      < Null
      , Unit
      , Converter
      , typename Converter<Head>::type
      , fold_null_unit
        < Null
        , Unit
        , Converter
        , Tail...
        >
      >
    {
          template
          < typename... U
          >
        struct apply
        : fold_null_unit
          < Null
          , Unit
          , Converter
          , U...
          >
        {};
    };
    
}//exit mpl namespace
}//exit boost namespace
#endif

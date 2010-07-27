#ifndef BOOST_MPL_NULL_UNIT_HPP_INCLUDED
#define BOOST_MPL_NULL_UNIT_HPP_INCLUDED

namespace boost
{
namespace mpl
{
  namespace aux
  {
        template
        < class Null
        , class Unit
        , template<class C>class Converter
        , typename T1
        , typename T2
        >
      struct null_unit_impl
      : Converter<T2>::type
           //T1 is Unit because, otherwise,
           //the null_unit_impl specialization
           //below would be used.
      {};
        template
        < class Null
        , class Unit
        , template<class C>class Converter
        , class T2
        >
      struct null_unit_impl
        < Null
        , Unit
        , Converter
        , Null
        , T2
        >
      : Null //short-circuit the evaluation.
      {};
      
  }//exit aux namespace
  
      template
      < class Null
      , class Unit
      , template<class C>class Converter
      >
    struct null_unit
      /**@brief
       *  Nested template defines binary
       *  metafunction for operation with
       *  'Null' and 'Unit' elements.
       *  Converter<C>::type converts C to {Null,Unit}.
       *
       *  For some binary operation, Op,
       *  over domain D:
       *    D Op(D,D)
       *  (i.e. Op is a function taking 2
       *  D's and returning a D.)
       *  the following statements are true:
       *
       *    1) D is {Null,Unit}
       *
       *    2) For any T in D:
       *
       *      2.1) T Op Null == Null
       *      2.2) Null Op T == Null
       *      2.3) T Op Unit == T
       *      2.4) Unit Op T == T
       *    
       */
    {
          template
          < typename T1
          , typename T2
          >
        struct op
        : aux::null_unit_impl
          < Null
          , Unit
          , Converter
          , typename Converter<T1>::type
          , T2
          >
        {
        };
    };
    
    
}//exit mpl namespace
}//exit boost namespace
#endif

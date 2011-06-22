#ifndef BOOST_ARRAY_STEPPER_PERMUTATION0_LAST_HPP_INCLUDED
#define BOOST_ARRAY_STEPPER_PERMUTATION0_LAST_HPP_INCLUDED
#include <boost/array_stepper/mk_iota.hpp>

namespace boost
{
namespace array_stepper
{

enum permut_predefined
{ permut_reverse //permutation in monotonically decreasing from last...0.
, permut_forward //permutaion in montonically increasing from 0...last.
};
enum
{ permut_predef_num=permut_forward+1
};
    static
  permut_predefined const
permut_default
=permut_reverse
;

  template
  < typename Value=unsigned
  >
  struct
permutation0_last
{
        typedef
      std::vector<Value>
    permute_t
      /**@brief
       *  Type of a permutaion of , 0...Last-1
       *  for some Value Last.
       */
    ;
        typedef typename
      permute_t::value_type
    permute_val_t
    ;
        typedef
      permute_t
    (*permute_0to_t) //function returning a permutation of [0,a_last)
      ( permute_val_t //a_last
      )
    ;
        static
      permute_t
    identity_permutation
      ( permute_val_t a_last
      )
    {
      return mk_iota( permute_val_t(0), a_last);
    }
    
        static
      permute_t
    reverse_permutation
      ( permute_val_t a_last
      )
    {
      return mk_iota( a_last, permute_val_t(0));
    }
    
        static
      permute_0to_t
    permute_select
      ( permut_predefined a_permut_pred=permut_default
      )
    {
        static permute_0to_t mk[permut_predef_num]=
          { reverse_permutation
          , identity_permutation
          };
        return mk[a_permut_pred];
    }
    
};  

}//exit array_stepper namespace
}//exit boost namespace
#endif

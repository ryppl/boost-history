
#ifndef BOOST_MPL_TRANSFORM_PACK_HPP_INCLUDED
#define BOOST_MPL_TRANSFORM_PACK_HPP_INCLUDED

#include <boost/mpl/back_inserter_pack.hpp>
#include <boost/mpl/package.hpp>

namespace boost { namespace mpl {

  template
  < typename Sequence
  , typename OpUnary
  , typename Inserter=back_inserter_pack<package<> >
  >
struct transform1_pack
;  
  template
  < template<typename...>class Sequence
  , typename... SeqElements
  , typename OpUnary
  , typename Inserter
  >
struct transform1_pack
  < Sequence<SeqElements...>
  , OpUnary
  , Inserter
  >
{
        typedef
      package
      < typename apply<OpUnary,SeqElements>::type...
      >
    op_elements
    ;
        typedef
      typename Inserter::template apply
      < op_elements
      >::type
    type
    ;
};  
  template
  < typename Sequence1
  , typename Sequence2
  , typename OpBinary
  , typename Inserter=back_inserter_pack<package<> >
  >
struct transform2_pack
;
  template
  < template<typename...>class Sequence1
  , typename... SeqElements1
  , template<typename...>class Sequence2
  , typename... SeqElements2
  , typename OpBinary
  , typename Inserter
  >
struct transform2_pack
  < Sequence1<SeqElements1...>
  , Sequence2<SeqElements2...>
  , OpBinary
  , Inserter
  >
{
        typedef
      package
      < typename apply<OpBinary,SeqElements1,SeqElements2>::type...
      >
    op_elements
    ;
        typedef
      typename Inserter::template apply
      < op_elements
      >::type
    type
    ;
};  

}}

#endif // BOOST_MPL_TRANSFORM_PACK_HPP_INCLUDED

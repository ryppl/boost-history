
#ifndef BOOST_MPL_BACK_INSERTER_PACK_HPP_INCLUDED
#define BOOST_MPL_BACK_INSERTER_PACK_HPP_INCLUDED

namespace boost {
namespace mpl {

  template
  < typename NowPackage
  >
struct back_inserter_pack
;
  template
  < template<typename...>class NowPackage
  , typename... NowElements
  >
struct back_inserter_pack
  < NowPackage<NowElements...>
  >  
{
      template
      < typename InsertedPackage
      >
    struct apply
    ;
      template
      < template<typename...>class InsertedPackage
      , typename... InsertedElements
      >
    struct apply
      < InsertedPackage<InsertedElements...>
      >
    {
            typedef
          NowPackage
          < NowElements...
          , InsertedElements...
          >
        type
        ;  
    };
};

}}

#endif // BOOST_MPL_BACK_INSERTER_PACK_HPP_INCLUDED

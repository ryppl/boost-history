//compile-time composition of alignments
#ifndef BOOST_COMPOSITE_STORAGE_ALIGNMENT_COMPOSE_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_ALIGNMENT_COMPOSE_HPP_INCLUDED

#ifdef COMPOSE_ALIGNMENTS_WITH_LCM
  #include <boost/math/common_factor_ct.hpp>
#endif

namespace boost
{
namespace composite_storage
{
namespace alignment
{
  template
  < std::size_t Left
  , std::size_t Right
  >
  struct
compose
/**@brief
 *  "Metafunction" returning alignment which is compatible with the 
 *   metafunction argument alignments, Left and Right.
 *
 *  The real reason why this template was created instead of directly using
 *  static_lcm was to provide the documentation shown below which justifies
 *  static_lcm use.
 */
{
        static
      std::size_t const
    value=
    #ifdef COMPOSE_ALIGNMENTS_WITH_LCM
      ::boost::math::static_lcm
      < (unsigned long)Left
      , (unsigned long)Right
      >::value
      //The use of static_lcm is based on the statement:
      //
      //  N is the least common multiple of all Alignments
      //
      //in paragraph 1 on page 10 of:
      //
      //  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2165.pdf
      //
      //AFAICT, the reason lcm is used instead of just taking the
      //maximum:
      //
      //  Left>Right?Left:Right
      //
      //is to account for "Extended alignments" mentioned in paragraph 3 
      //on page 3 of the n2165 reference mentioned above.  OTOH, if only
      //"Fundamental alignments" are used (which, I assume, are the
      //"static alignments;powers of 2" in paragraph 4 on page 5 of:
      //
      //  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2006/n2140.pdf
      //
      //) then simply using max instead of lcm would work.
    #else
      Left>Right?Left:Right 
      //2010-03-11:
      //  this is what boost::variant does currently, 
      //  although there's a pending fix:
      //    https://svn.boost.org/trac/boost/ticket/993
      //
    #endif
    ;
};
  
}//exit alignment namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

//specializations define number of enumerators in a specific enumeration.
#ifndef BOOST_INDEXED_TYPES_ENUM_SIZE_HPP_LJE20041205
#define BOOST_INDEXED_TYPES_ENUM_SIZE_HPP_LJE20041205
namespace boost
{
namespace indexed_types
{
template
  < typename Enumeration
  >
  struct
enum_size
/**@brief
 *  Specializations define the size (number of elements)
 *  in Enumeration as:
 *    static unsigned const value= <elements in Enumeration>;
 *
 *  Example:
 *    enum enum1{ i1_0, i1_1, i1_2};
 *    template<>enum_size<enum1>{ static unsigned const value=i1_2+1;};
 */
;

}//exit indexed_types namespace
}//exit boost namespace
#endif

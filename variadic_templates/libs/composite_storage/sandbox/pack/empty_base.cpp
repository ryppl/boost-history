//Purpose:
//  Demostrate effect of using empty base class
//  in various constructs.
//Reference:
//  [EBO] http://www.cantrip.org/emptyopt.html
//Dependencies:
//  [boost.variant]
//    http://www.boost.org/doc/libs/1_44_0/doc/html/variant.html
//  [boost.sandbox.vrtpl.cs]
//    https://svn.boost.org/trac/boost/browser/sandbox/variadic_templates/boost/composite_storage
//    
#include <boost/composite_storage/pack/container_all_of_aligned.hpp>
#include <boost/composite_storage/pack/container_one_of_maybe.hpp>
#include <boost/variant.hpp>
#include <iostream>

  template
  < unsigned I
  >
struct empty_class
/**@brief
 *  This is an empty class type, according to [EBO].
 */
{
        friend
      std::ostream&
    operator<<
      ( std::ostream& sout
      , empty_class<I>const&
      )
    {
        sout<<"empty_class<"<<I<<">const&";
        return sout;
    }
      unsigned
    get(void)const
    {
        return I;
    }
};

struct empties_inherit
: empty_class<0>
, empty_class<1>
/*@brief
 *  According to [EBO] this class is not an empty class 
 *  because its sequence of base class objects is not empty.
 */
{};

template<unsigned I, typename T>
struct index_inherit
: T
{};

struct empties_tree_inherit
: index_inherit<0,empties_inherit>
, index_inherit<1,empties_inherit>
/**@brief
 *  Since empties_inherit is not an empty class,
 *  the empty-base-class-optimization doesn't apply here;
 *  hence sizeof(empties_tree_inherit)>1.
 */
{};

struct empties_member
{
  empty_class<0> m0;
  empty_class<1> m1;
};

    typedef
  char
index_type
;
    typedef
  boost::mpl::integral_c<index_type,index_type(0)>
index0
;  
namespace boost
{
namespace composite_storage
{
  template<>
  struct enum_base<index0>
  {
          typedef 
        char //save space in one_of_maybe
      type;
  };
}
}
    typedef
  boost::composite_storage::pack::container
  < boost::composite_storage::tags::all_of_aligned
  , index0
  , empty_class<0>
  , empty_class<1>
  >
empties_all_of
;
    typedef
  boost::composite_storage::pack::container
  < boost::composite_storage::tags::all_of_aligned
  , index0
  , empties_all_of
  , empties_all_of
  >
empties_tree_all_of
;
    typedef
  boost::composite_storage::pack::container
  < boost::composite_storage::tags::one_of_maybe
  , index0
  , empty_class<0>
  , empty_class<1>
  >
empties_one_of
;
    typedef
  boost::variant
  < empty_class<0>
  , empty_class<1>
  >
empties_variant
;
int main(void)
{
    std::cout<<"sizeof(empty_class<0>)="<<sizeof(empty_class<0>)<<"\n";
    std::cout<<"sizeof(index_inherit<0,empties_inherit>)="
      <<sizeof(index_inherit<0,empties_inherit>)<<"\n";
    std::cout<<"sizeof(empties_inherit)="<<sizeof(empties_inherit)<<"\n";
    std::cout<<"sizeof(empties_tree_inherit)="<<sizeof(empties_tree_inherit)<<"\n";
    std::cout<<"sizeof(empties_member)="<<sizeof(empties_member)<<"\n";
    std::cout<<"sizeof(empties_all_of)="<<sizeof(empties_all_of)<<"\n";
    std::cout<<"sizeof(empties_tree_all_of)="<<sizeof(empties_tree_all_of)<<"\n";
    std::cout<<"sizeof(empties_variant)="<<sizeof(empties_variant)<<"\n";
    std::cout<<"sizeof(empties_one_of)="<<sizeof(empties_one_of)<<"\n";
    std::cout
      <<"sizeof(enum_base<index0>::type)="
      <<sizeof(boost::composite_storage::enum_base<index0>::type)<<"\n";
    {
        empties_inherit ei_v;
        empty_class<0>*p0=&ei_v;
        empty_class<1>*p1=&ei_v;
        std::cout<<"p0="<<p0<<"\n";
        std::cout<<"p1="<<p1<<"\n";
        std::cout<<"*p0="<<*p0<<"\n";
        std::cout<<"*p1="<<*p1<<"\n";
    }
    {
        empties_all_of ea_v;
        empty_class<0>&r0=ea_v.project<0>();
        empty_class<1>&r1=ea_v.project<1>();
        std::cout<<"r0="<<r0<<"\n";
        std::cout<<"r1="<<r1<<"\n";
        std::cout<<"&ea_v="<<&ea_v<<"\n";
        std::cout<<"&r0="<<&r0<<"\n";
        std::cout<<"&r1="<<&r1<<"\n";
        std::cout<<"r1.get()="<<r1.get()<<"\n";
    }
    return 0;
}    

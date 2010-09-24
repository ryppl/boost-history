//Purpose:
//  See if more than 1 empty base types contribute 
//  more to size of derived type than just 1
//  empty_base type.
//Result:
//  No. Output is:
//    sizeof(empty_class<0>)=1
//    sizeof(empties_inherits)=1
//    sizeof(empties_member)=2
//    sizeof(empties_all_of)=1
//
#include <boost/composite_storage/pack/container_all_of_aligned.hpp>
#include <boost/composite_storage/pack/container_one_of_maybe.hpp>
#include <boost/variant.hpp>
#include <iostream>

  template
  < unsigned I
  >
struct empty_class
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
  #if 0
    unsigned my_i;
    empty_class(void):my_i(I){}
  #endif
};

struct empties_inherit
: empty_class<0>
, empty_class<1>
{};

template<unsigned I, typename T>
struct index_inherit
: T
{};

struct empties_tree_inherit
: index_inherit<0,empties_inherit>
, index_inherit<1,empties_inherit>
{};

struct empties_member
{
  empty_class<0> m0;
  empty_class<1> m1;
};
    typedef
  boost::mpl::integral_c<unsigned,0>
index0
;  
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
    std::cout<<"sizeof(empties_inherit)="<<sizeof(empties_inherit)<<"\n";
    std::cout<<"sizeof(empties_tree_inherit)="<<sizeof(empties_tree_inherit)<<"\n";
    std::cout<<"sizeof(empties_member)="<<sizeof(empties_member)<<"\n";
    std::cout<<"sizeof(empties_all_of)="<<sizeof(empties_all_of)<<"\n";
    std::cout<<"sizeof(empties_tree_all_of)="<<sizeof(empties_tree_all_of)<<"\n";
    std::cout<<"sizeof(empties_one_of)="<<sizeof(empties_one_of)<<"\n";
    std::cout<<"sizeof(empties_variant)="<<sizeof(empties_variant)<<"\n";
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

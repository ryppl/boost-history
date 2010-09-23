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

struct empties_member
{
  empty_class<0> m0;
  empty_class<1> m1;
};
    typedef
  boost::composite_storage::pack::container
  < boost::composite_storage::tags::all_of_aligned
  , boost::mpl::integral_c<unsigned,0>
  , empty_class<0>
  , empty_class<1>
  >
empties_all_of
;
int main(void)
{
    std::cout<<"sizeof(empty_class<0>)="<<sizeof(empty_class<0>)<<"\n";
    std::cout<<"sizeof(empties_inherit)="<<sizeof(empties_inherit)<<"\n";
    std::cout<<"sizeof(empties_member)="<<sizeof(empties_member)<<"\n";
    std::cout<<"sizeof(empties_all_of)="<<sizeof(empties_all_of)<<"\n";
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

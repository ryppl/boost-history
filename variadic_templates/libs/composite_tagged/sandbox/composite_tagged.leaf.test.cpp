
#include <boost/composite_tagged.hpp>

#include <iostream>
namespace boost
{
namespace composite_tagged_leaf
{
    static
  unsigned
object_id
=0
;
    static
  int
object_number
=0
;
struct obj
{
      unsigned const
    my_id
    ;
    obj(void)
    : my_id(++object_id)
    {
        ++object_number;
    }
    obj(obj const&)
    : my_id(++object_id)
    {
        ++object_number;
    }
    ~obj(void)
    {
        --object_number;
    }
      void
    operator=(obj const&)
    {}
};
  template
  < unsigned I
  >
  struct
charvec_u
: obj
{
    char v[2*(I+1)];
    unsigned tag(void)const
    {
        return I;
    }
    charvec_u(void)
    {
        v[0]='a';
        v[1]='\0';
    }
};

  enum
index_numerals
{ index_0
, index_1
, index_2
, index_3
, index_4
};

void test(void)
{
#if 1
    std::cout<<"object_number="<<object_number<<"\n";
    {    
            typedef
          composite_tagged
          < composite_tags::one_of
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_scanned
        layout_scanned
        ;
        std::cout
          <<"***composite_tagged<one_of>:\n"
          <<":size="<<layout_scanned::size_part<<"\n"
          <<":alignment="<<layout_scanned::align_part<<"\n"
        ;
          tagged_type 
        tagged_values
        ;
          charvec_u<0>*
        t0
        ;
          charvec_u<1>*
        t1
        ;
        std::cout
          <<"which="<<tagged_values.which()<<"\n";
        t0=tagged_values.project<index_0>();
        std::cout
          <<"t0="<<t0<<"\n";
        t1=tagged_values.project<index_1>();
        std::cout
          <<"t1="<<t1<<"\n";
          
        tagged_values.inject<index_0>(charvec_u<0>());
        std::cout
          <<"which="<<tagged_values.which()<<"\n";
        t0=tagged_values.project<index_0>();
        std::cout
          <<"t0="<<t0<<"\n";
        t1=tagged_values.project<index_1>();
        std::cout
          <<"t1="<<t1<<"\n";
          
        tagged_values.inject<index_1>(charvec_u<1>());
        std::cout
          <<"which="<<tagged_values.which()<<"\n";
        t0=tagged_values.project<index_0>();
        std::cout
          <<"t0="<<t0<<"\n";
        t1=tagged_values.project<index_1>();
        std::cout
          <<"t1="<<t1<<"\n";
        std::cout
          <<"assign_test:\n";
          tagged_type
        tagged_from
        ;
        tagged_values=tagged_from;
                std::cout
          <<"which="<<tagged_values.which()<<"\n";
        t0=tagged_values.project<index_0>();
        std::cout
          <<"t0="<<t0<<"\n";
        t1=tagged_values.project<index_1>();
        std::cout
          <<"t1="<<t1<<"\n";

    }
#endif    
#if 1
    std::cout<<"object_number="<<object_number<<"\n";
    {
        std::cout
          <<"sizeof(charvec_u<0>)="<<sizeof(charvec_u<0>)<<"\n"
          <<"sizeof(charvec_u<1>)="<<sizeof(charvec_u<1>)<<"\n"
          <<"sizeof(charvec_u<2>)="<<sizeof(charvec_u<2>)<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < composite_tags::all_of_packed
          >::layout0<mpl::integral_c<int,-1> >
        tp0_type
        ;
        std::cout
          <<"***layout_operators<all_of_packed>::layout0<integral_c<int,-1>\n"
          <<":size="<<tp0_type::size_part<<"\n"
          <<":index="<<tp0_type::index_part::value<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < composite_tags::all_of_packed
          >::push_back
          < tp0_type
          , charvec_u<0>
          >::type
        tp1_type
        ;
        std::cout
          <<"***layout_operators<all_of_packed>::push_back<tp0_type,charvec_u<0> >\n"
          <<":size="<<tp1_type::size_part<<"\n"
          <<":index="<<tp1_type::index_part::value<<"\n"
          <<":offset(0)="<<tp1_type::offset(mpl::integral_c<int,0>())<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < composite_tags::all_of_packed
          >::push_back
          < tp1_type
          , charvec_u<1>
          >::type
        tp2_type
        ;
        std::cout
          <<"***layout_operators<all_of_packed>::push_back<tp1_type,charvec_u<1> >\n"
          <<":size="<<tp2_type::size_part<<"\n"
          <<":index="<<tp2_type::index_part::value<<"\n"
          <<":offset(1)="<<tp2_type::offset(mpl::integral_c<int,1>())<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < composite_tags::all_of_packed
          >::push_back
          < tp2_type
          , charvec_u<2>
          >::type
        tp3_type
        ;
        std::cout
          <<"***layout_operators<all_of_packed>::push_back<tp2_type,charvec_u<2> >\n"
          <<":size="<<tp3_type::size_part<<"\n"
          <<":index="<<tp3_type::index_part::value<<"\n"
          <<":offset(2)="<<tp3_type::offset(mpl::integral_c<int,2>())<<"\n"
        ;
        std::cout
          <<":offset(3)="<<tp3_type::offset(mpl::integral_c<int,3>())<<"\n"
        ;
    }
#endif    
#if 1
    std::cout<<"object_number="<<object_number<<"\n";
    {    
            typedef
          composite_tagged
          < composite_tags::all_of_packed
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_scanned
        layout_scanned
        ;
            typedef
          tagged_type::index_base
        index_base
        ;
          tagged_type
        tagged_valu
        ;
        std::cout
          <<"***composite_tagged<all_of_packed>:\n"
          <<":size="<<layout_scanned::size_part<<"\n"
          <<":offset<0>="<<layout_scanned::offset(mpl::integral_c<index_base,index_0>())<<"\n"
          <<":offset<1>="<<layout_scanned::offset(mpl::integral_c<index_base,index_1>())<<"\n"
          <<":offset<2>="<<layout_scanned::offset(mpl::integral_c<index_base,index_2>())<<"\n"
          <<":offset<3>="<<layout_scanned::offset(mpl::integral_c<index_base,index_3>())<<"\n"
        ;
        
        charvec_u<index_1> c_1;
        c_1.v[0]='b';
        std::cout
          <<"  (before project<index_1>='b')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        tagged_valu.project<index_1>(c_1);
        std::cout
          <<"  (after project<index_1>='b')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        c_1.v[0]='c';
        std::cout
          <<"  (before inject<index_1>='c')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        tagged_valu.inject<index_1>(c_1);
        std::cout
          <<"  (after inject<index_1>='c')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        c_1.v[0]='d';
        tagged_valu.project<index_1>(c_1);
        std::cout
          <<"  (after project<index_1>='d')\n"
          <<":c_1.v="<<c_1.v<<"\n";
          
        charvec_u<index_2> c_2;
        c_2.v[0]='b';
        std::cout
          <<"  (before project<index_2>='b')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        tagged_valu.project<index_2>(c_2);
        std::cout
          <<"  (after project<index_2>='b')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        c_2.v[0]='c';
        std::cout
          <<"  (before inject<index_2>='c')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        tagged_valu.inject<index_2>(c_2);
        std::cout
          <<"  (after inject<index_2>='c')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        c_2.v[0]='d';
        tagged_valu.project<index_2>(c_2);
        std::cout
          <<"  (after project<index_2>='d')\n"
          <<":c_2.v="<<c_2.v<<"\n";
          
    }
#endif    
#if 1
    std::cout<<"object_number="<<object_number<<"\n";
    {    
            typedef
          composite_tagged
          < composite_tags::all_of_aligned
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_scanned
        layout_scanned
        ;
            typedef
          tagged_type::index_base
        index_base
        ;
          tagged_type
        tagged_valu
        ;
        std::cout
          <<"***composite_tagged<all_of_aligned>:\n"
          <<":size="<<layout_scanned::size_part<<"\n"
          <<":offset<0>="<<layout_scanned::offset(mpl::integral_c<index_base,index_0>())<<"\n"
          <<":offset<1>="<<layout_scanned::offset(mpl::integral_c<index_base,index_1>())<<"\n"
          <<":offset<2>="<<layout_scanned::offset(mpl::integral_c<index_base,index_2>())<<"\n"
          <<":offset<3>="<<layout_scanned::offset(mpl::integral_c<index_base,index_3>())<<"\n"
          <<":project<index_0>="<<tagged_valu.project<index_0>().v<<"\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
        tagged_valu.project<index_1>().v[0]='b';
        std::cout
          <<"***composite_tagged<all_of_aligned>:\n"
          <<"  (after project<index_1>='b')\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
    }
#endif    
    std::cout<<"final:object_number="<<object_number<<"\n";
}

}//exit composite_tagged_leaf namespace
}//exit boost namespace

int main(void)
{
    boost::composite_tagged_leaf::test();
    return 0;
}    


#include <boost/composite_tagged.hpp>

#include <iostream>
namespace boost
{
namespace composite_tagged_leaf
{
  template
  < unsigned I
  >
  struct
charvec_u
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
#if 0
    {
            typedef
          detail::layout_make
          < composite_tags::one_of
          >::push_back
          < detail::size_alignment<>
          , detail::size_alignment<2,9>
          >::type
        tp_type
        ;
        std::cout
          <<"***layout_make<one_of>::push_back<<>,<2,9>>\n"
          <<":size="<<tp_type::size_part<<"\n"
          <<":alignment="<<tp_type::align_part<<"\n"
        ;
    }
#endif    
#if 1
    {    
            typedef
          composite_tagged
          < composite_tags::one_of
          , index_numerals
          , charvec_u<0>
          , charvec_u<1>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_type
        layout_type
        ;
        std::cout
          <<"***composite_tagged<one_of>:\n"
          <<":size="<<layout_type::size_part<<"\n"
          <<":alignment="<<layout_type::align_part<<"\n"
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
    }
#endif    
#if 0
    {
        std::cout
          <<"sizeof(charvec_u<0>)="<<sizeof(charvec_u<0>)<<"\n"
          <<"sizeof(charvec_u<1>)="<<sizeof(charvec_u<1>)<<"\n"
          <<"sizeof(charvec_u<2>)="<<sizeof(charvec_u<2>)<<"\n"
        ;
            typedef
          detail::layout_make
          < composite_tags::all_of_packed
          >::layout0
        tp0_type
        ;
            typedef
          detail::layout_make
          < composite_tags::all_of_packed
          >::push_back
          < tp0_type
          , charvec_u<0>
          >::type
        tp1_type
        ;
        std::cout
          <<"***layout_make<all_of_packed>::push_back<tp0_type,charvec_u<0> >\n"
          <<":size="<<tp1_type::size_part<<"\n"
          <<":index="<<tp1_type::index_part<<"\n"
          <<":offset(0)="<<tp1_type::offset(index_wrap<0>())<<"\n"
        ;
            typedef
          detail::layout_make
          < composite_tags::all_of_packed
          >::push_back
          < tp1_type
          , charvec_u<1>
          >::type
        tp2_type
        ;
        std::cout
          <<"***layout_make<all_of_packed>::push_back<tp1_type,charvec_u<1> >\n"
          <<":size="<<tp2_type::size_part<<"\n"
          <<":index="<<tp2_type::index_part<<"\n"
          <<":offset(1)="<<tp2_type::offset(index_wrap<1>())<<"\n"
        ;
            typedef
          detail::layout_make
          < composite_tags::all_of_packed
          >::push_back
          < tp2_type
          , charvec_u<2>
          >::type
        tp3_type
        ;
        std::cout
          <<"***layout_make<all_of_packed>::push_back<tp2_type,charvec_u<2> >\n"
          <<":size="<<tp3_type::size_part<<"\n"
          <<":index="<<tp3_type::index_part<<"\n"
          <<":offset(2)="<<tp3_type::offset(index_wrap<2>())<<"\n"
        ;
        std::cout
          <<":offset(3)="<<tp3_type::offset(index_wrap<3>())<<"\n"
        ;
    }
#endif    
#if 1
    {    
            typedef
          composite_tagged
          < composite_tags::all_of_packed
          , index_numerals
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_type
        layout_type
        ;
          tagged_type
        tagged_valu
        ;
        std::cout
          <<"***composite_tagged<all_of_packed>:\n"
          <<":size="<<layout_type::size_part<<"\n"
          <<":offset<0>="<<layout_type::offset(detail_composite_tagged::index_wrap<0>())<<"\n"
          <<":offset<1>="<<layout_type::offset(detail_composite_tagged::index_wrap<1>())<<"\n"
          <<":offset<2>="<<layout_type::offset(detail_composite_tagged::index_wrap<2>())<<"\n"
          <<":offset<3>="<<layout_type::offset(detail_composite_tagged::index_wrap<3>())<<"\n"
          <<":project<index_0>="<<tagged_valu.project<index_0>().v<<"\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
        tagged_valu.project<index_1>().v[0]='b';
        std::cout
          <<"***composite_tagged<all_of_packed>:\n"
          <<"  (after project<index_1>='b')\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
    }
#endif    
#if 1
    {    
            typedef
          composite_tagged
          < composite_tags::all_of_aligned
          , index_numerals
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_type
        layout_type
        ;
          tagged_type
        tagged_valu
        ;
        std::cout
          <<"***composite_tagged<all_of_aligned>:\n"
          <<":size="<<layout_type::size_part<<"\n"
          <<":offset<0>="<<layout_type::offset(detail_composite_tagged::index_wrap<0>())<<"\n"
          <<":offset<1>="<<layout_type::offset(detail_composite_tagged::index_wrap<1>())<<"\n"
          <<":offset<2>="<<layout_type::offset(detail_composite_tagged::index_wrap<2>())<<"\n"
          <<":offset<3>="<<layout_type::offset(detail_composite_tagged::index_wrap<3>())<<"\n"
          <<":project<index_0>="<<tagged_valu.project<index_0>().v<<"\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
        tagged_valu.project<index_1>().v[0]='b';
        std::cout
          <<"***composite_tagged<all_of_packed>:\n"
          <<"  (after project<index_1>='b')\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
    }
#endif    
}

}//exit composite_tagged_leaf namespace
}//exit boost namespace

int main(void)
{
    boost::composite_tagged_leaf::test();
    return 0;
}    

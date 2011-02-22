//ChangeLog:
//  2010-05-22.0902
//    WHAT:
//      cp'ed from /home/evansl/prog_dev/boost-svn/ro/trunk
//        /sandbox-local/lje/libs/composite_tagged_seq/sandbox
//        /composite_tagged_seq.leaf.test.cpp
//    WHY:
//      Modify for use with variadic template version of similar composite classes.
//
//#include <boost/composite_storage/pack/container_all_of_aligned.hpp>
#include <boost/composite_storage/pack/container_one_of_maybe.hpp>
#include <boost/composite_storage/pack/container_all_of_aligned.hpp>

#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <iostream>
#include <string>
static std::ostream& ind_out=std::cout;

#include "../../utility/curried_offset.hpp"

#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

struct trace_scope
{
    trace_scope(std::string const& a_where)
    : my_where(a_where)
    {
        ind_out<<"[[[ENTERING:"<<my_where<<"\n";
        ind_out<<indent_buf_in;
    }
    ~trace_scope(void)
    {
        ind_out<<indent_buf_out;
        ind_out<<"]]]EXITING:"<<my_where<<"\n";
    }
 private:
      std::string
    my_where
    ;    
};  

namespace boost
{
namespace composite_storage
{
namespace leaf_test
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
      unsigned
    id_get(void)const
    {
        return my_id;
    }
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

  template
  < unsigned I
  >
  std::ostream&
operator<<
  ( std::ostream& sout
  , charvec_u<I>const& x
  )
{
    sout
      <<"charvec_u<"<<I<<">"
      <<":id_get="<<x.id_get()
      <<":v[0]="<<x.v[0]
      ;
    return sout;
}  

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
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("one_of_maybe on non-class types TESTS");
            typedef
          pack::container
          < tags::one_of_maybe
          , mpl::integral_c<index_numerals,index_0>
          , int
          , float
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_comp
        layout_comp
        ;
            typedef
          layout_comp::scanned
        layout_scanned
        ;
        ind_out
          <<"***composite_storage<one_of_maybe>:\n"
          <<":size="<<layout_scanned::comp_part::size<<"\n"
          <<":alignment="<<layout_scanned::comp_part::align<<"\n"
        ;
        BOOST_MPL_ASSERT((is_same<layout_comp::result_type<index_0>::type,int>));
        BOOST_MPL_ASSERT((is_same<tagged_type::result_type<index_1>::type,float>));
        {
            trace_scope ts("default CTOR");
              tagged_type 
            tagged_values
            ;
              int*
            t0
            ;
              float*
            t1
            ;
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
              
            tagged_values.inject<index_0>(9);
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
            t0=&tagged_values.project<index_0>();
            ind_out
              <<"t0="<<*t0<<"\n";

            tagged_values.inject<index_1>(8.5);
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
            t1=&tagged_values.project<index_1>();
            ind_out
              <<"t1="<<*t1<<"\n";

        }
    } 
    {    
        trace_scope ts("one_of_maybe on class types TESTS");
            typedef
          pack::container
          < tags::one_of_maybe
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          >
        tagged_type
        ;
            typedef
          tagged_type::layout_comp::scanned
        layout_scanned
        ;
        ind_out
          <<"***composite_storage<one_of_maybe>:\n"
          <<":size="<<layout_scanned::comp_part::size<<"\n"
          <<":alignment="<<layout_scanned::comp_part::align<<"\n"
        ;
        {
            trace_scope ts("default CTOR");
              tagged_type 
            tagged_values
            ;
              charvec_u<0>*
            t0
            ;
              charvec_u<1>*
            t1
            ;
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
              
            tagged_values.inject<index_0>(charvec_u<0>());
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
            t0=&tagged_values.project<index_0>();
            ind_out
              <<"t0="<<*t0<<"\n";
              
            unsigned obj_id_saved=t0->id_get();
              
            tagged_values.inject<index_1>(charvec_u<1>());
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
            t1=&tagged_values.project<index_1>();
            ind_out
              <<"t1="<<*t1<<"\n";
              
            //Only 2 more objects should have been created since
            //obj_id_saved was set:
            //  1)  The charvec_u<1>() arg to inject<index_1>
            //  2)  The charvec_u<1> actually injected into tagged_values
            //Check that:
            BOOST_ASSERT((obj_id_saved+2==t1->id_get()));
              
            ind_out
              <<"assign_test:\n";
              tagged_type
            tagged_from
            ;
            tagged_values=tagged_from;
                    ind_out
              <<"which="<<tagged_values.which()<<"\n";
        }
        {
            trace_scope ts("index_component CTOR");
                typedef
              tagged_type::index_component<index_1,charvec_u<1>const&>
            index_component_1_type
            ;
              tagged_type 
            tagged_values
              ( index_component_1_type::_()
              )
            ;
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
        }
      #if 0
        {
            //2010-03-19:
            //  This feature:
            //    CTOR arg pairs where 1st arg is index
            //    and 2dn arg is value,
            //  is not implemented yet.  Instead, please
            //  use the index_component CTOR feature
            //  (whose test is immediately above).
            //
            trace_scope ts("assign_at CTOR");
              tagged_type 
            tagged_values
              ( index_1
              , charvec_u<1>()
              )
            ;
            ind_out
              <<"which="<<tagged_values.which()<<"\n";
        }
      #endif

    }
#endif    
#if 0
    ind_out<<"object_number="<<object_number<<"\n";
    {
        trace_scope ts("all_of_packed layout_operators TESTS");
        ind_out
          <<"sizeof(charvec_u<0>)="<<sizeof(charvec_u<0>)<<"\n"
          <<"sizeof(charvec_u<1>)="<<sizeof(charvec_u<1>)<<"\n"
          <<"sizeof(charvec_u<2>)="<<sizeof(charvec_u<2>)<<"\n"
        ;
            typedef
          mpl::integral_c<int,-1>
        index_undef
        ;
            typedef
          detail_composite_tagged::layout_operators
          < tags::all_of_packed
          >::layout0<index_undef>
        tp0_type
        ;
        ind_out
          <<"***layout_operators<all_of_packed>::layout0<integral_c<int,-1>\n"
          <<":size="<<tp0_type::comp_part::size<<"\n"
          <<":index="<<tp0_type::index_part::value<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < tags::all_of_packed
          >::push_back
          < tp0_type
          , charvec_u<0>
          >::type
        tp1_type
        ;
        ind_out
          <<"***layout_operators<all_of_packed>::push_back<tp0_type,charvec_u<0> >\n"
          <<":size="<<tp1_type::comp_part::size<<"\n"
          <<":index="<<tp1_type::index_part::value<<"\n"
          <<":offset(0)="<<tp1_type::comp_part::offset<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < tags::all_of_packed
          >::push_back
          < tp1_type
          , charvec_u<1>
          >::type
        tp2_type
        ;
        ind_out
          <<"***layout_operators<all_of_packed>::push_back<tp1_type,charvec_u<1> >\n"
          <<":size="<<tp2_type::comp_part::size<<"\n"
          <<":index="<<tp2_type::index_part::value<<"\n"
          <<":offset(1)="<<tp2_type::comp_part::offset<<"\n"
        ;
            typedef
          detail_composite_tagged::layout_operators
          < tags::all_of_packed
          >::push_back
          < tp2_type
          , charvec_u<2>
          >::type
        tp3_type
        ;
        ind_out
          <<"***layout_operators<all_of_packed>::push_back<tp2_type,charvec_u<2> >\n"
          <<":size="<<tp3_type::comp_part::size<<"\n"
          <<":index="<<tp3_type::index_part::value<<"\n"
          <<":offset(2)="<<tp3_type::comp_part::offset<<"\n"
        ;
    }
#endif    
#if 0
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("all_of_packed composite_storage TESTS");
            typedef
          pack::container
          < tags::all_of_packed
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          libs::composite_storage::utility::curried_offset<tagged_type>
        cur_off
        ;
            typedef
          tagged_type::layout_comp::scanned
        layout_scanned
        ;
            typedef
          tagged_type::index_base
        index_base
        ;
          tagged_type
        tagged_valu
        ;
        ind_out
          <<"***composite_storage<all_of_packed>:\n"
          <<":size="<<layout_scanned::comp_part::size<<"\n"
          <<":offset<0>="<<cur_off::offset_at<index_0>::value<<"\n"
          <<":offset<1>="<<cur_off::offset_at<index_1>::value<<"\n"
          <<":offset<2>="<<cur_off::offset_at<index_2>::value<<"\n"
        #if 0
          //This will cause compile error since only 3 elments in composite.
          <<":offset<3>="<<cur_off::offset_at<index_3>::value<<"\n"
        #endif
        ;
        
        charvec_u<index_1> c_1;
        c_1.v[0]='b';
        ind_out
          <<"  (before project<index_1>='b')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        tagged_valu.project<index_1>(c_1);
        ind_out
          <<"  (after project<index_1>='b')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        c_1.v[0]='c';
        ind_out
          <<"  (before inject<index_1>='c')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        tagged_valu.inject<index_1>(c_1);
        ind_out
          <<"  (after inject<index_1>='c')\n"
          <<":c_1.v="<<c_1.v<<"\n";
        c_1.v[0]='d';
        tagged_valu.project<index_1>(c_1);
        ind_out
          <<"  (after project<index_1>='d')\n"
          <<":c_1.v="<<c_1.v<<"\n";
          
        charvec_u<index_2> c_2;
        c_2.v[0]='b';
        ind_out
          <<"  (before project<index_2>='b')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        tagged_valu.project<index_2>(c_2);
        ind_out
          <<"  (after project<index_2>='b')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        c_2.v[0]='c';
        ind_out
          <<"  (before inject<index_2>='c')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        tagged_valu.inject<index_2>(c_2);
        ind_out
          <<"  (after inject<index_2>='c')\n"
          <<":c_2.v="<<c_2.v<<"\n";
        c_2.v[0]='d';
        tagged_valu.project<index_2>(c_2);
        ind_out
          <<"  (after project<index_2>='d')\n"
          <<":c_2.v="<<c_2.v<<"\n";
          
    }
#endif    
#if 1
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("all_of_aligned composite_storage TESTS");
            typedef
          pack::container
          < tags::all_of_aligned
          , mpl::integral_c<index_numerals,index_0>
          , charvec_u<0>
          , charvec_u<1>
          , charvec_u<2>
          >
        tagged_type
        ;
            typedef
          libs::composite_storage::utility::curried_offset<tagged_type>
        cur_off
        ;
            typedef
          tagged_type::layout_comp::scanned
        layout_scanned
        ;
            typedef
          tagged_type::index_base
        index_base
        ;
          tagged_type
        tagged_valu
        ;
        ind_out
          <<"***composite_storage<all_of_aligned>:\n"
          <<":size="<<layout_scanned::comp_part::size<<"\n"
          <<":offset<0>="<<cur_off::offset_at<index_0>::value<<"\n"
          <<":offset<1>="<<cur_off::offset_at<index_1>::value<<"\n"
          <<":offset<2>="<<cur_off::offset_at<index_2>::value<<"\n"
          <<":project<index_0>="<<tagged_valu.project<index_0>().v<<"\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
        tagged_valu.project<index_1>().v[0]='b';
        ind_out
          <<"***composite_storage<all_of_aligned>:\n"
          <<"  (after project<index_1>='b')\n"
          <<":project<index_1>="<<tagged_valu.project<index_1>().v<<"\n"
        ;
    }
#endif
#if 1
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("all_of_aligned min end pad DEMO");
            typedef
          pack::container
          < tags::all_of_aligned
          , mpl::integral_c<index_numerals,index_0>
          , char
          , int
          , double
          >
        tagged_type
        ;
            typedef
          libs::composite_storage::utility::curried_offset<tagged_type>
        cur_off
        ;
            typedef
          tagged_type::layout_comp::scanned
        layout_scanned
        ;
        ind_out
          <<"***composite_storage<all_of_aligned> min end pad:\n"
          <<":size scan="<<layout_scanned::comp_part::size<<"\n"
          <<":offset<0>="<<cur_off::offset_at<index_0>::value<<"\n"
          <<":offset<1>="<<cur_off::offset_at<index_1>::value<<"\n"
          <<":offset<2>="<<cur_off::offset_at<index_2>::value<<"\n"
          <<":size composite="<<sizeof(tagged_type)<<"\n"
          <<":end pad="<<(sizeof(tagged_type)-layout_scanned::comp_part::size)<<"\n"
          ;
    }
    ind_out<<"object_number="<<object_number<<"\n";
    {    
        trace_scope ts("all_of_aligned max end pad DEMO");
            typedef
          pack::container
          < tags::all_of_aligned
          , mpl::integral_c<index_numerals,index_0>
          , double
          , int
          , char
          >
        tagged_type
        ;
            typedef
          libs::composite_storage::utility::curried_offset<tagged_type>
        cur_off
        ;
            typedef
          tagged_type::layout_comp::scanned
        layout_scanned
        ;
        ind_out
          <<"***composite_storage<all_of_aligned> max end pad:\n"
          <<":size scan="<<layout_scanned::comp_part::size<<"\n"
          <<":offset<0>="<<cur_off::offset_at<index_0>::value<<"\n"
          <<":offset<1>="<<cur_off::offset_at<index_1>::value<<"\n"
          <<":offset<2>="<<cur_off::offset_at<index_2>::value<<"\n"
          <<":size composite="<<sizeof(tagged_type)<<"\n"
          <<":end pad="<<(sizeof(tagged_type)-layout_scanned::comp_part::size)<<"\n"
          ;
    }
#endif    
    ind_out<<"final:object_number="<<object_number<<"\n";
}

}//exit leaf_test
}//exit composite_storage
}//exit boost namespace

int main(void)
{
    boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(std::cout,4);
    boost::composite_storage::leaf_test::test();
    return 0;
}    

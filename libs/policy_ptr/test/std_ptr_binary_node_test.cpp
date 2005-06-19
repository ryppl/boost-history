//test std/policies:shared_storage
#define CONTAINER_SINGLE_OBJ_ID
#include "sp_types_std.hpp"
#include "binary_node.hpp"
#include "sp_graph_dfs_print.hpp"
#include "boost/policy_ptr/detail/collector_sp_counted_accepting.hpp"
#include <boost/test/unit_test.hpp>

  template
  < class SpTypes
  , binary_node::member_multiplicity Multiplicity
  >
struct sp_node
{
        static
      boost::sp_std_numerals const
    our_std_numeral
      = SpTypes::our_std_numeral
    ;
        static
      binary_node::member_multiplicity const
    our_multiplicity
      = Multiplicity
    ;
        typedef 
      typename binary_node::binary_container
      < SpTypes
      , Multiplicity
      >
    type
    ;
};

    typedef 
  sp_node
  < boost::sp_types_std<boost::std_shared_graph_accepting>
  , binary_node::contains_one
  >::type 
desc_type;

SELECTED_FIELDS_DESCRIPTION_OF_RECORD(desc_type)

  template
  < class BinaryNode
  >
void simple_test(void)
{
        typedef 
      typename BinaryNode::type
    node_type
    ;
        typedef
      typename node_type::sp_type
    sp_type
    ;
        typedef
      typename node_type::src_type
    src_type
    ;
    utility::object_tracked::reset();
    {
        node_type a_bs;
        BOOST_CHECK_EQUAL(utility::object_tracked::members_size(), 1u);
        src_type src_left_child;
        sp_type left_child(src_left_child.release());
        BOOST_CHECK_EQUAL(utility::object_tracked::members_size(), 2u);
        BOOST_CHECK_EQUAL(left_child.use_count(), 1);
        a_bs.left()=left_child;
        BOOST_CHECK(a_bs.left() == left_child);
        BOOST_CHECK_EQUAL(left_child.use_count(), 2);
        BOOST_CHECK_EQUAL(left_child.use_count(), 2);
    }
    BOOST_CHECK_EQUAL(utility::object_tracked::members_size(), 0u);
}

#include "binary_node_graph_tests.hpp"
namespace boost
{
  namespace binary_node_graph_tests
  {
      template
      < typename BinaryNode
      >
      struct
    test_ops
    {
            typedef
          policy_ptr::detail::collector
          < policy_ptr::detail::sp_counted_accepting
          >
        collector_type
        ;
            typedef
          typename BinaryNode::sp_type
        sp_type
        ;
            typedef
          std::vector<sp_type*>
        roots_type
        ;
            static
          roots_type&
        the_roots(void)
        {
            static roots_type a_roots;
            return a_roots;
        }
            static
          void
        view(void)
        {
            sp_graph_dfs_print<BinaryNode> dfs_printer;
            mout()<<"dfs_printer:\n";
            for(unsigned i=0; i<the_roots().size(); ++i)
            {
                mout()<<"root_ptr["<<i<<"]=\n";
                ++mout();
                sp_type* a_sp=the_roots()[i];
                if(a_sp)
                {                
                    dfs_printer.visit_arc_check(*a_sp);
                }
                else
                {
                    mout()<<"(null sp*)\n";
                }
                --mout();
            }
        }
        
            static
          void
        in_scope(void)
        {
            collector_type::collect();
        }
        
            static
          void
        out_scope(void)
        {
            collector_type::collect();
        }
    };
    
      bool const
    expect_pass
      [ num_tests ]
      [ binary_node::contains_one+1 ]
      [ num_sp ]
    =
    { //simple_list
      { //contains_one
        { //std_shared_dag
          true
        , //std_shared_graph_accepting
          true
        , //std_shared_graph_tagged
          true
        }
      }
    , //cycle_1_external
      { //contains_one
        { //std_shared_dag
          false
        , //std_shared_graph_accepting
          true
        , //std_shared_graph_tagged
          false
        }
      }
    , //cycle_2_external
      { //contains_one
        { //std_shared_dag
          false
        , //std_shared_graph_accepting
          true
        , //std_shared_graph_tagged
          false
        }
      }
    };
    template
      < test_ids TestId
      , typename BinaryNode
      >
        static
      void
    run(void)
    {
        utility::object_tracked::reset();
        sp_std_numerals const sp_id=BinaryNode::our_std_numeral;
        binary_node::member_multiplicity const multip_id=BinaryNode::our_multiplicity;
        mout()
          <<":TestId="<<TestId
          <<":std_numeral="<<sp_id
          <<":multiplicity="<<multip_id
          <<"\n";
        ++mout();
        test<BinaryNode,test_ops>::vec[TestId](); 
        //The above assumes that the function uses objects derived from object_tracked
        unsigned n_members = utility::object_tracked::members_size();
        mout()<<"n_members="<<n_members<<"\n";
        bool expected_result = expect_pass[TestId][multip_id][sp_id];
        BOOST_CHECK_EQUAL(n_members == 0u, expected_result);
        --mout();
    }
    
  }//exit binary_node_graph_tests namespace
  
}//exit boost namespace

namespace butf = boost::unit_test_framework;
using namespace boost;

  template
  < sp_std_numerals SpId
  >
void add_spid_tests(butf::test_suite* tests)
{
  #if 0
  #endif
    tests->add(BOOST_TEST_CASE((&simple_test<sp_node<sp_types_std<SpId>,binary_node::contains_one> >)));
    {
        binary_node_graph_tests::test_fun_type test_fun=boost::binary_node_graph_tests::run
          < binary_node_graph_tests::simple_list
          , sp_node<sp_types_std<SpId>,binary_node::contains_one>
          >;
        tests->add(BOOST_TEST_CASE(test_fun));
    }
    {
        binary_node_graph_tests::test_fun_type test_fun=boost::binary_node_graph_tests::run
          < binary_node_graph_tests::cycle_1_external
          , sp_node<sp_types_std<SpId>,binary_node::contains_one>
          >;
        tests->add(BOOST_TEST_CASE(test_fun));
    }
    {
        binary_node_graph_tests::test_fun_type test_fun=boost::binary_node_graph_tests::run
          < binary_node_graph_tests::cycle_2_external
          , sp_node<sp_types_std<SpId>,binary_node::contains_one>
          >;
        tests->add(BOOST_TEST_CASE(test_fun));
    }
  #if 0
  #endif
}

void always_fail(void)
{
    BOOST_CHECK(false);
}

butf::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    butf::test_suite* tests = BOOST_TEST_SUITE("std_binary_node tests");
    add_spid_tests<std_shared_graph_accepting>(tests);
    add_spid_tests<std_shared_graph_tagged>(tests);
//    tests->add(BOOST_TEST_CASE(always_fail));

    return tests;
}
//--------------------------------
//ChangeLog:
//  2005-06-18 Larry Evans
//    WHAT:
//      1) changed expect_pass to array from specializations of template.
//      2) changed dfs_printer call.
//    WHY:
//      1) Less typing and consequent "clutter".
//      2) dfs_printer interface changed.


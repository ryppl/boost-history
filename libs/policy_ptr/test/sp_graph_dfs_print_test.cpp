//test sp_graph_dfs_print code
#include "sp_graph_dfs_print.hpp"
#include "binary_node.hpp"
#include "sp_types_noop.hpp"
#include <boost/test/unit_test.hpp>

  template
  < class SpTypes
  , binary_node::member_multiplicity Multiplicity
  >
struct sp_node
{
        typedef 
      typename binary_node::binary_container
      < SpTypes
      , Multiplicity
      >
    type
    ;
};

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
    
    {
        src_type src_root;
        sp_type root_sp(src_root.release());
        src_type src_left_child;
        sp_type left_child_sp(src_left_child.release());
        root_sp->left()=left_child_sp;
        sp_graph_dfs_print<node_type> printer;
        printer.visit_sp(root_sp);
    }
}

namespace butf = boost::unit_test_framework;
using namespace boost;

void always_fail(void)
{
    BOOST_CHECK(false);
}

butf::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    butf::test_suite* tests = BOOST_TEST_SUITE("binary_node_iterators tests");
    tests->add(BOOST_TEST_CASE((&simple_test<sp_node<sp_types_noop,binary_node::contains_one> >)));
    tests->add(BOOST_TEST_CASE((&simple_test<sp_node<sp_types_noop,binary_node::contains_two> >)));
    tests->add(BOOST_TEST_CASE(always_fail));

    return tests;
}

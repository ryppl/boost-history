//test smart_ptrs arranged in various types of graphs
#ifndef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_HPP_LJE20050513
#define LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_HPP_LJE20040513
//#define LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
#ifdef  LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
#include "boost/utility/trace_scope.hpp"
#include "sp_graph_dfs_print.hpp"
#endif
namespace boost
{
  namespace binary_node_graph_tests
  {
      enum
    test_ids
    { simple_list
    , cycle_1_external
    , cycle_2_external
    , num_tests
    };
    
        typedef
      void(*
    test_fun_type
      )(void)
    ;
    
      template
      < typename BinaryNode
      >
      struct
    no_ops
    {
            static
          void
        view(void)
        {}
        
            static
          void
        in_scope(void)
        {}
        
            static
          void
        out_scope(void)
        {}
    };
    
    template
      < typename BinaryNode
      , template<typename>class Ops=no_ops
      >
      class 
    test
    {
     public:
          static
        test_fun_type
      vec[num_tests]
      ;
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
          static 
        void 
      simple_list(void)
      //Form a simple list from parent2children.
      {
      
        #if defined(UTILITY_TRACE_SCOPE_HPP)
          utility::trace_scope ts("simple_list");
        #endif
          {
              src_type sN1;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just created sN1\n";
            #endif
              sp_type pN(sN1.release());
              src_type sN2;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just created sN2\n";
            #endif
              sp_type pN2(sN2.release());
              pN->right() = pN2;
              Ops<node_type>::the_roots().clear();
              Ops<node_type>::the_roots().push_back(&pN);
              Ops<node_type>::view();
              Ops<node_type>::in_scope();
          }//exit inner block
          Ops<node_type>::out_scope();
      }//exit list_test
      
          static 
        void 
      simple_cycle(sp_type* outer_sp=0)
      //Simple cycle with either:
      //  1 external reference (when outer_sp==0)
      //or:
      //  2 external references (when outer_sp!= 0)
      {
      
        #if defined(UTILITY_TRACE_SCOPE_HPP) && defined(LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE)
          utility::trace_scope ts("outer block");
        #endif
          {

            #if defined(UTILITY_TRACE_SCOPE_HPP) && defined(LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE)
              utility::trace_scope ts("inner block");
            #endif
              src_type sN1;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just created sN1\n";
            #endif
              sp_type pN(sN1.release());
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just created pN\n";
            #endif
              if(outer_sp) *outer_sp = pN;
              src_type sN2;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just created sN2\n";
            #endif
              sp_type pN2(sN2.release());
              pN->right() = pN2;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just assigned pN->right()\n";
            #endif
              pN->right()->right() = pN;
            #ifdef LIBS_POLICY_PTR_TEST_BINARY_NODE_GRAPH_TESTS_TRACE
              mout()<<"just assigned pN->right()->right()\n";
              sp_graph_dfs_print<node_type> printer;
              mout()<<"print from external_1:\n";
              printer.visit_sp(pN);
              if(outer_sp)
              {
                  mout()<<"print from external_2:\n";
                  printer.clear_visited_vertices();
                  printer.visit_sp(pN);
              }
            #endif
              Ops<node_type>::the_roots().clear();
              Ops<node_type>::the_roots().push_back(&pN);
              Ops<node_type>::the_roots().push_back(outer_sp);
              Ops<node_type>::view();
          }//exit inner block
          
      }//exit simple_cycle(..sp_type*)
      
          static 
        void 
      simple_cycle_1_external_sp(void)
      {

        #if defined(UTILITY_TRACE_SCOPE_HPP)
          utility::trace_scope ts("simple_cycle_1_external_sp");
        #endif
          simple_cycle(0);
          Ops<node_type>::out_scope();
      }
      
          static 
        void 
      simple_cycle_2_external_sp(void)
      {
        #if defined(UTILITY_TRACE_SCOPE_HPP)
          utility::trace_scope ts("simple_cycle_2_external_sp");
        #endif
          {
              sp_type outer_sp;
              simple_cycle(&outer_sp);
          }
          Ops<node_type>::out_scope();
      }

    };//end test template class
    
    template
      < typename BinaryNode
      , template<typename>class Ops
      >
        test_fun_type
    test<BinaryNode,Ops>::
      vec[num_tests]=
      { test<BinaryNode,Ops>::simple_list
      , test<BinaryNode,Ops>::simple_cycle_1_external_sp
      , test<BinaryNode,Ops>::simple_cycle_2_external_sp
      }
      ;
  }//exit binary_node_graph_tests namespace
  
}//exit boost namespace
#endif

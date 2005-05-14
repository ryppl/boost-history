//test smart_ptrs arranged in various types of graphs
#ifndef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_HPP_LJE20040129
#define LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_HPP_LJE20040129
#include "boost/utility/object_tracked.hpp"
//#define LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
namespace boost
{
  namespace binary_node_tests
  {
    template<typename BinaryNode>
    class test
    {
     public:
              typedef 
          typename BinaryNode::basis_adapt_type
      basis_adapt_type
      ;
              typedef
          typename BinaryNode::sp_type
      sp_type
      ;
              static 
          void 
      simple_list(void)
      //Form a simple list from parent2children.
      {
      
        #if defined(TRACE_SCOPE_HPP) && defined(LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE)
          utility::trace_scope ts("simple_list");
        #endif
          {
              basis_adapt_type iN1(managed_ptr::default_ctor_tag);
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just created iN1\n";
            #endif
              sp_type pN(iN1.as_basis_source());
              basis_adapt_type iN2(managed_ptr::default_ctor_tag);
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just created iN2\n";
            #endif
              pN->right() = iN2.as_basis_source();
  
          }//exit inner block
          
      }//exit list_test
      
              static 
          void 
      simple_cycle(sp_type* outer_sp=0)
      //Simple cycle with either:
      //  1 external reference (when outer_sp==0)
      //or:
      //  2 external references (when outer_sp!= 0)
      {
      
        #if defined(TRACE_SCOPE_HPP) && defined(LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE)
          utility::trace_scope ts("outer block");
        #endif
          {

            #if defined(TRACE_SCOPE_HPP) && defined(LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE)
              utility::trace_scope ts("inner block");
            #endif
              basis_adapt_type iN1(managed_ptr::default_ctor_tag);
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just created iN1\n";
            #endif
              sp_type pN(iN1.as_basis_source());
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just created pN\n";
            #endif
              if(outer_sp) *outer_sp = pN;
              basis_adapt_type iN2(managed_ptr::default_ctor_tag);
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just created iN2\n";
            #endif
              pN->right() = iN2.as_basis_source();
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just assigned pN->right()\n";
            #endif
              pN->right()->right() = pN;
            #ifdef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE
              mout()<<"just assigned pN->right()->right()\n";
            #endif
  
          }//exit inner block
          
      }//exit simple_cycle(..sp_type*)
      
              static 
          void 
      simple_cycle_1_external_sp(void)
      {

        #if defined(TRACE_SCOPE_HPP) && defined(LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE)
          utility::trace_scope ts("simple_cycle_1_external_sp");
        #endif
          simple_cycle(0);
      }
      
              static 
          void 
      simple_cycle_2_external_sp(void)
      {
        #if defined(TRACE_SCOPE_HPP) && defined(LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_TRACE)
          utility::trace_scope ts("simple_cycle_2_external_sp");
        #endif
          sp_type outer_sp;
          simple_cycle(&outer_sp);
      }
      
    };//end test template class
    
  }//exit binary_node_tests namespace
  
}//exit boost namespace
#endif

//test smart_ptrs arranged in various types of graphs
#ifndef LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_HPP_LJE20040129
#define LIBS_MANAGED_PTR_TEST_BINARY_NODE_TESTS_HPP_LJE20040129
#include "boost/utility/object_tracked.hpp"
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
      
          utility::trace_scope ts("simple_list");
          {
              basis_adapt_type iN1(managed_ptr::default_ctor_tag);
              mout()<<"just created iN1\n";
              sp_type pN(iN1.as_basis_source());
              basis_adapt_type iN2(managed_ptr::default_ctor_tag);
              mout()<<"just created iN2\n";
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
      
          utility::trace_scope ts("outer block");
          {
              utility::trace_scope ts("inner block");
              basis_adapt_type iN1(managed_ptr::default_ctor_tag);
              mout()<<"just created iN1\n";
              sp_type pN(iN1.as_basis_source());
              mout()<<"just created pN\n";
              if(outer_sp) *outer_sp = pN;
              basis_adapt_type iN2(managed_ptr::default_ctor_tag);
              mout()<<"just created iN2\n";
              pN->right() = iN2.as_basis_source();
              mout()<<"just assigned pN->right()\n";
              pN->right()->right() = pN;
              mout()<<"just assigned pN->right()->right()\n";
  
          }//exit inner block
          
      }//exit simple_cycle(..sp_type*)
      
              static 
          void 
      simple_cycle_1_external_sp(void)
      {
          utility::trace_scope ts("simple_cycle_1_external_sp");
          simple_cycle(0);
      }
      
              static 
          void 
      simple_cycle_2_external_sp(void)
      {
          utility::trace_scope ts("simple_cycle_2_external_sp");
          sp_type outer_sp;
          simple_cycle(&outer_sp);
      }
      
    };//end test template class
    
            typedef
        void(*
    test_fun_type
        )(void)
    ;
    
    template
        < test_fun_type TestFun
        >
            static
        void
    run(void)
    {
        utility::trace_scope ts("binary_node_tests::run<TestFun>");
        utility::object_tracked::our_members.reset();
        (*TestFun)(); //Assumes that TestFun uses objects derived from object_tracked
        unsigned n_members = utility::object_tracked::our_members.size();
        BOOST_CHECK_EQUAL(n_members, 0u);
    }
    
  }//exit binary_node_tests namespace
  
}//exit boost namespace
#endif

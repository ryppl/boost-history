#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "boost/utility/object_tracked.hpp"
#include "boost/utility/trace_scope.hpp"
#include "boost/grammar_pipeline/eff/productions.hpp"
using namespace boost::grammar_pipeline;


  void
compositions_test(void)
{
    {
        utility::trace_scope ts("arith_expr");
        //The following is based on [Lew82,pp.114-117]
        //(See boost/grammar_pipeline/grammar_pipeline_prefix.hpp)
        enum enum_inp
          { inp_id_a
          , inp_id_b
          , inp_op_add
          , inp_op_mult
          , inp_op_paren_open
          , inp_op_paren_close
          , num_inp
          };
        char const* inp_names[num_inp]=
          { "a"
          , "b"
          , "+"
          , "*"
          , "("
          , ")"
          };
        enum enum_var
          { var_e
          , var_t
          , var_f
          , num_var
          };
        char const* var_names[num_var]=
          { "E"
          , "T"
          , "F"
          };
        {
            utility::trace_scope ts("input bit positions");
            for(unsigned i=0; i<num_inp; ++i)
            {
                eff::input_set<num_inp> inp_set;
                inp_set.set(i);
                mout()<<inp_names[i]<<"="<<inp_set<<"\n";
            }
        }
        base::productions base_prods(num_var);
        typedef sole_ptr<base::exp_tree::top    > top_ptr_type;
        typedef base::exp_tree::node<input      > inp_type;
        typedef base::exp_tree::node<variable   > var_type;
        typedef base::exp_tree::node<product    > prd_type;
        typedef base::exp_tree::node<sum        > sum_type;
        typedef base::exp_tree::node<repeat_ge0 > rep_type;
        {
          top_ptr_type& l_rhs=base_prods[var_f];
          l_rhs=new sum_type;
          l_rhs->push_back(new inp_type(inp_id_a));
          l_rhs->push_back(new inp_type(inp_id_b));
          l_rhs->push_back(new prd_type);
          {
             top_ptr_type& l_prod = l_rhs->back();
             l_prod->push_back(new inp_type(inp_op_paren_open));
             l_prod->push_back(new var_type(var_e));
             l_prod->push_back(new inp_type(inp_op_paren_close));
          }
        }
        {
          top_ptr_type& l_rhs=base_prods[var_t];
          l_rhs=new prd_type;
          l_rhs->push_back(new var_type(var_f));
          {
             prd_type*l_prod = new prd_type;
             l_prod->push_back(new inp_type(inp_op_mult));
             l_prod->push_back(new var_type(var_f));
             l_rhs->push_back(new rep_type(l_prod));
          }
        }
        {
          top_ptr_type& l_rhs=base_prods[var_e];
          l_rhs=new prd_type;
          l_rhs->push_back(new var_type(var_t));
          {
             prd_type*l_prod = new prd_type;
             l_prod->push_back(new inp_type(inp_op_add));
             l_prod->push_back(new var_type(var_t));
             l_rhs ->push_back(new rep_type(l_prod));
          }
        }
        eff::productions<num_inp> eff_prods(base_prods);
        {
            unsigned not_solved=eff_prods.solve_all(9);
            BOOST_TEST(not_solved == 0);
        }
        {   
              bool 
            expect_empty = false;
              eff::input_set<num_inp> 
            expect_first;
            expect_first
              .set(inp_id_a)
              .set(inp_id_b)
              .set(inp_op_paren_open)
              ;
              eff::input_set<num_inp> 
            expect_follow;
            expect_follow
              .set(inp_op_add)
              .set(inp_op_mult)
              .set(inp_op_paren_close)
              ;
              bool 
            actual_empty =eff_prods[var_f].my_lhs_attributes
                  .get_attr_mut<eff::attr_empty >();
              eff::input_set<num_inp> 
            actual_first =eff_prods[var_f].my_lhs_attributes
                  .get_attr_mut<eff::attr_first >();
              eff::input_set<num_inp> 
            actual_follow=eff_prods[var_f].my_lhs_attributes
                  .get_attr_mut<eff::attr_follow>();
            BOOST_TEST(expect_empty  == actual_empty );
            BOOST_TEST(expect_first  == actual_first );
            BOOST_TEST(expect_follow == actual_follow);
        }
        {   
              bool 
            expect_empty = false;
              eff::input_set<num_inp> 
            expect_first;
            expect_first
              .set(inp_id_a)
              .set(inp_id_b)
              .set(inp_op_paren_open)
              ;
              eff::input_set<num_inp> 
            expect_follow;
            expect_follow
              .set(inp_op_paren_close)
              ;
              bool 
            actual_empty =eff_prods[var_e].my_lhs_attributes
                  .get_attr_mut<eff::attr_empty >();
              eff::input_set<num_inp> 
            actual_first =eff_prods[var_e].my_lhs_attributes
                  .get_attr_mut<eff::attr_first >();
              eff::input_set<num_inp> 
            actual_follow=eff_prods[var_e].my_lhs_attributes
                  .get_attr_mut<eff::attr_follow>();
            BOOST_TEST(expect_empty  == actual_empty );
            BOOST_TEST(expect_first  == actual_first );
            BOOST_TEST(expect_follow == actual_follow);
        }
    }
    BOOST_TEST(utility::object_tracked::members_size() == 0);
}    
  int
main(void)
{
    compositions_test();
    return boost::report_errors();
}

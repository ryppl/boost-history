#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "boost/utility/object_tracked.hpp"
#include "boost/utility/trace_scope.hpp"
#include "boost/grammar_pipeline/base/prt_productions.hpp"
using namespace boost::grammar_pipeline;


  void
compositions_test(void)
{
    {
        utility::trace_scope ts("compositions");
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
          { "inp_id_a"
          , "inp_id_b"
          , "inp_op_add"
          , "inp_op_mult"
          , "inp_op_paren_open"
          , "inp_op_paren_close"
          };
        enum enum_var
          { var_e
          , var_t
          , var_f
          , num_var
          };
        char const* var_names[num_var]=
          { "var_e"
          , "var_t"
          , "var_f"
          };
        base::productions prods(num_var);
        typedef sole_ptr<base::exp_tree::top   > top_ptr_type;
        typedef base::exp_tree::node<input     > inp_type;
        typedef base::exp_tree::node<variable  > var_type;
        typedef base::exp_tree::node<product   > prd_type;
        typedef base::exp_tree::node<sum       > sum_type;
        typedef base::exp_tree::node<repeat_ge0> rep_type;
        {
          top_ptr_type& l_rhs=prods[var_f];
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
          top_ptr_type& l_rhs=prods[var_t];
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
          top_ptr_type& l_rhs=prods[var_e];
          l_rhs=new prd_type;
          l_rhs->push_back(new var_type(var_t));
          {
             prd_type*l_prod = new prd_type;
             l_prod->push_back(new inp_type(inp_op_add));
             l_prod->push_back(new var_type(var_t));
             l_rhs->push_back(new rep_type(l_prod));
          }
        }
        base::prt_productions(inp_names,var_names,prods);
    }
    BOOST_TEST(utility::object_tracked::members_size() == 0);
}    
  int
main(void)
{
    compositions_test();
    return boost::report_errors();
}

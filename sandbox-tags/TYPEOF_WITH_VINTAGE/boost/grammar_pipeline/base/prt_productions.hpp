//formated print of base::productions
#ifndef BOOST_GRAMMAR_PIPELINE_BASE_PRT_PRODUCTIONS_HPP_LJE20040522
#define BOOST_GRAMMAR_PIPELINE_BASE_PRT_PRODUCTIONS_HPP_LJE20040522
#include "productions.hpp"
#include "boost/io/filters/mout.hpp"

#include "../grammar_pipeline_prefix.hpp"

namespace base
{
  class
prt_productions
  : public exp_tree::visitor 
{
 private:
        typedef 
      sole_ptr<exp_tree::top> 
    top_ptr_type
    ;
 public:
    prt_productions
      ( char const** a_inputs
      , char const** a_variables
      , productions const& a_prods
      )
      : m_inputs(a_inputs)
      , m_variables(a_variables)
    {
        for
          ( unsigned cur=0
          ; cur < a_prods.size()
          ; ++cur
          )
        {
            top_ptr_type const& l_rhs = a_prods[cur];
            mout()<<m_variables[cur]<<"::=\n";
            ++mout();
            l_rhs->accept(this);
            --mout();
            mout()<<"\n";
        }
    }
    
      void
    visit( exp_tree::node<input>*a_node)
    {
        mout()<<m_inputs[a_node->id()]<<"\n";
    }

      void
    visit( exp_tree::node<variable>*a_node)
    {
        mout()<<m_variables[a_node->id()]<<"\n";
    }

      void
    visit( exp_tree::node<zero>*a_node)
    {
        mout()<<"<zero>\n";
    }

      void
    visit( exp_tree::node<one>*a_node)
    {
        mout()<<"<one>\n";
    }

      void
    visit( exp_tree::node<sum>*a_node)
    {
        mout()<<"<sum>\n";
        prt_list(a_node);
    }

      void
    visit( exp_tree::node<product>*a_node)
    {
        mout()<<"<product>\n";
        prt_list(a_node);
    }

      void
    visit( exp_tree::node<closure>*a_node)
    {
        mout()<<"<closure>\n";
        prt_list(a_node);
    }
 private:
      void
    prt_list(std::list<top_ptr_type>* a_list)
    {
        typedef std::list<top_ptr_type>::iterator iter_type;
        iter_type l_cur=a_list->begin();
        iter_type l_end=a_list->end();
        unsigned l_cnt=0;
        mout()<<"( ";
        for(;l_cur!=l_end;++l_cur,++l_cnt)
        {
            if(l_cnt)
            {
                mout()<<", ";
            }
            top_ptr_type& l_top=*l_cur;
            ++mout();
            l_top->accept(this);
            --mout();
        }
        mout()<<")\n";
    }
      char const**
    m_inputs
    ;       
      char const**
    m_variables
    ;
};

}//exit base namespace

#include "../grammar_pipeline_suffix.hpp"

#endif

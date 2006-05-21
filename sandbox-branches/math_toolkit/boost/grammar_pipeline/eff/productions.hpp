//decorate grammar exp's with empty,first,follow attributes
#ifndef BOOST_GRAMMAR_PIPELINE_EFF_PRODUCTIONS_HPP_LJE20040523
#define BOOST_GRAMMAR_PIPELINE_EFF_PRODUCTIONS_HPP_LJE20040523
//  (C) Copyright Larry Joe Evans 2004.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "../base/productions.hpp"
#include "boost/dyn_inherit/inherit.hpp"
#include <bitset>    
#include "../grammar_pipeline_prefix.hpp"
//^above include defines teminology and
// references

namespace eff
//Purpose:
//  Provide classes for calculating:
//    empty
//    first
//    follow
//  "lookahead" attributes for grammar symbols.
//  [Lew82] defines these terms and describes the
//  algorithm used here for calculating these
//  attributes.
{

template
  < unsigned NumInputs
  >
  class
input_set
  : public std::bitset<NumInputs>
{};

#ifdef BOOST_IO_FILTERS_MARG_OSTREAM_HPP
template
  < unsigned NumInputs
  >
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , input_set<NumInputs>const& a_inp_set
  )
{
    return sout.super_insert(a_inp_set);
}
#endif

  enum
lookahead_enum
{ attr_empty
, attr_first
, attr_follow
};

template
  < unsigned NumInputs
  , lookahead_enum Atype
  >
  struct
lookahead_types
{
      typedef
    input_set<NumInputs>
  attr_type
  ;
};

template
  < unsigned NumInputs
  >
  struct
lookahead_types
  < NumInputs
  , attr_empty
  >
{
      typedef
    bool
  attr_type
  ;
};

template
  < unsigned NumInputs
  >
  class
lookahead_attributes
{
 public:
    lookahead_attributes(void)
      : my_empty(false)
    {}
    
      input_set<NumInputs>
    direction(void)const
    //Purpose:
    //  Implements "Definition of DIRSYMB"
    //  at [Lew82,p.113]
    {
        input_set<NumInputs> l_dir = this->template get_attr_con<attr_first>();
        if(this->template get_attr_con<attr_empty>())
        {
            l_dir |= this->template get_attr_con<attr_follow>();
        }
        return l_dir;
    }
   
    template
      < lookahead_enum AttrEnum
      , unsigned NestNumInputs=NumInputs
      >
      class
    attr_getter
    //Purpose:
    //  Specializations allow returning
    //  the value of the AttrEnum
    //  attribute from a node.
    ;
      
    template
      < unsigned NestNumInputs
      >
      class
    attr_getter
      < attr_empty
      , NestNumInputs
      >
    {
     public:
            static
          typename lookahead_types<NestNumInputs,attr_empty>::attr_type const&
        get_con
          ( lookahead_attributes<NestNumInputs> const* a_attributes
          )
        {
            return a_attributes->my_empty;
        }
            static
          typename lookahead_types<NestNumInputs,attr_empty>::attr_type&
        get_mut
          ( lookahead_attributes<NestNumInputs>* a_attributes
          )
        {
            return a_attributes->my_empty;
        }
    };
      
    template
      < unsigned NestNumInputs
      >
      class
    attr_getter
      < attr_first
      , NestNumInputs
      >
    {
     public:
            static
          typename lookahead_types<NestNumInputs,attr_first>::attr_type const&
        get_con
          ( lookahead_attributes<NestNumInputs> const* a_attributes
          )
        {
            return a_attributes->my_first;
        }
            static
          typename lookahead_types<NestNumInputs,attr_first>::attr_type&
        get_mut
          ( lookahead_attributes<NestNumInputs>* a_attributes
          )
        {
            return a_attributes->my_first;
        }
    };
      
    template
      < unsigned NestNumInputs
      >
      class
    attr_getter
      < attr_follow
      , NestNumInputs
      >
    {
     public:
            static
          typename lookahead_types<NestNumInputs,attr_follow>::attr_type const&
        get_con
          ( lookahead_attributes<NestNumInputs> const* a_attributes
          )
        {
            return a_attributes->my_follow;
        }
            static
          typename lookahead_types<NestNumInputs,attr_follow>::attr_type&
        get_mut
          ( lookahead_attributes<NestNumInputs>* a_attributes
          )
        {
            return a_attributes->my_follow;
        }
    };
    
    template
      < lookahead_enum AttrEnum
      >
      typename lookahead_types<NumInputs,AttrEnum>::attr_type const&
    get_attr_con(void)const
    {
        return attr_getter<AttrEnum,NumInputs>::get_con(this);
    }
      
    template
      < lookahead_enum AttrEnum
      >
      typename lookahead_types<NumInputs,AttrEnum>::attr_type&
    get_attr_mut(void)
    {
        return attr_getter<AttrEnum,NumInputs>::get_mut(this);
    }
    
        friend
      class
    attr_getter<attr_empty,NumInputs>
    ;
        friend
      class
    attr_getter<attr_first,NumInputs>
    ;
        friend
      class
    attr_getter<attr_follow,NumInputs>
    ;

 private:          
      typename lookahead_types<NumInputs,attr_empty>::attr_type
    my_empty
    ;
      typename lookahead_types<NumInputs,attr_first>::attr_type
    my_first
    ;
      typename lookahead_types<NumInputs,attr_follow>::attr_type
    my_follow
    ;
   
};//end lookahead_attributes<NumInputs> class
  
  namespace
exp_tree
//Purpose:
//  Define a class heirarchy parallel to that
//  of base::exp_tree but where each node has
//  the additional attributes in
//  lookahead_attributes<NumInputs>.
{
 
    template
      < unsigned NumInputs
      >
      class
    top
      : public dyn_inherit::inherit
        < base::exp_tree::top
        , lookahead_attributes<NumInputs>
        >
    {
     public:
     
            typedef
          base::exp_tree::top
        dyn_super_type
        ;
            typedef
          lookahead_attributes<NumInputs>
        lookahead_super_type
        ;
            typedef
          dyn_inherit::inherit
          < dyn_super_type
          , lookahead_super_type
          >
        top_super_type
        ;
            
        top(dyn_super_type* a_dyn_super)
          : top_super_type( a_dyn_super)
        {}
        
            static
          top*
        get_subtype
        ( dyn_super_type* a_super
        )
        {
            top_super_type*l_super=top_super_type::get_subtype(a_super);
            top*l_tp=static_cast<top*>(l_super);
            return l_tp;
        }
        
          exp_types
        exp_type(void)const
        {
            return this->get_supertype()->exp_type();
        }
        
          class
        iterator
          : public base::exp_tree::children::iterator
        {
         public:
                typedef
              base::exp_tree::children::iterator
            iter_super_type
            ;
            iterator(iter_super_type const&a_super_iter)
              : iter_super_type(a_super_iter)
            {}
              top*
            operator*(void)
            {
                sole_ptr<dyn_super_type>&l_dsup_sptr=iter_super_type::operator*();
                dyn_super_type*l_dsup_rptr=l_dsup_sptr.get();
                top* l_top = top::get_subtype(l_dsup_rptr);
                return l_top;
            }
        };
        
          iterator
        begin(void)
        {
            iterator l_iter(this->get_supertype()->begin());
            return l_iter;
        }
        
          iterator
        end(void)
        {
            iterator l_iter(this->get_supertype()->end());
            return l_iter;
        }
        
          class
        reverse_iterator
          : public base::exp_tree::children::reverse_iterator
        {
         public:
                typedef
              base::exp_tree::children::reverse_iterator
            iter_super_type
            ;
            reverse_iterator(iter_super_type const&a_super_iter)
              : iter_super_type(a_super_iter)
            {}
              top*
            operator*(void)
            {
                sole_ptr<dyn_super_type>&l_dsup_sptr=iter_super_type::operator*();
                dyn_super_type*l_dsup_rptr=l_dsup_sptr.get();
                top* l_top = top::get_subtype(l_dsup_rptr);
                return l_top;
            }
        };
        
          reverse_iterator
        rbegin(void)
        {
            reverse_iterator l_iter(this->get_supertype()->rbegin());
            return l_iter;
        }
        
          reverse_iterator
        rend(void)
        {
            reverse_iterator l_iter(this->get_supertype()->rend());
            return l_iter;
        }
        
            virtual
          void
        eval_node_empty(void)
        =0
        ;
            virtual
          void
        eval_node_first(void)
        =0
        ;
            virtual
          void
        eval_tree_follow(input_set<NumInputs>const&)
        =0
        ;
      
        template
          < lookahead_enum
          >
          void
        eval_tree
          ( void
          )
        ;

     protected:
          void
        put_follow
          ( input_set<NumInputs>const & a_follow
          )
        {
            lookahead_super_type::template get_attr_mut<attr_follow>() = a_follow;
        }
      
    };//end exp_tree::top<NumInputs> class
    
    template
      < unsigned NumInputs
      , lookahead_enum AttrEnum
      >
      class
    node_evaluator
    //Purpose:
    //  Specializations allow selection
    //  of eval_node_xxx based on value
    //  of AttrEnum in {attr_empty,attr_first}.
    ;
      
    template
      < unsigned NumInputs
      >
      class
    node_evaluator
      < NumInputs
      , attr_empty
      >
    {
     public:
            static
          void
        evaluate
          ( top<NumInputs>* a_top
          )
        {
            a_top->eval_node_empty();
        }
    };
      
    template
      < unsigned NumInputs
      >
      class
    node_evaluator
      < NumInputs
      , attr_first
      >
    {
     public:
            static
          void
        evaluate
          ( top<NumInputs>* a_top
          )
        {
            a_top->eval_node_first();
        }
    };
      
    template
      < unsigned NumInputs
      >
        template
          < lookahead_enum AttrEnum
          >
          void
    top<NumInputs>::
        eval_tree
          ( void
          )
        //Purpose:
        //  Perform postfix traversal of expression
        //  tree and at each node, do
        //  node_evaluator<...,AttrEnum>,
        //  for AttrEnum in {attr_empty, attr_first}.
        {
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top* l_top=*l_cur;
                l_top->template eval_tree<AttrEnum>();
            }
            node_evaluator<NumInputs,AttrEnum>::evaluate(this);
                
        }
        
    template
      < unsigned NumInputs
      >
      class
    symbol
    //Purpose:
    //  Represents "symbols", i.e.
    //  inputs and variables.
    : public top<NumInputs>
    {
     public:
            typedef
          top<NumInputs>
        super_type
        ;
            typedef
          base::exp_tree::symbol
        dyn_super_type
        ;
            typedef
          dyn_super_type::id_type
        id_type
        ;
        symbol
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
          dyn_super_type const*
        get_supertype(void)const
        {
          typename super_type::dyn_super_type const*super_top=super_type::get_supertype();
          return static_cast<dyn_super_type const*>(super_top);
        }
          dyn_super_type*
        get_supertype(void)
        {
          typename super_type::dyn_super_type*super_top=super_type::get_supertype();
          return static_cast<dyn_super_type*>(super_top);
        }
          id_type
        id(void)const
        {
            return get_supertype()->id();
        }
    };
    
    template
      < unsigned NumInputs
      , exp_types ExpType
      >
      class
    node
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =ExpType
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
        node(typename super_type::dyn_super_type* a_dyn_super)
          : super_type(a_dyn_super)
        {}

    };//end node<NumInputs,exp_types> general template class
    

    
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , input
      >
      : public symbol<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =input
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          symbol<NumInputs>
        super_type
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
    
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements rule 2) in "Definition of EMPTY"
        //  at [Lew82,p.114] 
        {
            this->template get_attr_mut<attr_empty>() = false;
        }
        
          void
        eval_node_first(void)
        //Purpose:
        //  Implements rule 2) in "Definition of FIRST"
        //  at [Lew82,p.113] 
        {
            typename super_type::id_type l_id=this->id();
            super_type::template get_attr_mut<attr_first>().set(l_id);
        }
        
          void
        eval_tree_follow(input_set<NumInputs>const& a_follow)
        {
            this->put_follow(a_follow);
        }
        
    };//end node<NumInputs,input> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , variable
      >
      : public symbol<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =variable
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          symbol<NumInputs>
        super_type
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          , lookahead_attributes<NumInputs>& a_lhs_attributes
          )
          : super_type(a_dyn_super)
          , my_lhs_attributes(a_lhs_attributes)
        {}
    
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements "half" of Axiomatic rule 2') for
        //  Empty in [Lew82,p. 115].
        //
        //  The other half is implemented by 
        //  var_evaluator<NumInputs,attr_empty>::evaluate.
        {
            super_type::template get_attr_mut<attr_empty>() 
              = my_lhs_attributes.template get_attr_con<attr_empty>();
        }
     
          void
        eval_node_first(void)
        //Purpose:
        //  Implements "half" of Axiomatic rule 2') for
        //  First in [Lew82,p. 113].
        //
        //  The other half is implemented by 
        //  var_evaluator<NumInputs,attr_first>::evaluate.
        {
            super_type::template get_attr_mut<attr_first>() 
              = my_lhs_attributes.template get_attr_con<attr_first>();
        }
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        //Purpose:
        //  Implements "half" of rule 4) under 
        //  "Definition of FOLLOW" in [Lew82,p.116].
        //
        //  The other half is implemented by 
        //  var_evaluator<NumInputs,attr_follow>::evaluate.
        {
            this->put_follow(a_follow);
            input_set<NumInputs>& lhs_follow =
              my_lhs_attributes.template get_attr_mut<attr_follow>();
            lhs_follow |= a_follow;
        }
        
     private:
          lookahead_attributes<NumInputs>& 
        my_lhs_attributes
        //Reference to var_definition<NumInputs>::my_lhs_attributes
        ;
    
    };//end node<NumInputs,variable> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , zero
      >
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =zero
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
        
        //Since there [Lew82] makes no mention of zero,
        //the following eval_xxx_yyy methods are
        //really just to enable construction:
    
          void
        eval_node_empty(void)
        {
            super_type::template get_attr_mut<attr_empty>() 
              = false;
        }
     
          void
        eval_node_first(void)
        {}
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        {}
        
    };//end node<NumInputs,zero> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , one
      >
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =one
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
        
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements rule 1) in "Definition of EMPTY"
        //  at [Lew82,p.114] 
        {
            super_type::template get_attr_mut<attr_empty>() 
              = true;
        }
     
          void
        eval_node_first(void)
        //Purpose:
        //  Implements rule 1) in "Definition of FIRST"
        //  at [Lew82,p.114].
        //  I.e. does nothing since default value for
        //  first is empty set.
        {}
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        {
            this->put_follow(a_follow);
        }
        
    };//end node<NumInputs,one> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , sum
      >
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =sum
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
            typedef
          typename top<NumInputs>::iterator
        iterator
        ;
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
    
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements rule 3) in "Definition of EMPTY"
        //  at [Lew82,p.114] 
        {
            typename lookahead_types<NumInputs,attr_empty>::attr_type
              l_attr=false;
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                l_attr = l_top->template get_attr_con<attr_empty>();
                if(l_attr)break;
            }
            this->template get_attr_mut<attr_empty>() = l_attr;
        }
     
          void
        eval_node_first(void)
        //Purpose:
        //  Implements rule 3) in "Definition of FIRST"
        //  at [Lew82,p.113] 
        {
            typename lookahead_types<NumInputs,attr_first>::attr_type
              l_attr;
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                l_attr |= l_top->template get_attr_con<attr_first>();
            }
            this->template get_attr_mut<attr_first>() = l_attr;
        }
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        //Purpose:
        //  Implements rule 1) in "Definition of FOLLOW"
        //  at [Lew82,p.116].
        {
            this->put_follow(a_follow);
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                l_top->eval_tree_follow(a_follow);
            }
        }
        
    };//end node<NumInputs,sum> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , product
      >
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =product
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
            typedef
          typename top<NumInputs>::iterator
        iterator
        ;
            typedef
          typename top<NumInputs>::reverse_iterator
        reverse_iterator
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {}
    
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements rule 3 in "Definition of EMPTY"
        //  at [Lew82,p.114].
        {
            typename lookahead_types<NumInputs,attr_empty>::attr_type
              l_attr=true;
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                l_attr = l_top->template get_attr_con<attr_empty>();
                if(!l_attr)break;
            }
            this->template get_attr_mut<attr_empty>() = l_attr;
        }
     
          void
        eval_node_first(void)
        //Purpose:
        //  Implements rule 3 in "Definition of FIRST"
        //  at [Lew82,p.113].
        {
            typename lookahead_types<NumInputs,attr_first>::attr_type
              l_attr;
            iterator l_end=this->end();
            for
              ( iterator l_cur=this->begin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                l_attr |= l_top->template get_attr_con<attr_first>();
                if(!(l_top->template get_attr_con<attr_empty>()))break;
            }
            this->template get_attr_mut<attr_first>() = l_attr;
        }
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        //Purpose:
        //  Implements rule 2) in "Definition of FOLLOW"
        //  at [Lew82,p.116].
        {
            this->put_follow(a_follow);
            reverse_iterator l_end=this->rend();
            top<NumInputs>* l_follow=0;
            for
              ( reverse_iterator l_cur=this->rbegin()
              ; l_cur != l_end
              ; ++l_cur
              )
            {
                top<NumInputs>* l_top = *l_cur;
                if(l_follow)
                {
                    input_set<NumInputs> l_dir = l_follow->direction();
                    l_top->eval_tree_follow(l_dir);
                }
                else
                {
                    l_top->eval_tree_follow(a_follow);
                }
                l_follow=l_top;
            }
        }
      
    };//end node<NumInputs,product> specific template class
        
    template
      < unsigned NumInputs
      >
      class
    node
      < NumInputs
      , repeat_ge0
      >
      : public top<NumInputs>
    {
     public:
            static
          exp_types const
        our_exp_type
        =repeat_ge0
        ;
            typedef
          base::exp_tree::node<our_exp_type>
        dyn_super_type
        ;
            typedef
          top<NumInputs>
        super_type
        ;
            typedef
          typename top<NumInputs>::iterator
        iterator
        ;
            typedef
          typename top<NumInputs>::reverse_iterator
        reverse_iterator
        ;
            
        node
          ( dyn_super_type* a_dyn_super
          )
          : super_type(a_dyn_super)
        {
            this->template get_attr_mut<attr_empty>() = true;
        }
    
          void
        eval_node_empty(void)
        //Purpose:
        //  Implements rule 5) in "Definition of EMPTY"
        //  at [Lew82,p.114] 
        {
          //The value is constant and already set by CTOR.
        }
     
          void
        eval_node_first(void)
        //Purpose:
        //  Implements rule 5) in "Definition of FIRST"
        //  at [Lew82,p.113] 
        {
            top<NumInputs>* l_top = *(this->begin());
            this->template get_attr_mut<attr_first>() 
              = l_top->template get_attr_con<attr_first>();
        }
        
          void
        eval_tree_follow(input_set<NumInputs> const& a_follow)
        //Purpose:
        //  Implements rule 3) in "Definition of FOLLOW"
        //  at [Lew82,p.116].
        {
            this->put_follow(a_follow);
            top<NumInputs>* l_top = *(this->begin());
            input_set<NumInputs> l_follow
              = l_top->template get_attr_con<attr_first>();
            l_follow |= a_follow;
            l_top->eval_tree_follow(l_follow);
        }
    };//end node<NumInputs,repeat_ge0> specific template class
        
}//exp exp_tree namespace

template
  < unsigned NumInputs
  >
  class
var_definition
//Purpose:
//  Store data used for iteratively solving
//  for lookahead_attributes of a grammar 
//  variable, i.e. exp_tree<NumInputs>::exp<variable>.
{
 public:
        typedef
      exp_tree::top<NumInputs>
    rhs_type
    ;
        typedef
      std::vector<var_definition>
    var_def_vec
    //Only purpose is for use later in
    //productions class.
    ;
      void
    put_rhs(rhs_type* a_rhs)
    {
        my_rhs_expression=a_rhs;
    }
      rhs_type*
    my_rhs_expression
    //The "right-hand-side" of production
    ;
      lookahead_attributes<NumInputs>
    my_lhs_attributes
    //Variable attributes
    ;
  
};
  
template
  < unsigned NumInputs
  >
  class
tree_constructor
: public grammar_pipeline::base::exp_tree::visitor
//Purpose:
//  "Decorate/Wrap" a tree of with nodes in:
//     base::exp_tree::node<exp_types>
//  with corresponding instances of:
//     eff::exp_tree::node<exp_types>
{
 public:
        typedef
      base::exp_tree::top
    super_heirarchy_top_type
    ;
        typedef
      exp_tree::top<NumInputs>
    this_heirarchy_top_type
    ;
    
    tree_constructor
      ( super_heirarchy_top_type* a_super_top
      , typename var_definition<NumInputs>::var_def_vec& a_prods
      )
      : my_super_heirarchy_top_root(a_super_top)
      , my_productions(a_prods)
    {        
        a_super_top->accept(this);
    }
    
      void
    visit(base::exp_tree::node<zero>*a_tree)
    {
        construct_parent(a_tree);
    }

      void
    visit(base::exp_tree::node<one>*a_tree)
    {
        construct_parent(a_tree);
    }

      void
    visit(base::exp_tree::node<input>*a_tree)
    {
        construct_parent(a_tree);
    }

      void
    visit(base::exp_tree::node<variable>*a_tree)
    {
        base::exp_tree::symbol::id_type l_id=
          a_tree->id();
        var_definition<NumInputs>& l_vsd=my_productions[l_id];
        lookahead_attributes<NumInputs>& l_laa=l_vsd.my_lhs_attributes;
        construct_parent(a_tree, l_laa);
    }

      void
    visit(base::exp_tree::node<product>*a_tree)
    {
        construct_parent(a_tree);
        construct_children(a_tree);
    }

      void
    visit(base::exp_tree::node<sum>*a_tree)
    {
        construct_parent(a_tree);
        construct_children(a_tree);
    }

      void
    visit(base::exp_tree::node<repeat_ge0>*a_tree)
    {
        construct_parent(a_tree);
        construct_children(a_tree);
    }

      this_heirarchy_top_type*
    get_root(void)const
    {
        typename this_heirarchy_top_type::top_super_type*l_topsuper
          = this_heirarchy_top_type::get_subtype(my_super_heirarchy_top_root);
        return static_cast<this_heirarchy_top_type*>(l_topsuper);
    }
    
 private:
    template
      < exp_types ExpType
      >  
      exp_tree::node<NumInputs,ExpType>*
    construct_parent(base::exp_tree::node<ExpType>*a_super)
    {
        typedef exp_tree::node<NumInputs,ExpType> node_type;
        return new node_type(a_super);
    }
      exp_tree::node<NumInputs,variable>*
    construct_parent
      ( base::exp_tree::node<variable>*a_super
      , lookahead_attributes<NumInputs>&a_lhs_attributes
      )
    {
        typedef exp_tree::node<NumInputs,variable> node_type;
        return new node_type(a_super, a_lhs_attributes);
    }
      void
    construct_children(base::exp_tree::children* a_super_heirarchy_children)
    {
        base::exp_tree::children::iterator cur=a_super_heirarchy_children->begin();
        base::exp_tree::children::iterator end=a_super_heirarchy_children->end();
        for(;cur != end; ++cur)
        {
            base::exp_tree::top*l_super_heirarchy_top=(*cur).get();
            l_super_heirarchy_top->accept(this);
        }
    }
    
      super_heirarchy_top_type*
    my_super_heirarchy_top_root
    ;
      typename var_definition<NumInputs>::var_def_vec& 
    my_productions
    ;
};

template
  < unsigned NumInputs
  >
  class
productions
  : public var_definition<NumInputs>::var_def_vec
{
 public:
        typedef
      grammar_pipeline::base::productions
    super_prods_type
    ;
        typedef
      super_prods_type::this_top_type
    super_top_type
    ;
        typedef
      exp_tree::top<NumInputs>
    this_top_type
    ;
        typedef
      typename var_definition<NumInputs>::var_def_vec
    super_type
    ;   
    productions(super_prods_type& a_super_prods)
      : super_type(a_super_prods.size())
    {
        for(unsigned i=0; i<a_super_prods.size(); ++i)
        {
            super_top_type*l_stop_rptr = a_super_prods[i].get();
            tree_constructor<NumInputs> l_ctor(l_stop_rptr,*this);
            this_top_type*l_ttop_rptr = l_ctor.get_root();
            this->operator[](i).put_rhs(l_ttop_rptr);
        }
    }
    
    template
      < lookahead_enum AttrEnum
      >
      unsigned
    attr_fixed_point
      ( unsigned maxit
      )
    //Effects:
    //  Finds "fixed point" of all productions for
    //  attribute, AttrEnum.  IOW, given an assumed
    //  value for the attributes, the attributes
    //  are calculated for all lhs and rhs of the
    //  productions.  If there's no change, the
    //  assumed value is the fixed point.
    ;
      unsigned
    solve_all
      ( unsigned maxit
      )
    //Effects:
    //  given:
    //    n=solve_all(m)
    //  then:
    //    if(n==0)
    //      then all attributes have been successfully
    //      solved for.
    //    else
    //      the (n-1)-th attribute could not be solved
    //      for within m iterations.
    {
        unsigned changed;
        changed=this->attr_fixed_point<attr_empty >(maxit);
        if(changed != 0) return attr_empty+1;
        changed=this->attr_fixed_point<attr_first >(maxit);
        if(changed != 0) return attr_first+1;
        changed=this->attr_fixed_point<attr_follow>(maxit);
        if(changed != 0) return attr_follow+1;
        return 0;
    }
    
     
};

template
  < unsigned NumInputs
  , lookahead_enum AttrEnum
  >
  class
var_evaluator
{
 public:
        static
      void
    evaluate
      ( var_definition<NumInputs>& a_var
      )
    //Purpose:
    //  Implements "half" of Axiomatic rules 2') for
    //  First and Empty in [Lew82,p.113 and p. 115].
    //
    //  The other half is implemented by 
    //  exp_tree::node<NumInputs,variable>::
    //    eval_node_empty
    //    eval_node_first
    {
        exp_tree::top<NumInputs>* l_rhs=a_var.my_rhs_expression;
        l_rhs->template eval_tree<AttrEnum>();
        a_var.my_lhs_attributes.template get_attr_mut<AttrEnum>()
          = l_rhs->template get_attr_con<AttrEnum>();
    }
};

template
  < unsigned NumInputs
  >
  class
var_evaluator
  < NumInputs
  , attr_follow
  >
{
 public:
        static
      void
    evaluate
      ( var_definition<NumInputs>& a_var
      )
    //Purpose:
    //  Implements "half" of rule 4) under 
    //  "Definition of FOLLOW" in [Lew82,p.116].
    //
    //  The other half is implemented by 
    //  exp_tree::node<NumInputs,variable>::eval_tree_follow.
    {
        input_set<NumInputs>const& lhs_follow
          = a_var.my_lhs_attributes.template get_attr_con<attr_follow>();
        a_var.my_rhs_expression->eval_tree_follow(lhs_follow);   
    }
};

//#define TRACE_SOLVEGRAM_SOLVED
template
  < unsigned NumInputs
  >
    template
      < lookahead_enum AttrEnum
      >
       unsigned
productions
  < NumInputs
  >::
    attr_fixed_point
      ( unsigned maxit
      )
    {
    ; unsigned i
    ; unsigned changed
    ; unsigned numit
    ; super_type& var_defs=*this;
    ; unsigned n=var_defs.size()
    #if defined(TRACE_SOLVEGRAM_SOLVED) && defined(TRACE_SCOPE_HPP)
    ; utility::trace_scope ts("attr_fixed_point")
    ; mout()<<":n="<<n<<":attr="<<AttrEnum<<"\n"
    #endif
    ; typedef typename lookahead_types<NumInputs,AttrEnum>::attr_type TYPE
    ; TYPE* pre=new TYPE[n]
      //^Previous values of attribute.
    /* body */
    ; {
      #ifdef TRACE_SOLVEGRAM_SOLVED
      ; utility::trace_scope ts("initialize previous values.")
      #endif
      ; for(i=0; i<n; i++)
        { 
        ; pre[i]=var_defs[i].my_lhs_attributes.template get_attr_con<AttrEnum>()
        #ifdef TRACE_SOLVEGRAM_SOLVED
        ; mout()<<"SolveGram init loop:"<<i<<"\n"
        #endif
        ;}
      ;}
    ; numit=0
    ; {
      #ifdef TRACE_SOLVEGRAM_SOLVED
      ; utility::trace_scope ts("iteratively solve for fixed point.")
      #endif
      ; do
        {
        ; {
          #ifdef TRACE_SOLVEGRAM_SOLVED
          ; utility::trace_scope ts("evaluate the rhs's for attribute")
          #endif
          ; for(i=0; i<n; i++)
            { 
            #ifdef TRACE_SOLVEGRAM_SOLVED
            ; mout()<<"SolveGram iter::eval loop:"<<i<<"\n"
            #endif
            ; var_evaluator<NumInputs,AttrEnum>::evaluate(var_defs[i])
            ;}
          ;}
        ; changed=0
        ; {
          #ifdef TRACE_SOLVEGRAM_SOLVED
          ; utility::trace_scope ts("update previous values while detecting change")
          #endif
          ; for(i=0; i<n; i++)
            {
            ; TYPE const& cur
              = var_defs[i].my_lhs_attributes.template get_attr_con<AttrEnum>()
            ; if(cur!=pre[i])
              {
                  changed++;
                  pre[i]=cur;
              }
            #ifdef TRACE_SOLVEGRAM_SOLVED
            ; mout()<<"SolveGram iter::update loop:"<<i<<":changed="<<changed<<"\n"
            #endif
            ;}
          ;}
        ;}
        while(++numit<maxit && changed)
      ;}
    ; delete[] pre
    #ifdef TRACE_SOLVEGRAM_SOLVED
    ; mout()<<"attr_fixed_point:numit="<<numit<<"\n"
    #endif
    ; return changed
    ;}
     
}//exit eff namespace

#include "../grammar_pipeline_suffix.hpp"

#endif

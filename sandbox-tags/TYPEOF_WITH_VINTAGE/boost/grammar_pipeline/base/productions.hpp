//define a "base" grammar for use via "dynamic inheritance" by "derived" grammars.
#ifndef BOOST_GRAMMAR_PIPELINE_BASE_PRODUCTIONS_HPP_LJE20040223
#define BOOST_GRAMMAR_PIPELINE_BASE_PRODUCTIONS_HPP_LJE20040223
//  (C) Copyright Larry Joe Evans 2004.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "boost/dyn_inherit/base.hpp"
#include "../exp_types.hpp"
#include "../sole_ptr_container.hpp"
#include <list>
#include <vector>
    
#include "../grammar_pipeline_prefix.hpp"
//^See above include file for terminology
// and references
  
namespace base
//Purpose:
//  Provide classes just to represent the grammar.
//  No attributes, such as first or follow sets
//  as described in [Lew82] are provided.
//  Those will be provided by derived classes.
{
  
  namespace
exp_tree
//WHAT:
//  A "namespace" for grammar EXPression TREE nodes's which form
//  BASE classes.
//WHY:
//  Derived classes may have "attributes", such as first and
//  follow sets (as defined in [Lew82] or most any compiler
//  text). These derived classes will be "dynamically derived"
//  using the inherit class defined in
//  boost/dyn_inherit/inherit.hpp.

{
      class
    top
    //Purpose:
    //  This is the topmost superclass i.e. all node<exp_types> classes
    //  derive ultimately from top.
    ;
        typedef
      sole_ptr_container<top,std::list>
    children
    //Container for children of a node<exp_types>
    ;
    template<exp_types ExpType>
      class
    node
    //A node for grammer expression of type, ExpType.
    ;
      class
    symbol
    //Super type of node<input> and node<variable>
    ;
      class
    visitor
    {
     public:
            virtual
          void
        visit( node<input>*a_node)
        =0
        ;
            virtual
          void
        visit( node<variable>*a_node)
        =0
        ;
            virtual
          void
        visit( node<zero>*a_node)
        =0
        ;
            virtual
          void
        visit( node<one>*a_node)
        =0
        ;
            virtual
          void
        visit( node<sum>*a_node)
        =0
        ;
            virtual
          void
        visit( node<product>*a_node)
        =0
        ;
            virtual
          void
        visit( node<repeat_ge0>*a_node)
        =0
        ;
    };//end exp_visitor class
    
      class
    top
    : public boost::dyn_inherit::base
  #ifdef UTILITY_OBJECT_TRACKED_HPP_LJE20040112
    , public utility::object_tracked
  #endif
    , public exp_tree::children
    {
     public:
       ~top(void)
       {}
       
           virtual
         void
       accept(visitor* a_visitor)
       =0
       ;
           virtual
         exp_types
       exp_type(void)const
       =0
       ;
    };//end exp_tree::top class
    
      class
    symbol
    //Purpose:
    //  Represents "symbols", i.e.
    //  inputs and variables.
    : public exp_tree::top
    {
     public:
            typedef
          unsigned
        id_type
        ;
        
        symbol(id_type a_id)
          : my_id(a_id)
        {}
        
          id_type
        id(void)const
        {
            return my_id;
        }
     private:
          id_type const
        my_id
        ;
    };
    
//node<exp_types> definitions:

    template<>
      class
    node
      < input
      >
    : public exp_tree::symbol
    {
     public:
       node(unsigned a_id)
         : exp_tree::symbol(a_id)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return input;
       }
     };
     
    template<>
      class
    node
      < variable
      >
    : public exp_tree::symbol
    {
     public:
       node(unsigned a_id)
         : exp_tree::symbol(a_id)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return variable;
       }
     };
     
    template<>
      class
    node
      < zero
      >
    : public exp_tree::top
    {
     public:
       node(void)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return zero;
       }
    };

    template<>
      class
    node
      < one
      >
    : public exp_tree::top
    {
     public:
       node(void)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return one;
       }
    };

    template<>
      class
    node
      < sum
      >
    : public exp_tree::top
    {
     public:
       node(void)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return sum;
       }
    };

    template<>
      class
    node
      < product
      >
    : public exp_tree::top
    {
     public:
       node(void)
       {}
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return product;
       }
    };

    template<>
      class
    node
      < repeat_ge0
      >
    : public exp_tree::top
    {
     public:
       node(exp_tree::top* body)
       {
           this->push_back(body);
       }
         void
       accept(visitor*a_visitor)
       {
           a_visitor->visit(this);
       }
         exp_types
       exp_type(void)const
       {
           return repeat_ge0;
       }
    };

}//exit exp_tree namespace
    
  class
productions
  : public sole_ptr_container<exp_tree::top,std::vector>
//Purpose:
//  Represent the set of "productions"
//  in a grammar.
{
 public:
        typedef
      exp_tree::top
    this_top_type
    ;
        typedef
      sole_ptr_container<this_top_type,std::vector>
    super_type
    ;
    productions(super_type::size_type n)
      : super_type(n)
    {}
};

}//exit base namespace

#include "../grammar_pipeline_suffix.hpp"

#endif

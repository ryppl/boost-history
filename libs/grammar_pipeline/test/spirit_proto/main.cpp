//Purpose:
//  prototype method for using template metaprogramming
//  to calculate empty, first, follow, precede, etc. attributes
//  for grammar expressions.
//
//Compatible Compilers:
//  intel version 8.0
//
//Copyright:
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/core/parser.hpp>
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <bitset>
#include "boost/utility/trace_scope.hpp"
#define TRACK_ATTRIBUTES
#ifdef TRACK_ATTRIBUTES
//#define UTILITY_OBJECT_TRACKED_TRACE_MODE
#include "boost/utility/object_tracked.hpp"
#endif

namespace boost
{
namespace grammar_pipeline
//References:
//  [Lew82] 
//     Lewi, et. al.
//     _A Programming Methodology in Compiler Construction_ Part 2,
//     North-Holland, (1982) 
//Terminology:
//  variable = non-terminal in a grammar.  The reason
//    variable is used instead of non-terminal is that
//    variable suggests a value to be solved-for, and
//    the productions in the grammar are the "equations"
//    describing the constraints on these variables.
//  input = a terminal in a grammar.  The reason for
//    "input" instead of "terminal" is that [Lew82]
//    used the term "input vocabulary".  Also, the
//    inputs really what's "input" to a parser.
//  direction_fwd set = this is a variable attribute 
//    which is calculated from the follow and first set's 
//    (see most any compiler textbook) for a variable. It's
//    used to "direct" the parser to the next variable to
//    parse.  It's call DirSymb or direction
//    symbol in [Lew82].
//  output = a variable that's used as an "output".
//    IOW, the output's of one grammar could be used
//    as the input to another grammar, thus forming
//    a "grammar pipeline".
//
//    "output vocabulary" is the term used in [Lew82].
//Purpose:
//  Define classes for use in defining a grammar
//  with input's and output's and the direction sets 
//  for use in creating a recursive descent parser for 
//  each variable in that grammar.
//
{

template
  < typename ScannerTraits
  , typename Variables
  >
  struct
gram_tree
{
        typedef
      typename ScannerTraits::outs
    inps
    ;
        typedef
      Variables
    vars
    ;
      struct
    input_set
      : public std::bitset<inps::num_outs>
    {
            typedef
          std::bitset<inps::num_outs>
        super_type
        ;
            typedef
          input_set
        my_type
        ;
          void
        add_inp
          ( typename inps::symb_enum Id
          )
        {
            super_type::set(unsigned(Id));
        }
          input_set
        operator|(my_type const& a_set)const
        {
            input_set result(*this);
            result|= a_set;
            return result;
        }
          void
        marg_print(marg_ostream& sout)const
        {
            bool first=true;
            sout<<"{";
            for(unsigned i=0; i<inps::num_outs; ++i)
            {
                if(super_type::test(i))
                {
                    if(first)
                    {
                        first=false;
                    }
                    else
                    {
                        sout<<",";
                    }
                    sout<<" "<<inps::symb_names()[i];
                }
            }
            sout<<" } ";
        }
            friend
          marg_ostream&
        operator<<
          ( marg_ostream& sout
          , my_type const& x
          )
        {
            x.marg_print(sout);
            return sout;
        }
    };//end input_set struct

      enum
    attr_enum
    //WHAT:
    //  Enumeration of attributes requiring
    //  iterative solution.
    //WHY:
    //  Enable templatized grammar::solve_single_attr.
    { empty_attr
    , first_attr
    , last_attr
    , precede_attr
    , follow_attr
    , numb_attrs
    };
    
      struct
    attributes
    #ifdef TRACK_ATTRIBUTES
      : public utility::object_tracked
    #endif
    {
            typedef
          attributes
        my_type
        ;
          bool
        empty_attr
        ;
          input_set
        first_attr
        ;
          input_set
        last_attr
        ;
          input_set
        precede_attr
        ;
          input_set
        follow_attr
        ;
        attributes(void)
          : empty_attr(false)
        {
        }
          input_set
        direction_fwd(void)const
        //Purpose:
        //  Implements "Definition of DIRSYMB"
        //  at [Lew82,p.113]
        {
            input_set l_dir= first_attr;
            if(empty_attr)
            {
                l_dir|= follow_attr;
            }
            return l_dir;
        }
          input_set
        direction_rev(void)const
        //Purpose:
        //  Implements "reverse" direction counterpart
        //  to direction_fwd.
        {
            input_set l_dir= last_attr;
            if(empty_attr)
            {
                l_dir|= precede_attr;
            }
            return l_dir;
        }
          void
        marg_print(marg_ostream& sout)const
        {
          #ifdef TRACK_ATTRIBUTES
            sout<<"{ id="<<id_get()<<"\n";
            sout<<", ";
          #else
            sout<<"{ ";
          #endif
            sout<<"empty="<<empty_attr<<"\n";
            sout<<", first=\n";
            ++sout;
            first_attr.marg_print(sout);sout<<"\n";
            --sout;
            sout<<", last=\n";
            ++sout;
            last_attr.marg_print(sout);sout<<"\n";
            --sout;
            sout<<", precede=\n";
            ++sout;
            precede_attr.marg_print(sout);sout<<"\n";
            --sout;
            sout<<", follow=\n";
            ++sout;
            follow_attr.marg_print(sout);sout<<"\n";
            --sout;
            sout<<"}\n";
        }
            friend
          marg_ostream&
        operator<<
          ( marg_ostream& sout
          , my_type const& x
          )
        {
            x.marg_print(sout);
            return sout;
        }
        
    };//end attributes struct
    
      struct
    expr_top
    //Purpose:
    //  Serve as "top" or super type of all
    //  grammar expressions, i.e. all expr_*
    //  classes derive from this one.
    //
    //Alternative:
    //  Have all expr_* classes publicly inherit
    //  from attributes.  The rationale for
    //  not doing this is to avoid any ambiguity
    //  between the | operator for grammar expressions
    //  and the | operator between input_set's.
      : public boost::spirit::parser<expr_top> 
    {
     public:
            typedef
          typename ScannerTraits::scanner_type
        scanner_type
        ;
            typedef
          typename boost::spirit::parser_result<expr_top, scanner_type>::type
        result_type
        ;
          attributes const&
        attrs_con(void)const
        {
            return my_attrs;
        }
          attributes&
        attrs_mut(void)
        {
            return my_attrs;
        }
     private:
          attributes
        my_attrs
        ;
    };//end expr_top struct
    
      struct
    expr_abstract
    //Purpose:
    //  Enable defn_variable to hold a definition
    //  of variable's rhs without knowing exact
    //  type of rhs.  This, in turn, enables
    //  storing a sequence of defn_variable's in 
    //  an stl container, such as the productions
    //  class.
    //Spirit_Counterpart:
    //  Similar to abstract_parser template in
    //  boost/spirit/core/non_terminal/impl/rule.ipp.
    {
     public:
            typedef
          typename expr_top::scanner_type
        scanner_type
        ;
            typedef
          typename expr_top::result_type
        result_type
        ;
            typedef
          expr_abstract
        my_type
        ;
            virtual
        ~expr_abstract(void)
        {}
            virtual
          expr_abstract*
        clone(void)const
        =0
        ;
            virtual
          void
        empty_abstract(void)
        =0
        ;
            virtual
          void
        first_abstract(void)
        =0
        ;
            virtual
          void
        last_abstract(void)
        =0
        ;
            virtual
          void
        precede_abstract(input_set const&)
        =0
        ;
            virtual
          void
        follow_abstract(input_set const&)
        =0
        ;
            virtual
          attributes const&
        attrs_abstract(void)const
        =0
        ;
            virtual
          void
        tree_print_abstract(marg_ostream& sout)const
        =0
        ;
            friend
          marg_ostream&
        operator<<
          ( marg_ostream& sout
          , my_type const& x
          )
        {
            x.tree_print_abstract(sout);
            return sout;
        }
            virtual
          result_type
        parse(scanner_type const& scan)const
        =0
        ;
    };//end expr_abstract
    
    template
      < typename ConcreteExpr
      >
      struct
    expr_concrete
      : public expr_abstract
    //Purpose:
    //  Enable templated defn_variable CTOR
    //  to receive a concrete rhs but to store
    //  as abstract rhs.
    //Spirit_Counterpart:
    //  Similar to concrete_parser template in
    //  boost/spirit/core/non_terminal/impl/rule.ipp.
    {
     public:
            typedef
          typename expr_top::scanner_type
        scanner_type
        ;
            typedef
          typename expr_top::result_type
        result_type
        ;
        expr_concrete(ConcreteExpr const& a_expr)
          : my_expr(a_expr)
        {}
          expr_abstract*
        clone(void)const
        {
            return new expr_concrete(*this);
        }
          void
        empty_abstract(void)
        {
            my_expr.empty_concrete();
        }
          void
        first_abstract(void)
        {
            my_expr.first_concrete();
        }
          void
        last_abstract(void)
        {
            my_expr.last_concrete();
        }
          void
        precede_abstract(input_set const& a_set)
        {
            my_expr.precede_concrete(a_set);
        }
          void
        follow_abstract(input_set const& a_set)
        {
            my_expr.follow_concrete(a_set);
        }
          attributes const&
        attrs_abstract(void)const
        {
            return my_expr.attrs_con();
        }
          void
        tree_print_abstract(marg_ostream& sout)const
        {
            my_expr.tree_print_concrete(sout);
            sout<<"\n";
        }
          result_type
        parse(scanner_type const& scan)const
        {
            return my_expr.parse(scan);
        }
     private:
          ConcreteExpr
        my_expr
        ;    
    };//end expr_concrete
    
      struct
    defn_variable
    //Purpose:
    //  Hold the definition of a variable.
    //Spirit_Counterpart:
    //  Similar to rule template in 
    //  boost/spirit/core/non_terminal/rule.hpp.
    {
     public:
            typedef
          defn_variable
        my_type
        ;
            typedef
          boost::scoped_ptr<expr_abstract>
        rhs_type
        ;
        defn_variable(void)
        {}
        
        defn_variable(my_type const& a_copy)
          : my_rhs(a_copy.rhs_clone())
        {}
        
        template
          < typename Rhs
          >
        defn_variable(Rhs const& a_rhs)
          : my_rhs(new expr_concrete<Rhs>(a_rhs))
        {}
        
          my_type const&
        operator=(my_type const& a_var)
          {
              expr_abstract*rp= a_var.rhs_clone();
              my_rhs.reset(rp);
              return *this;
          }
        
          expr_abstract*
        rhs_get(void)const
        {
            return my_rhs.get();
        }
        
        template
          < typename Rhs
          >
          void
        rhs_put(Rhs const& a_rhs)
        {
            expr_abstract*rp=new expr_concrete<Rhs>(a_rhs);
            my_rhs.reset(rp);
        }
        
        template
          < typename Rhs
          >
          my_type const&
        operator=(Rhs const& a_rhs)
        {
            rhs_put(a_rhs);
            return *this;
        }
        
        template
          < attr_enum AttrName
          >
          bool
        eval_single_attr
          ( void
          )
        //Purpose:
        //  Allow grammar::solve_single_attr to work.
        {
            eval_attr_vec_type const&ea_vec=evaluators();
            eval_attr_elem_type ea_elem=ea_vec[AttrName];
            bool result=(this->*ea_elem)();
            return result;
        }
          void
        marg_print(marg_ostream& sout)const
        {
            sout<<"{ my_attrs=\n";
            ++sout;
            my_attrs.marg_print(sout);
            --sout;
            sout<<", my_rhs=\n";
            ++sout;
            expr_abstract*rhs_ptr=rhs_get();
            if(rhs_ptr==0)
            {
                sout<<"(is_null)";
            }
            else
            {
                rhs_ptr->tree_print_abstract(sout);
            }
            --sout;
            sout<<"}\n";
        }
            friend
          marg_ostream&
        operator<<
          ( marg_ostream& sout
          , my_type const& x
          )
        {
            x.marg_print(sout);
            return sout;
        }
     public:
          attributes
        my_attrs
        ;
     private:
          rhs_type
        my_rhs
        //Purpose:
        //  The variable's definition.
        //Spirit_Counterpart:
        //  rule<...>::ptr member variable.
        ;
          expr_abstract*
        rhs_clone(void)const
        {
            expr_abstract*rp=my_rhs?my_rhs->clone():0;
            return rp;
        }
        
     private: //btm_up evaluations
          bool
        eval_empty(void)
        {
            bool pre=my_attrs.empty_attr;
            my_rhs->empty_abstract();
            my_attrs.empty_attr= my_rhs->attrs_abstract().empty_attr;
            return pre==my_attrs.empty_attr;
        }
          bool
        eval_first(void)
        {
            input_set pre=my_attrs.first_attr;
            my_rhs->first_abstract();
            my_attrs.first_attr= my_rhs->attrs_abstract().first_attr;
            return pre==my_attrs.first_attr;
        }
          bool
        eval_last(void)
        {
            input_set pre=my_attrs.last_attr;
            my_rhs->last_abstract();
            my_attrs.last_attr= my_rhs->attrs_abstract().last_attr;
            return pre==my_attrs.last_attr;
        }
        
     private: //top_down evaluations
          bool
        eval_precede(void)
        {
            input_set pre=my_attrs.precede_attr;
            my_rhs->precede_abstract(pre);
            return pre==my_attrs.precede_attr;
        }
          bool
        eval_follow(void)
        {
            input_set pre=my_attrs.follow_attr;
            my_rhs->follow_abstract(pre);
            return pre==my_attrs.follow_attr;
        }
        
     private: //enable templated solve_single_attr
            typedef
          bool 
          (defn_variable::*
        eval_attr_elem_type
          )
          (void)
        ;
            typedef
          eval_attr_elem_type
        eval_attr_vec_type
          [numb_attrs]
        ;
            static
          eval_attr_vec_type const&
        evaluators(void)
        //Allow selection of proper eval_XXX function
        //by eval_single_attr.
        {
            static eval_attr_vec_type const evals=
            { &my_type::eval_empty
            , &my_type::eval_first
            , &my_type::eval_last
            , &my_type::eval_precede
            , &my_type::eval_follow
            };
            return evals;
        }
    };//end defn_variable struct
    
      struct
    productions
      : public std::vector<defn_variable>
    {
            typedef
          std::vector<defn_variable>
        super_type
        ;
        productions(void)
          : super_type(Variables::num_symb)
        {}
    };//end productions struct
    
    //begin: Primitive expressions
    
      struct
    expr_input
    {
        template
          < typename inps::symb_enum Id
          >
          struct
        node
        //Purpose:
        //  Represent terminal, or input, expression
        //  on the rhs of a variable definition.
        //Spirit_Counterpart:
        //  None really, because spirit does not
        //  encode inputs as int's.  The closest
        //  is maybe char_parser in:
        //    boost/spirit/core/primitives.hpp
        //  or rxstrlit in:
        //    boost/spirit/utility/regex.hpp
        : public expr_top
        {
         public:
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
                typedef
              node
            my_type
            ;
            node(void)
            {
                this->attrs_mut().empty_attr=false;
                this->attrs_mut().first_attr.add_inp(Id);
                this->attrs_mut().last_attr.add_inp(Id);
            }
              void
            empty_concrete(void)const
            {
                //set by CTOR.
            }
              void
            first_concrete(void)const
            {
                //set by CTOR.
            }
              void
            last_concrete(void)const
            {
                //set by CTOR.
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_input("<<inps::symb_names()[Id]<<")\n";
            }
            
                friend
              marg_ostream&
            operator<<
              ( marg_ostream& sout
              , my_type const& x
              )
            {
                x.tree_print_concrete(sout);
                return sout;
            }
            
              bool
            test
              ( typename scanner_type::value_t const& a_inp
              )const
            {
                return a_inp==typename scanner_type::value_t(Id);
            }
            
              result_type
            parse(scanner_type const& scan)const
            //Method:
            //  Essentially that of boost::spirit::
            //  char_parser<DerivedT>::parse(scanner_type const&).
            //
            //  This method also implements an "approximation"
            //  to Axiomatic rule 2) in [Lew82,p.108].
            {
                
              #if defined(TRACE_SCOPE_HPP)
                utility::trace_scope ts("parse<input>");
                mout()<<":expr_input:id="<<Id<<"\n";
              #endif
              
                if (!scan.at_end())
                {
                    typedef typename scanner_type::value_t value_t;
                    value_t inp= *scan;
                  #if defined(TRACE_SCOPE_HPP)
                    mout()<<":inp="<<inp<<"\n";
                  #endif
                    if (this->test(inp))
                    {
                        typedef typename scanner_type::iterator_t iterator_t;
                        iterator_t save(scan.first);
                        ++scan.first;
                        return scan.create_match(1, inp, save, scan.first);
                    }
                }
                return scan.no_match();
            }
            
        };//end node struct
        
        template
          < typename inps::symb_enum Id
          >
            static
          node<Id>
        mk(void)
        {
            return node<Id>();
        }
    };
    
      struct
    expr_variable
    {
        template
          < typename vars::symb_enum Id
          >
          struct
        node
        //Purpose:
        //  Represent non-terminal, or variable, expression
        //  on the rhs of a variable definition.
        //Spirit_Counterpart:
        //  I'm guessing it's the same as rule template class since
        //  in the libs/spirit/example/fundamental/tree_calc_grammar.hpp
        //  rules appear on the rhs of a rule definition.
        : public expr_top
        {
         public:
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
                typedef
              node
            my_type
            ;
            node(productions& a_prods)
              : my_def(a_prods[Id])
            {
            }
              void
            empty_concrete(void)
            {
                this->attrs_mut().empty_attr= my_def.my_attrs.empty_attr;
            }
              void
            first_concrete(void)
            {
                this->attrs_mut().first_attr|= my_def.my_attrs.first_attr;
            }
              void
            last_concrete(void)
            {
                this->attrs_mut().last_attr|= my_def.my_attrs.last_attr;
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
                my_def.my_attrs.precede_attr|= a_set;
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
                my_def.my_attrs.follow_attr|= a_set;
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_variable("<<vars::symb_names()[Id]<<")\n";
            }
                friend
              marg_ostream&
            operator<<
              ( marg_ostream& sout
              , my_type const& x
              )
            {
                x.tree_print_concrete(sout);
                return sout;
            }
              result_type
            parse(scanner_type const& scan)const
            //This method implements Axiomatic rule 3)
            //in [Lew82,p.108].
            {
                result_type result=my_def.rhs_get()->parse(scan);
                return result;
            }
             
         private:
              defn_variable&
            my_def
            ;
        };//end node
        
        template
          < typename vars::symb_enum Id
          >
            static
          node<Id>
        mk(productions& a_prods)
        {
            return node<Id>(a_prods);
        }
    };//end expr_variable struct
    
      struct
    expr_epsilon
    {
          struct
        node
        //Purpose:
        //  Represent the null terminal string.
        //Spirit_Counterpart:
        //  epsilon parser defined in:
        //    boost/spirit/core/composite/epsilon.hpp
        : public expr_top
        {
         public:
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
                typedef
              node
            my_type
            ;
            node(void)
            {
                this->attrs_mut().empty_attr= true;
            }
              void
            empty_concrete(void)
            {
                //set by CTOR.
            }
              void
            first_concrete(void)const
            {
                //default value(empty set) is correct value
            }
              void
            last_concrete(void)const
            {
                //default value(empty set) is correct value
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_epsilon()\n";
            }
                friend
              marg_ostream&
            operator<<
              ( marg_ostream& sout
              , my_type const& x
              )
            {
                x.tree_print_concrete(sout);
                return sout;
            }
              result_type
            parse(scanner_type const& scan) const
            //Method:
            //  Always assumes a match and return the 
            //  same as empty_match_parser::parse defined
            //  in boost/spirit/core/composite/epsilon.hpp
            //
            //  This method also implements
            //  Axiomatic rule 1) in [Lew82,p.108].
            {
                return scan.empty_match();
            }
            
        };
            static
          node
        mk(void)
        {
            return node();
        }
    };
    
    //end: Primitive expressions
    //begin: Composite expressions
    
    template
      < typename Body
      >
      struct
    expr_unary
      : public expr_top
    {
     public:
          void
        tree_print_concrete(marg_ostream& sout)const
        {
            sout<<"( my_body=\n";
            ++sout;
            my_body.tree_print_concrete(sout);
            --sout;
            sout<<")\n";
        }
          Body const&
        body(void)const
        {
            return my_body;
        }
          Body&
        body(void)
        {
            return my_body;
        }
     protected:
        expr_unary(Body const& a_body)
          : my_body(a_body)
        {}
     private:
          Body
        my_body
        ;
    };
    
    template
      < typename Left
      , typename Right
      >
      struct
    expr_binary
      : public expr_top
    {
     public:
          void
        tree_print_concrete(marg_ostream& sout)const
        {
            sout<<"( my_left=\n";
            ++sout;
            my_left.tree_print_concrete(sout);
            --sout;
            sout<<", my_right=\n";
            ++sout;
            my_right.tree_print_concrete(sout);
            --sout;
            sout<<")\n";
        }
          Left const&
        left(void)const
        {
            return my_left;
        }
          Right const&
        right(void)const
        {
            return my_right;
        }
          Left&
        left(void)
        {
            return my_left;
        }
          Right&
        right(void)
        {
            return my_right;
        }
     protected:
        expr_binary(Left const& a_left, Right const& a_right)
          : my_left(a_left)
          , my_right(a_right)
        {}
     private:
          Left
        my_left
        ;
          Right
        my_right
        ;
    };
    
      struct
    expr_alternative
    {
        template
          < typename Left
          , typename Right
          >
          struct
        node
        //Purpose:
        //  Handles expressions of the form:
        //    Left | Right
        //Spirit_Counterpart:
        //  alternative class defined in:
        //    boost/spirit/core/composite/alternative.hpp
          : public expr_binary<Left,Right>
        {
                typedef
              expr_binary<Left,Right>
            super_type
            ;
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
            node(Left const& a_left, Right const& a_right)
              : super_type(a_left, a_right)
            {}
              void
            empty_concrete(void)
            {
                this->left().empty_concrete();
                this->right().empty_concrete();
                this->attrs_mut().empty_attr
                   = this->left().attrs_con().empty_attr
                  || this->right().attrs_con().empty_attr;
            }
              void
            first_concrete(void)
            {
                this->left().first_concrete();
                this->right().first_concrete();
                //Lew82_Counterpart:
                //  The following two statements correspond to:
                //    7.6 FIRST pass
                //      $Treatment of expression
                //        ##actions
                //          #pass_a_e
                //          #union
                //  
                //  of [Lew82,pp. 444], and to:
                //    
                //    Definition of FIRST
                //      rule 3)
                //  
                //  of [Lew82,p.114].
                //
                this->attrs_mut().first_attr
                   =  this->left().attrs_con().first_attr;
                this->attrs_mut().first_attr
                  |=  this->right().attrs_con().first_attr;
            }
              void
            last_concrete(void)
            {
                this->left().last_concrete();
                this->right().last_concrete();
                this->attrs_mut().last_attr
                  |=  this->left().attrs_con().last_attr;
                this->attrs_mut().last_attr
                  |=  this->right().attrs_con().last_attr;
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
                this->left().precede_concrete(a_set);
                this->right().precede_concrete(a_set);
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
                //Lew82_Counterpart:
                //  The following two statements correspond to:
                //  
                //    7.7 FOLLOW pass
                //      $Treatment of expression
                //        ##actions
                //          #pass_e_a1
                //          #pass_e_a2
                //  
                //  of [Lew82,pp. 447-448], and to:
                //    
                //    Definition of FOLLOW
                //      rule 1)
                //  
                //  of [Lew82,p.116].
                //
                this->left().follow_concrete(a_set);
                this->right().follow_concrete(a_set);
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_alternative\n";
                super_type::tree_print_concrete(sout);
            }
              result_type
            parse(scanner_type const& scan) const
            {
                typedef result_type result_t;//conform to spirit name
                typedef typename scanner_type::value_t value_t;
                value_t inp= *scan;
              #if defined(TRACE_SCOPE_HPP)
                utility::trace_scope ts("<alternative>::parse");
                mout()<<":inp="<<inp<<"\n";
              #endif
                bool lookahead_test= this->left().attrs_con().direction_fwd().test(inp);
              #if defined(TRACE_SCOPE_HPP)
                mout()<<":lookahead_test="<<lookahead_test<<"\n";
              #endif
                if(lookahead_test)
                {// scope for save
                    typedef typename scanner_type::iterator_t iterator_t;
                    iterator_t save = scan.first;
                    if (result_t hit = this->left().parse(scan))
                        return hit;
                    scan.first = save;
                }
                return this->right().parse(scan);
            }
                
        };
        template
          < typename Left
          , typename Right
          >
            static
          node<Left,Right>
        mk(Left const& a_left, Right const& a_right)
        {
            return node<Left,Right>(a_left,a_right);
        }
    };
    
    template
      < typename Left
      , typename Right
      >
        friend
      expr_alternative::node<Left,Right>
    operator|(Left const& a_left, Right const& a_right)
    {
        return expr_alternative::mk(a_left,a_right);
    }
    
      struct
    expr_sequence
    {
        template
          < typename Left
          , typename Right
          >
          struct
        node
        //Purpose:
        //  Handles expressions of the form:
        //    Left >> Right
        //  i.e. a Left followed by a Right.
        //Spirit_Counterpart:
        //  sequence class defined in:
        //    boost/spirit/core/composite/sequence.hpp
          : public expr_binary<Left,Right>
        {
                typedef
              expr_binary<Left,Right>
            super_type
            ;
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
                typedef
              node
            my_type
            ;
            node(Left const& a_left, Right const& a_right)
              : super_type(a_left, a_right)
            {}
              void
            empty_concrete(void)
            {
                this->left().empty_concrete();
                this->right().empty_concrete();
                this->attrs_mut().empty_attr
                  =  this->left().attrs_con().empty_attr
                  && this->right().attrs_con().empty_attr;
            }
              void
            first_concrete(void)
            {
                this->left().first_concrete();
                this->right().first_concrete();
                this->attrs_mut().first_attr
                  =  this->left().attrs_con().first_attr;
                if(this->left().attrs_con().empty_attr)
                {
                    this->attrs_mut().first_attr
                      |= this->right().attrs_con().first_attr;
                }
            }
              void
            last_concrete(void)
            {
                this->left().last_concrete();
                this->right().last_concrete();
                this->attrs_mut().last_attr
                  =  this->right().attrs_con().last_attr;
                if(this->right().attrs_con().empty_attr)
                {
                    this->attrs_mut().last_attr
                      |= this->left().attrs_con().last_attr;
                }
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
                this->left().precede_concrete(a_set);
                this->right().precede_concrete(this->left().attrs_con().direction_rev());
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
                this->right().follow_concrete(a_set);
                this->left().follow_concrete(this->right().attrs_con().direction_fwd());
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_sequence\n";
                super_type::tree_print_concrete(sout);
            }
              result_type
            parse(scanner_type const& scan) const
            {
                typedef result_type result_t;
                if (result_t ma = this->left().parse(scan))
                    if (result_t mb = this->right().parse(scan))
                    {
                        scan.concat_match(ma, mb);
                        return ma;
                    }
                return scan.no_match();
            }
        };
        template
          < typename Left
          , typename Right
          >
            static
          node<Left,Right>
        mk(Left const& a_left, Right const& a_right)
        {
            return node<Left,Right>(a_left,a_right);
        }
    };
    
    template
      < typename Left
      , typename Right
      >
        friend
      expr_sequence::node<Left,Right>
    operator>>(Left const& a_left, Right const& a_right)
    {
        return expr_sequence::mk(a_left,a_right);
    }
    
      struct
    expr_kleene_star
    {
        template
          < typename Body
          >
          struct
        node
        //Purpose:
        //  Handles expressions of the form:
        //    *Body
        //  i.e. a sequence of 0 or more strings
        //  described by Body.
        //Spirit_Counterpart:
        //  kleene_star class defined in:
        //    boost/spirit/core/composite/kleene_star.hpp
          : public expr_unary<Body>
        {
         public:
                typedef
              expr_unary<Body>
            super_type
            ;
                typedef
              typename expr_top::scanner_type
            scanner_type
            ;
                typedef
              typename expr_top::result_type
            result_type
            ;
            node(node const& a_node)
              : super_type(a_node)
            {
                this->attrs_mut().empty_attr= true;
            }
            node(Body const& a_body)
              : super_type(a_body)
            {
                this->attrs_mut().empty_attr= true;
            }
              void
            empty_concrete(void)
            {
                //set by CTOR.
            }
              void
            first_concrete(void)
            {
                this->body().first_concrete();
                this->attrs_mut().first_attr
                  =  this->body().attrs_con().first_attr;
            }
              void
            last_concrete(void)
            {
                this->body().last_concrete();
                this->attrs_mut().last_attr
                  =  this->body().attrs_con().last_attr;
            }
              void
            precede_concrete(input_set const& a_set)
            {
                this->attrs_mut().precede_attr= a_set;
                this->body().precede_concrete
                  ( this->attrs_con().last_attr
                  | this->attrs_con().precede_attr
                  );
            }
              void
            follow_concrete(input_set const& a_set)
            {
                this->attrs_mut().follow_attr= a_set;
                //Lew82_Counterpart:
                //    Definition of FOLLOW
                //      rule 3)
                //  
                //  
                //  of [Lew82,p.116].
                //
                this->body().follow_concrete
                  ( this->attrs_con().first_attr
                  | this->attrs_con().follow_attr
                  );
            }
              void
            tree_print_concrete(marg_ostream& sout)const
            {
                sout<<"expr_kleene_star\n";
                super_type::tree_print_concrete(sout);
            }
              result_type
            parse(scanner_type const& scan) const
            {
                typedef result_type result_t;
                result_type hit = scan.empty_match();
                input_set const dir= this->body().attrs_con().direction_fwd();
        
                for (;;)
                {
                    typedef typename scanner_type::value_t value_t;
                    value_t inp= *scan;
                    bool lookahead_test= dir.test(inp);
                    if(lookahead_test)
                    {
                        typedef typename scanner_type::iterator_t iterator_t;
                        iterator_t save = scan.first;
                        result_t next = this->body().parse(scan);
                        if (next)
                        {
                            scan.concat_match(hit, next);
                        }
                        else
                        {
                            scan.first = save;
                            return hit;
                        }
                    }//end then(lookahead_test)
                    else
                    {
                        return hit;
                    }
                }//end for(;;)
            }//end parse
        };
        template
          < typename Body
          >
            static
          node<Body>
        mk(Body const& a_body)
        {
            return node<Body>(a_body);
        }
    };
    
    template
      < typename Body
      >
        friend
      expr_kleene_star::node<Body>
    operator*(Body const& a_body)
    {
        return expr_kleene_star::mk(a_body);
    }
    
    //end: Composite expressions
    
      struct
    grammar
      : public productions
    {
            typedef
          grammar
        my_type
        ;
        grammar(void)
        {}
        template
          < attr_enum AttrName
          >
          bool
        solve_single_attr(unsigned maxit)
        //What:
        //  Solve for attribute, AttrName.
        //How:
        //  For each variable, evaluate and update the attribute's
        //  values until they're constant, i.e. until all of 
        //  (*this)[ivar].eval_single_attr<AttrName>(), for
        //  i=0..nvar-1 returns true.
        {
            bool solved=false;
            productions& me=*this;
            unsigned nvar=me.size();
            for(; maxit>0 && !solved; --maxit)
            {
                solved=true;
                for(unsigned ivar=0; ivar<nvar; ++ivar)
                {
                    defn_variable& vdef= me[ivar];
                    bool result= vdef.template eval_single_attr<AttrName>();
                    solved= solved && result;
                }
            }
            return solved;
        }
          unsigned
        solve_all_attrs(unsigned maxit)
        {
            unsigned num_soln=0;
            if(!(solve_single_attr<empty_attr>(maxit))) return num_soln;
            ++num_soln;
            if(!(solve_single_attr<first_attr>(maxit))) return num_soln;
            ++num_soln;
            if(!(solve_single_attr<last_attr>(maxit))) return num_soln;
            ++num_soln;
            if(!(solve_single_attr<precede_attr>(maxit))) return num_soln;
            ++num_soln;
            if(!(solve_single_attr<follow_attr>(maxit))) return num_soln;
            ++num_soln;
            return num_soln;
        }
          void
        marg_print(marg_ostream& sout)const
        {
            productions const& me=*this;
            unsigned const nvar=me.size();
            sout<<"{ ";
            for(unsigned ivar=0; ivar<nvar; ++ivar)
            {
                defn_variable const& vdef=me[ivar];
                if(ivar>0)
                {
                    sout<<", ";
                }
                sout<<vars::symb_names()[ivar]<<"=\n";
                ++sout;
                sout<<vdef;
                --sout;
            }
            sout<<"}\n";
        }
            friend
          marg_ostream&
        operator<<
          ( marg_ostream& sout
          , my_type const& x
          )
        {
            x.marg_print(sout);
            return sout;
        }
    };//end grammar struct
        
};//end gram_tree<Scanner,Variables> struct

}//exit grammar_pipeline namespace
}//exit boost namespace

#include "libs/grammar_pipeline/examples/arith_exp_symbols.hpp"
using namespace libs::grammar_pipeline::examples::arith_exp;

template
  < typename InpSymbs
  >
  struct
scanner_traits
  : public boost::spirit::scanner<typename InpSymbs::symb_enum const*>
{
        typedef
      InpSymbs
    outs
    ;
        typedef
      boost::spirit::scanner<typename outs::symb_enum const*>
    scanner_type
    ;
};

using namespace boost::grammar_pipeline;

  int
main(void)
{
    typedef scanner_traits<inps> scan_traits_type;
    typedef gram_tree<scan_traits_type,vars> gt;
    typedef gt::expr_top::result_type result_type;
        
    {//creates arith_expr grammar using only operators and primitive CTORS.
        utility::trace_scope("arith_expr using operators");
        typedef gt::expr_input inp;
        
        inp::node<inps::id_a> id_a;
        inp::node<inps::id_b> id_b;
        inp::node<inps::op_add        > op_add;
        inp::node<inps::op_mult       > op_mult;
        inp::node<inps::op_paren_open > op_paren_open;
        inp::node<inps::op_paren_close> op_paren_close;
        
        //The following grammar is based on [Lew82,pp.114-117]
        //(See boost/grammar_pipeline/grammar_pipeline_prefix.hpp)
        gt::grammar prods;
        typedef gt::expr_variable var;
        var::node<vars::e> e(prods);
        var::node<vars::t> t(prods);
        var::node<vars::f> f(prods);
      
        prods[vars::e]= t >> *( op_add  >> t );
        prods[vars::t]= f >> *( op_mult >> f );
        prods[vars::f]= id_a | id_b | op_paren_open >> e >> op_paren_close;
        bool all_solved= prods.solve_all_attrs(4) == gt::numb_attrs;
        BOOST_TEST(all_solved);
        mout()<<"arith_exp_grammar=\n"<<prods<<"\n";
        {
            inps::symb_enum const inputs[]=
            { inps::id_a
            , inps::op_mult
            , inps::id_b
            };
            unsigned const len_inp=sizeof(inputs)/sizeof(inps::symb_enum);
            typedef scan_traits_type::scanner_type scan_type;
            scan_type::iterator_t beg_inp=inputs;
            scan_type::iterator_t end_inp=inputs+len_inp;
            scan_type l_scan(beg_inp,end_inp);
            {
                utility::trace_scope ts("parse");
                result_type rt=prods[vars::e].rhs_get()->parse(l_scan);
                mout()<<"rt.length()="<<rt.length()<<"\n";
                BOOST_TEST(rt);
            }
            bool at_end=l_scan.at_end();
            BOOST_TEST(at_end);
        }
    }
    unsigned num_leaks=utility::object_tracked::members_size();
    BOOST_TEST(num_leaks==0);
    return 1;
}

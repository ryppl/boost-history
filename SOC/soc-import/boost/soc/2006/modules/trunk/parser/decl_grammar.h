#ifndef INCLUDE_DECL_GRAMMAR_H
#define INCLUDE_DECL_GRAMMAR_H

#include <iostream>

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_grammar_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/cpp_exceptions.hpp>

#include "actions.h"
#include "lexpolicies.h"

// using namespace boost::spirit;
using namespace boost::wave;

/*
The grammar here's a slight modification of Danny Havenith's partial
C++ grammar (the Hannibal example in Wave).  Much thanks and appreciation!
*/

struct decl_grammar : public boost::spirit::grammar<decl_grammar> {
	// iterator splicing engine.
	mutable std::vector<std::string> m_ids;
	mutable OutputDelegate& m_del;
	
	decl_grammar (OutputDelegate& del) : m_del(del) {}
	
	template<typename ScannerT>
	struct definition {
//		boost::spirit::rule<ScannerT>  skip_semi, skip_block, skip, import_stmt,
//		                export_stmt, translation_unit, inner_block;
        // declare non-terminals
        typedef boost::spirit::rule<ScannerT> rule_type;

		rule_type import_stmt, export_stmt, skip_block, preproc_stmt;

        rule_type constant_expression;
        rule_type logical_or_exp, logical_and_exp;
        rule_type inclusive_or_exp, exclusive_or_exp, and_exp;
        rule_type cmp_equality, cmp_relational;
        rule_type shift_exp;
        rule_type add_exp, multiply_exp;
        rule_type unary_exp, primary_exp, constant;

        boost::spirit::subrule<0> const_exp_subrule;
        boost::spirit::subrule<1> shift_exp_clos;

        rule_type simple_type_name, class_keywords;
        rule_type storage_class_specifier, cv_qualifier, function_specifier;
        rule_type access_specifier;
        rule_type sign_specifier;
        rule_type extension_type_decorator;
        rule_type operator_sym;
        rule_type class_key;
        rule_type enumerator;
        rule_type enumerator_list;
        rule_type enumerator_definition;
        rule_type member_declarator;
        rule_type member_declarator_list;
        rule_type member_declaration;
        rule_type constant_initializer;
        rule_type pure_specifier;
        rule_type namespace_body;
        rule_type type_id;
        rule_type unnamed_namespace_definition;
        rule_type extension_namespace_definition;
        rule_type original_namespace_definition;
        rule_type named_namespace_definition;
        rule_type namespace_definition;
        rule_type linkage_specification;
        rule_type explicit_specialization;
        rule_type using_directive;
        rule_type using_declaration;
        rule_type type_parameter;
        rule_type template_parameter;
        rule_type template_parameter_list;
        rule_type template_declaration;
        rule_type explicit_instantiation;
        rule_type qualified_namespace_specifier;
        rule_type namespace_alias_definition;
        rule_type expression_list;
        rule_type initializer_list;
        rule_type initializer_clause;
        rule_type initializer;
        rule_type init_declarator;
        rule_type init_declarator_list;
        rule_type asm_definition;
        rule_type simple_declaration;
        rule_type block_declaration;
        rule_type declaration;
        rule_type declaration_seq;
        rule_type translation_unit;

        rule_type function_definition, function_definition_helper, declarator;
        rule_type direct_declarator, parameters_or_array_spec;
        rule_type abstract_declarator, direct_abstract_declarator;
        rule_type direct_abstract_declarator_helper;
        rule_type parameter_declaration_clause, parameter_declaration_list;
        rule_type parameter_declaration, assignment_expression, decimal_literal;
        rule_type octal_literal, hexadecimal_literal;
        rule_type declarator_id, id_expression, qualified_id, unqualified_id;
        rule_type operator_function_id, conversion_function_id, conversion_type_id;
        rule_type conversion_declarator, function_body;
        rule_type compound_statement, ctor_initializer, ptr_operator;
        rule_type decl_specifier, type_specifier;
        rule_type type_specifier_seq, cv_qualifier_seq, enum_specifier;
        rule_type enum_keyword, simple_type_specifier;
        rule_type class_specifier, member_specification, class_head;
        rule_type type_name, elaborated_type_specifier, template_argument_list;
        rule_type template_argument, nested_name_specifier;
        rule_type class_or_namespace_name, class_name, enum_name, typedef_name;
        rule_type namespace_name, template_id;
        rule_type decl_specifier_seq, no_type_decl_specifier;
        rule_type function_try_block, handler_seq, handler;
        rule_type exception_specification, template_name;
        rule_type original_namespace_name, base_specifier;
        rule_type base_specifier_list, base_clause;
        rule_type odd_language_extension, mem_initializer_id;
        rule_type mem_initializer, mem_initializer_list;
		
		definition (decl_grammar const& self) {
			//
			// Partial C++ Grammar
			//
		
			using namespace boost::spirit;
            using namespace boost::wave;
            using boost::wave::util::pattern_p;

            // constant_expression was copied from Wave
            constant_expression
                =   logical_or_exp
                >> !(const_exp_subrule =
                        ch_p(T_QUESTION_MARK)
                        >>  logical_or_exp >> ch_p(T_COLON) >> logical_or_exp
                     )
                ;

            logical_or_exp 
                =   logical_and_exp
                >> *(   pattern_p(T_OROR, MainTokenMask) 
                    >>  logical_and_exp 
                    )
                ;

            logical_and_exp
                =   inclusive_or_exp
                >> *(   pattern_p(T_ANDAND, MainTokenMask)
                    >>  inclusive_or_exp
                    )
                ;

            inclusive_or_exp
                =   exclusive_or_exp
                >> *(   pattern_p(T_OR, MainTokenMask)
                    >>  exclusive_or_exp
                    )
                ;

            exclusive_or_exp
                =   and_exp
                >> *(   pattern_p(T_XOR, MainTokenMask)
                    >>  and_exp
                    )
                ;

            and_exp
                =   cmp_equality
                >> *(   pattern_p(T_AND, MainTokenMask)
                    >>  cmp_equality
                    )
                ;

            cmp_equality
                =   cmp_relational
                >> *(   ch_p(T_EQUAL) >>  cmp_relational
                    |   pattern_p(T_NOTEQUAL, MainTokenMask) >> cmp_relational
                    )
                ;

            cmp_relational
                =   shift_exp
                >> *(   ch_p(T_LESSEQUAL) >> shift_exp 
                    |   ch_p(T_GREATEREQUAL) >> shift_exp 
                    |   ch_p(T_LESS) >> shift_exp 
                    |   ch_p(T_GREATER) >> shift_exp
                    )
                ;

            shift_exp
                =   add_exp
                >> *(shift_exp_clos =
                        ch_p(T_SHIFTLEFT) >> add_exp
                    |   ch_p(T_SHIFTRIGHT) >> add_exp
                    )
                ;

            add_exp
                =   multiply_exp
                >> *(   ch_p(T_PLUS) >> multiply_exp
                    |   ch_p(T_MINUS) >> multiply_exp
                    )
                ;

            multiply_exp
                =   unary_exp
                >> *(   ch_p(T_STAR) >> unary_exp
                    |   ch_p(T_DIVIDE) >> unary_exp
                    |   ch_p(T_PERCENT) >> unary_exp
                    )
                ;

            unary_exp
                =   primary_exp
                |   ch_p(T_PLUS) >> unary_exp
                |   ch_p(T_MINUS) >> unary_exp
                |   pattern_p(T_COMPL, MainTokenMask) >> unary_exp
                |   pattern_p(T_NOT, MainTokenMask) >> unary_exp
                ;

            primary_exp
                =   constant
                |   ch_p(T_LEFTPAREN) >> constant_expression 
                    >>  ch_p(T_RIGHTPAREN)
                ;

            constant
                =   ch_p(T_PP_NUMBER) 
                |   ch_p(T_INTLIT) 
                |   ch_p(T_CHARLIT) 
                ;
            // end of Wave rules

            ctor_initializer
                =   ch_p(T_COLON) >> mem_initializer_list
                ;

            mem_initializer_list
                =	  mem_initializer % ch_p(T_COMMA)
                ;

            mem_initializer
                =	  mem_initializer_id 
                >>  ch_p(T_LEFTPAREN) >> !expression_list >> ch_p(T_RIGHTPAREN)
                ;

            mem_initializer_id
                =	 !ch_p(T_COLON_COLON) >> !nested_name_specifier >> class_name
                |	  ch_p(T_IDENTIFIER)
                ;

            translation_unit
                =	 !declaration_seq 
                      >> ch_p(T_EOF)
                ;

            odd_language_extension    // read: microsoft extensions
                =	  extension_type_decorator 
                >> !comment_nest_p(ch_p(T_LEFTPAREN), ch_p(T_RIGHTPAREN))
                ;

			// Splice in some Modular C++ constructs (LSMOD)
            declaration_seq
                =	 +( declaration     [emit(self.m_del)] 
                        | preproc_stmt
                        | import_stmt 
                        | export_stmt )
                ;

            declaration
                =	  template_declaration
                |	  explicit_instantiation
                |	  explicit_specialization
                |	  linkage_specification
                |	  namespace_definition
                |	  block_declaration
                |	  function_definition
                ;

            block_declaration
                =	  simple_declaration
                |	  asm_definition
                |	  namespace_alias_definition
                |	  using_declaration
                |	  using_directive
                ;

            simple_declaration
                =	 !decl_specifier_seq >> !init_declarator_list 
                    >>  ch_p(T_SEMICOLON)
                ;

            asm_definition
                =	  ch_p(T_ASM) 
                    >>  ch_p(T_LEFTPAREN) >> ch_p(T_STRINGLIT) >> ch_p(T_RIGHTPAREN)
                    >>  ch_p(T_SEMICOLON)
                ;

            init_declarator_list
                =	  init_declarator % ch_p(T_COMMA)
                ;

            init_declarator
                =	  declarator >> !initializer
                ;

            initializer
                =	  ch_p(T_ASSIGN) >> initializer_clause
                |	  ch_p(T_LEFTPAREN) >> expression_list >> ch_p(T_RIGHTPAREN)
                ;

            initializer_clause
                =	  assignment_expression
                |	  ch_p(T_LEFTBRACE) >> initializer_list 
                    >> !ch_p(T_COMMA) >> ch_p(T_RIGHTBRACE)
                |	  ch_p(T_LEFTBRACE) >> ch_p(T_RIGHTBRACE)
                ;

            initializer_list
                =	  initializer_clause % ch_p(T_COMMA)
                ;

            expression_list
                =	  assignment_expression % ch_p(T_COMMA)
                ;

            namespace_alias_definition
                =	  ch_p(T_NAMESPACE) >> ch_p(T_IDENTIFIER) >> ch_p(T_ASSIGN)
                    >>  qualified_namespace_specifier 
                    >>  ch_p(T_SEMICOLON)
                ;

            qualified_namespace_specifier
                =	 !ch_p(T_COLON_COLON) >> !nested_name_specifier 
                    >>  namespace_name
                ;

            explicit_instantiation
                =	  template_declaration
                ;

            template_declaration
                =  !ch_p(T_EXPORT) >> ch_p(T_TEMPLATE)
                    >>  ch_p(T_LESS) >> template_parameter_list >> ch_p(T_GREATER)
                    >>  declaration
                ;

            template_parameter_list
                =	  template_parameter % ch_p(T_COMMA)
                ;

            template_parameter
                =	  type_parameter
                |	  parameter_declaration
                ;

            type_parameter
                =	  ch_p(T_CLASS) >> !ch_p(T_IDENTIFIER) 
                    >> !(ch_p(T_ASSIGN) >> type_id)
                |	  ch_p(T_TYPENAME) >> !ch_p(T_IDENTIFIER) 
                    >> !(ch_p(T_ASSIGN) >> type_id)
                |	  ch_p(T_TEMPLATE) 
                    >>  ch_p(T_LESS) >> template_parameter_list >> ch_p(T_GREATER) 
                    >>  ch_p(T_CLASS) >> !ch_p(T_IDENTIFIER) 
                    >> !(ch_p(T_ASSIGN) >> template_name)
                ;

            template_name
                =	  ch_p(T_IDENTIFIER)
                ;

            using_declaration     // optimize?
                =	  ch_p(T_USING) >> !ch_p(T_TYPENAME) >> !ch_p(T_COLON_COLON) 
                    >>  nested_name_specifier >> unqualified_id 
                    >>  ch_p(T_SEMICOLON)
                |	  ch_p(T_USING) >> ch_p(T_COLON_COLON) >> unqualified_id 
                    >>  ch_p(T_SEMICOLON)
                ;

            using_directive
                =	  ch_p(T_USING) >> ch_p(T_NAMESPACE) >> !ch_p(T_COLON_COLON) 
                    >> !nested_name_specifier >> namespace_name 
                    >>  ch_p(T_SEMICOLON)
                ;

            explicit_specialization
                =	  ch_p(T_TEMPLATE) >> ch_p(T_LESS) >> ch_p(T_GREATER) 
                    >>  declaration
                ;

            linkage_specification
                =	  ch_p(T_EXTERN) >> ch_p(T_STRINGLIT) 
                    >>  (   ch_p(T_LEFTBRACE) >> !declaration_seq >> ch_p(T_RIGHTBRACE)
                        |	  declaration
                        )
                ;

            namespace_definition
                =	  named_namespace_definition
                |	  unnamed_namespace_definition    // TODO: optimize?
                ;

            named_namespace_definition
                =	  original_namespace_definition
           //   |   extension_namespace_definition // optimization: extension namespace is syntactically identical
                ;

            original_namespace_definition
                =	  ch_p(T_NAMESPACE) >> ch_p(T_IDENTIFIER)
                    >>  ch_p(T_LEFTBRACE) >> namespace_body >> ch_p(T_RIGHTBRACE)
                ;

            extension_namespace_definition
                =	  ch_p(T_NAMESPACE) >> original_namespace_name 
                    >>  ch_p(T_LEFTBRACE) >> namespace_body >> ch_p(T_RIGHTBRACE)
                ;

            original_namespace_name
                =	  ch_p(T_IDENTIFIER)
                ;

            unnamed_namespace_definition
                =	  ch_p(T_NAMESPACE) 
                    >>  ch_p(T_LEFTBRACE) >> namespace_body >> ch_p(T_RIGHTBRACE)
                ;

            namespace_body
                =	 !declaration_seq
                ;

            function_definition
                =	  function_definition_helper 
                    >> !ctor_initializer >> !function_body  // removed semicolons
                |	  decl_specifier_seq >> declarator >> function_try_block
                |	  declarator >> function_try_block
                ;

            function_definition_helper
                =	 !decl_specifier_seq >> declarator
                |	  declarator
                ;

            function_try_block
                =	  ch_p(T_TRY) 
                    >> !ctor_initializer >> function_body >> handler_seq
                ;

            handler_seq
                =	 +handler
                ;

            handler // TODO
                =	  ch_p(T_CATCH) 
                    >>  comment_nest_p(ch_p(T_LEFTPAREN), ch_p(T_RIGHTPAREN)) 
                    >>  compound_statement
                ;

            declarator
                =	 *(   ptr_operator 
                    |   odd_language_extension
                    ) 
                    >> direct_declarator
                ;

            direct_declarator
                =	  (   declarator_id
                    |   ch_p(T_LEFTPAREN) >> declarator >> ch_p(T_RIGHTPAREN)
                    )  
                    >> *parameters_or_array_spec
                ;

            parameters_or_array_spec
                =	  ch_p(T_LEFTPAREN) >> parameter_declaration_clause >> ch_p(T_RIGHTPAREN)
                    >> !cv_qualifier_seq >> !exception_specification
                |	  ch_p(T_LEFTBRACKET) >> !constant_expression >> ch_p(T_RIGHTBRACKET)
                ;

            exception_specification     // TODO
                =	  ch_p(T_THROW) 
                    >>  comment_nest_p(ch_p(T_LEFTPAREN), ch_p(T_RIGHTPAREN))
                ;

            abstract_declarator
                =	 +(   ptr_operator 
                    |   odd_language_extension
                    ) 
                    >> !direct_abstract_declarator
                |	  direct_abstract_declarator
                ;

            direct_abstract_declarator
                =	  ch_p(T_LEFTPAREN) >> abstract_declarator >> ch_p(T_RIGHTPAREN)
                    >> *direct_abstract_declarator_helper
                ;

            direct_abstract_declarator_helper
                =	  ch_p(T_LEFTPAREN) >> parameter_declaration_clause >> ch_p(T_RIGHTPAREN)
                    >> !cv_qualifier_seq >> !exception_specification
                |	  ch_p(T_LEFTBRACKET) >> !constant_expression >> ch_p(T_RIGHTBRACKET)
                ;

            parameter_declaration_clause
                =	  parameter_declaration_list >> ch_p(T_COMMA) 
                    >> ch_p(T_ELLIPSIS)
                |	 !parameter_declaration_list >> !ch_p(T_ELLIPSIS)
                ;

            parameter_declaration_list
                =	  parameter_declaration % ch_p(T_COMMA)
                ;


            parameter_declaration
                =	  decl_specifier_seq 
                    >> !(declarator | abstract_declarator) 
                    >> !(ch_p(T_ASSIGN) >> assignment_expression)
                ;

            assignment_expression
                =	  constant_expression     // TODO later assignment expressions
                |	  ch_p(T_IDENTIFIER)
                ;

            declarator_id
                =	 !ch_p(T_COLON_COLON)
                    >>	(   id_expression
                        |	 !nested_name_specifier >> type_name
                        )
                ;

            id_expression
                =	  unqualified_id
                |   qualified_id
                ;

            qualified_id
                =	  nested_name_specifier >> !ch_p(T_TEMPLATE) >> unqualified_id
                ;

            unqualified_id
                =	  operator_function_id
                |	  conversion_function_id 
                |	  ch_p(T_COMPL) >> class_name
                |	  template_id
                |	  ch_p(T_IDENTIFIER)
                ;

            operator_function_id
                =	  ch_p(T_OPERATOR) >> operator_sym    // this is called 'operator' in the std grammar
                ;
                
            operator_sym 
                =	  ch_p(T_DELETE) >> ch_p(T_LEFTBRACKET) >> ch_p(T_RIGHTBRACKET)
                |	  ch_p(T_NEW) >> ch_p(T_LEFTBRACKET) >> ch_p(T_RIGHTBRACKET)
                |	  ch_p(T_LEFTBRACKET) >> ch_p(T_RIGHTBRACKET)
                |	  ch_p(T_LEFTPAREN) >> ch_p(T_RIGHTPAREN)
                |	  pattern_p(OperatorTokenType, TokenTypeMask)
                ;

            conversion_function_id
                =   ch_p(T_OPERATOR) >> conversion_type_id
                ;

            conversion_type_id
                =   type_specifier_seq >> !conversion_declarator
                ;

            type_id
                =	  type_specifier_seq >> !abstract_declarator
                ;


            conversion_declarator
                = ptr_operator >> !conversion_declarator
                ;

            function_body
                =	compound_statement
                ;

            compound_statement
                =	  comment_nest_p(ch_p(T_LEFTBRACE), ch_p(T_RIGHTBRACE))
                ;   // TODO later


            ptr_operator
                =	  ch_p(T_STAR) >> !cv_qualifier_seq
                |	  ch_p(T_AND)
                |  !ch_p(T_COLON_COLON) >> nested_name_specifier 
                    >> ch_p(T_STAR) >> !cv_qualifier_seq
                ;


            decl_specifier
                =	  no_type_decl_specifier
                |   type_specifier
                ;

            no_type_decl_specifier 
                =	  storage_class_specifier
                |	  function_specifier
                |	  ch_p(T_FRIEND)
                |	  ch_p(T_TYPEDEF)
                |	  cv_qualifier
                |	  odd_language_extension
                ;

            type_specifier_seq
                =	 +type_specifier
                ;

            type_specifier
                =	  enum_specifier
                |	  class_specifier
                |	  elaborated_type_specifier
                |	  simple_type_specifier
                ;

            cv_qualifier_seq
                =	  cv_qualifier >> !cv_qualifier_seq
                ;

            cv_qualifier
                =	  ch_p(T_CONST) 
                |   ch_p(T_VOLATILE)
                ;

            enum_specifier 
                =	  enum_keyword >> !ch_p(T_IDENTIFIER) 
                    >> ch_p(T_LEFTBRACE) >> !enumerator_list >> ch_p(T_RIGHTBRACE)
                ;

            enum_keyword
                =	  ch_p(T_ENUM)
                ;

            enumerator_list
                =	  enumerator_definition % ch_p(T_COMMA) 
                    >> !ch_p(T_COMMA) 
                    // TODO find out if this last COMMA_T is an MS-"extension"?
                    // it seems not to be in the grammar but MSVC 7.0 accepts it.
                ;

            enumerator_definition
                =	enumerator >> !(ch_p(T_ASSIGN) >> constant_expression)
                ;

            enumerator
                =	  ch_p(T_IDENTIFIER)
                ;


            simple_type_specifier
                =   !ch_p(T_COLON_COLON) >> !nested_name_specifier 
                    >>  ch_p(T_TEMPLATE) >> template_id
                |   sign_specifier >> !simple_type_name 
                |   simple_type_name
                |	 !ch_p(T_COLON_COLON) >> !nested_name_specifier >> type_name
                ;

            class_head 
                =	 !access_specifier >> *odd_language_extension 
                    >>  class_key >> *odd_language_extension 
                    >>  (  !ch_p(T_IDENTIFIER) 
                        |   nested_name_specifier >> !ch_p(T_TEMPLATE) >> ch_p(T_IDENTIFIER)
                        )
                    >> !base_clause
                ;

            sign_specifier
                =	  ch_p(T_SIGNED)
                |   ch_p(T_UNSIGNED)
                ;

            type_name
                =	  class_name
                |	  enum_name
                |	  typedef_name
                ;

            elaborated_type_specifier
                =	  class_key >> *odd_language_extension >> !ch_p(T_COLON_COLON) 
                    >>  !nested_name_specifier >> ch_p(T_IDENTIFIER)
                |	  ch_p(T_ENUM) >> !ch_p(T_COLON_COLON) 
                    >> !nested_name_specifier >> ch_p(T_IDENTIFIER)
                |	  ch_p(T_TYPENAME) >> !ch_p(T_COLON_COLON) 
                    >>  nested_name_specifier >> ch_p(T_IDENTIFIER) 
                    >> ch_p(T_LESS) >> template_argument_list >> ch_p(T_GREATER)
                |	  ch_p(T_TYPENAME) >> !ch_p(T_COLON_COLON) 
                    >> !nested_name_specifier >> ch_p(T_IDENTIFIER)
                ;

            template_argument_list 
                =	  template_argument % ch_p(T_COMMA)
                ;

            template_argument
                =	  longest_d
                    [
                        type_id
                    |   assignment_expression
                    |   template_name
                    ]
                ;

            class_key
                =	  class_keywords
                ;

            class_keywords 
                =	  ch_p(T_CLASS)
                |   ch_p(T_STRUCT)
                |   ch_p(T_UNION)
                ;

            nested_name_specifier
                =	  class_or_namespace_name >> ch_p(T_COLON_COLON) 
                    >>  ch_p(T_TEMPLATE) >> nested_name_specifier
                |	  class_or_namespace_name >> ch_p(T_COLON_COLON) 
                    >> !nested_name_specifier
                ;

            class_or_namespace_name
                =	  class_name 
                |   namespace_name
                ;

            class_name 
                =	  template_id
                |	  ch_p(T_IDENTIFIER)
                ;

            enum_name
                =	  ch_p(T_IDENTIFIER)
                ;

            typedef_name
                =	  ch_p(T_IDENTIFIER)
                ;

            namespace_name    // TODO
                =	  ch_p(T_IDENTIFIER)
                ;

            template_id 
                =   template_name 
                    >> ch_p(T_LESS) >> template_argument_list >> ch_p(T_GREATER)
                ;

            //
            //  This is kind of a HACK. We want to prevent the decl_specifier_seq 
            //  from eating the whole declaration, including the identifier. 
            //  Therefore in the sequence, we only allow one 'unknown' word 
            //  (the type_specifier), the rest of the decl_specifier sequence 
            //  must consist of known keywords or constructs (the 
            //  no_type_decl_specifier).
            //  This means that a declaration like:
            //     MYDLL_EXPORT int f(); 
            //  will not be accepted unless the MYDLL_EXPORT is properly 
            //  expanded by the preprocessor first.
            //     
            //  This should not cause any problems normally, it just means that 
            //  this rule is not very robust in the case where not all symbols 
            //  are known.
            //
            decl_specifier_seq
                =	 *no_type_decl_specifier >> type_specifier >> *no_type_decl_specifier
                ;

            //  The following rule is more according to the standard grammar
            //  decl_specifier_seq // adapted
            //	    =	  decl_specifier >> decl_specifier_seq
            //		  |	  (decl_specifier - (declarator_id >> parameters_or_array_spec )) 
            //	    ;

            storage_class_specifier
                =	  ch_p(T_AUTO)
                |   ch_p(T_REGISTER)
                |   ch_p(T_STATIC)
                |   ch_p(T_EXTERN)
                |   ch_p(T_MUTABLE)
                ;

            function_specifier
                =	  ch_p(T_INLINE)
                |   ch_p(T_VIRTUAL)
                |   ch_p(T_EXPLICIT)
                ;

            class_specifier
                =	  class_head 
                    >> ch_p(T_LEFTBRACE) >> !member_specification >> ch_p(T_RIGHTBRACE)
                ;

            member_specification
                =	 +(	  access_specifier >> ch_p(T_COLON)
                    |	  member_declaration 
                    )
                ;

      //				member_specification
      //					=	access_specifier >> COLON_T >> !member_specification
      //					|	member_declaration >> !member_specification
      //					;

            member_declaration
                =   using_declaration
                |	  template_declaration
                |	 !decl_specifier_seq >> !member_declarator_list 
                    >> ch_p(T_SEMICOLON)
                |	  function_definition >> 
                   !ch_p(T_SEMICOLON)
                |	  qualified_id 
                    >> ch_p(T_SEMICOLON)
                ;

            member_declarator_list
                =	  member_declarator % ch_p(T_COMMA)
                ;

            member_declarator
                =	 !ch_p(T_IDENTIFIER) >> ch_p(T_COLON) >> constant_expression
                |	  declarator >> !(pure_specifier | constant_initializer)
                ;

            pure_specifier
                =   ch_p(T_ASSIGN) >> ch_p(T_INTLIT)
                ;

            constant_initializer
                =   ch_p(T_ASSIGN) >> constant_expression
                ;

            access_specifier 
                =   ch_p(T_PUBLIC)
                |   ch_p(T_PROTECTED)
                |   ch_p(T_PRIVATE)
                ;

            base_clause
                =   ch_p(T_COLON) >> base_specifier_list
                ;

            base_specifier_list
                =	  base_specifier % ch_p(T_COMMA)
                ;

            base_specifier
                =	  ch_p(T_VIRTUAL) >> !access_specifier >> !ch_p(T_COLON_COLON) 
                    >> !nested_name_specifier >> class_name
                |	  access_specifier >> !ch_p(T_VIRTUAL) >> !ch_p(T_COLON_COLON)
                    >> !nested_name_specifier >> class_name
                |	 !ch_p(T_COLON_COLON) >> !nested_name_specifier >> class_name
                ;

            extension_type_decorator
                =	  ch_p(T_MSEXT_CDECL) 
                |   ch_p(T_MSEXT_DECLSPEC)
                |   ch_p(T_MSEXT_BASED)
                |   ch_p(T_MSEXT_FASTCALL) 
                |   ch_p(T_MSEXT_INLINE)
                ;

            simple_type_name
                =	  ch_p(T_CHAR)
                |   ch_p(T_WCHART)
                |   ch_p(T_BOOL)
                |   ch_p(T_SHORT)
                |   ch_p(T_INT)
                |   ch_p(T_LONG)
                |   ch_p(T_FLOAT)
                |   ch_p(T_DOUBLE)
                |   ch_p(T_VOID)
                |   ch_p(T_MSEXT_INT64)
                |   ch_p(T_MSEXT_INT8)
                |   ch_p(T_MSEXT_INT16)
                |   ch_p(T_MSEXT_INT32)
                ;


		//
		// Modular C++ Grammar below
		//

			preproc_stmt
			    = ch_p(T_PP_LINE)
			      >> ch_p(T_INTLIT)
			      >> ch_p(T_STRINGLIT)
			    ;

			skip_block
			    = ch_p(T_LEFTBRACE) 
			      >> *declaration
			      >> ch_p(T_RIGHTBRACE)
			    ;

			export_stmt 
			    = ch_p(T_EXPORT)
			      >> ch_p(T_NAMESPACE) 
			      >> ch_p(T_IDENTIFIER) [ save_as(self.m_ids) ]
			                         [ decl_module(self.m_del) ]
			      // this we'll have to break down for public and private
			      // sections, later.
			      >> skip_block [ finish_decl(self.m_del) ]
			    ; 

			import_stmt
			    = ch_p(T_IMPORT)
			      >> ch_p(T_NAMESPACE)
			      >> ch_p(T_IDENTIFIER) [ import_module(self.m_del) ]
			      >> ch_p(T_SEMICOLON)
			    ;

/*			translation_unit 
			    = *(export_stmt [ &print ]
			        | import_stmt 
			        | skip [ &print ] [emit(self.m_del)]
			        ) 
			      >> ch_p(T_EOF)
			    ;  */
			
			
/*			BOOST_SPIRIT_DEBUG_RULE(export_stmt);
			BOOST_SPIRIT_DEBUG_RULE(import_stmt);
			BOOST_SPIRIT_DEBUG_RULE(skip_block); */
		}		
		boost::spirit::rule<ScannerT> const& start () { return translation_unit; }
	};
};


#endif

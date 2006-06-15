#ifndef INCLUDE_DECL_GRAMMAR_H
#define INCLUDE_DECL_GRAMMAR_H

//#include "actions.h"

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_grammar_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/cpp_exceptions.hpp>

using namespace boost::spirit;
using namespace boost::wave;

/*
The grammar here's based on two sources:
1. Hartmut Kaiser's C grammar for Spirit.
2. GCC's C++ Parser: gcc/gcc/cp/parser.c

For #2, I'll  mention the function names as appropriate.
*/

struct decl_grammar : public grammar<decl_grammar> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  simple_type_spec, type_spec, decl_spec, simple_decl,
		                qual_namesp_spec, namesp_alias_def, using_decl,
		                using_dir, block_decl, decl, translation_unit;
		
		definition (decl_grammar const& self) {
			// cp_parser_simple_type_specifier
			//    simple-type-specifier:
			//      :: [opt] nested-name-specifier [opt] type-name
			//      :: [opt] nested-name-specifier template template-id
			//      char
			//      wchar_t
			//      bool
			//      short
			//      int
			//      long
			//      signed
			//      unsigned
			//      float
			//      double
			//      void
			simple_type_spec
			 = qual_namesp_spec // a little abusive, but ok here
//			 | we're ignoring templates for a little while.
			 | ch_p(T_CHAR) | ch_p(T_WCHART) | ch_p(T_BOOL) | ch_p(T_SHORT)
			 | ch_p(T_INT) | ch_p(T_LONG) | ch_p(T_SIGNED) | ch_p(T_UNSIGNED)
			 | ch_p(T_FLOAT) | ch_p(T_DOUBLE) | ch_p(T_VOID)
			 ;

			// cp_parse_type_specifier
			//    type-specifier:
			//      simple-type-specifier
			//      class-specifier
			//      enum-specifier
			//      elaborated-type-specifier
			//      cv-qualifier
			type_spec
			 = simple_type_spec
//			 | ignoring the rest for a bit. 
			 ;
			// cp_parser_decl_specifier_seq
			//    decl-specifier-seq:
			//      decl-specifier-seq [opt] decl-specifier
			// 
			//    decl-specifier:
			//      storage-class-specifier
			//      type-specifier
			//      function-specifier
			//      friend
			//      typedef
			// (this one's a bit of a bitch).
			decl_spec
			 = 	  /* storage-class-specifier:
					   auto
					   register
					   static
					   extern
					   mutable */
				( ch_p(T_AUTO) | ch_p(T_REGISTER) | ch_p(T_STATIC)
				 | ch_p(T_EXTERN) | ch_p(T_MUTABLE) )
		     |  ch_p(T_FRIEND) // friend
//			 |  func_spec // function-specifier
			 |  type_spec // type-specifier
//			 |  typedef_spec  // typedef
			 ;
			 
			// cp_parser_simple_declaration
			//    simple-declaration:
			//      decl-specifier-seq [opt] init-declarator-list [opt] ;
			// 
			//    init-declarator-list:
			//      init-declarator
			//      init-declarator-list , init-declarator
			simple_decl
			 = *decl_spec 
//			 >> *( init_decl *(ch_p(T_COMMA) >> init_decl) )
			 >> ch_p(T_SEMICOLON);

			// cp_parser_qualified_namespace_specifier
			//  qualified-namespace-specifier:
			//    :: [opt] nested-name-specifier [opt] namespace-name
			// we simplified quite a bit, as we don't care about name lookup
			// for these.
			qual_namesp_spec
			 = !ch_p(T_COLON_COLON)
			  >> *(ch_p(T_COLON_COLON) >> ch_p(T_IDENTIFIER))
			  >> ch_p(T_IDENTIFIER);
			
			// cp_parser_namespace_alias_definition
			//  namespace-alias-definition:
			//   namespace identifier = qualified-namespace-specifier ;
			namesp_alias_def
			 = ch_p(T_NAMESPACE) >> ch_p(T_IDENTIFIER) >> ch_p(T_ASSIGN)
			   >> qual_namesp_spec >> ch_p(T_SEMICOLON);
			
			// cp_parser_using_declaration
			//  using-declaration:
			//   using typename [opt] :: [opt] nested-name-specifier unqualified-id ;
			//   using :: unqualified-id ;  */
			using_decl
			 = ch_p(T_USING) >> !ch_p(T_TYPENAME) 
			 >> *(ch_p(T_COLON_COLON) >> ch_p(T_IDENTIFIER))
			 >> ch_p(T_IDENTIFIER) >> ch_p(T_SEMICOLON);
			 
			// cp_parser_using_directive
			using_dir
			 = ch_p(T_USING) >> ch_p(T_NAMESPACE) 
			    >> qual_namesp_spec >> ch_p(T_SEMICOLON);
			 
			 
			// cp_parser_block_declaration:
			// 	block-declaration:
			// 	 simple-declaration
			// 	 asm-definition (unused)
			// 	 namespace-alias-definition
			// 	 using-declaration (merged)
			// 	 using-directive
			block_decl
			 = simple_decl
			 | namesp_alias_def
			 | using_decl
			 | using_dir;
			
			// cp_parser_declaration:
			//  declaration:
			//    block-declaration
			//    function-declaration
			//    template-declaration
			//    explicit-instantiation
			//    explicit-specialization
			//    linkage-specification
			//    namespace definition
			decl 
			  = block_decl 
// 			  | func_decl
// 			  | templ_decl
// 			  | expl_templ_inst
// 			  | expl_templ_spec
// 			  | linkage_spec
// 			  | namespace_def
			  ;
			
			
			// cp_parser_translation_unit:
			//   translation-unit: 
			//     declaration-seq [opt]
			translation_unit = 
			   *decl ; //>> ch_p(T_EOF);
			
// 			BOOST_SPIRIT_DEBUG_RULE(var_decl);
// 			BOOST_SPIRIT_DEBUG_RULE(type_decl);
// 			BOOST_SPIRIT_DEBUG_RULE(decl);
// 			BOOST_SPIRIT_DEBUG_RULE(body);
		}
		rule<ScannerT> const& start () { return translation_unit; }
	};
};



#endif

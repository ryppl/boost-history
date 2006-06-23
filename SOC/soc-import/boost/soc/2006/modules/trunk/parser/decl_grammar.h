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
The grammar here's based on two sources:
1. Hartmut Kaiser's C grammar for Spirit.
2. GCC's C++ Parser: gcc/gcc/cp/parser.c

For #2, I'll  mention the function names as appropriate.
*/

struct decl_grammar : public boost::spirit::grammar<decl_grammar> {
	// iterator splicing engine.
	mutable std::vector<std::string> m_ids;
	mutable OutputDelegate& m_del;
	
	decl_grammar (OutputDelegate& del) : m_del(del) {}
	
	template<typename ScannerT>
	struct definition {
		boost::spirit::rule<ScannerT>  skip_semi, skip_block, skip, import_stmt,
		                export_stmt, translation_unit, inner_block;
		
		definition (decl_grammar const& self) {
			
			skip_semi 
			    =     *(~ch_p(T_SEMICOLON)) >> ch_p(T_SEMICOLON)
			    ;
			 
			skip_block 
			    = ch_p(T_LEFTBRACE) 
			      >> *inner_block
			      >> ch_p(T_RIGHTBRACE)
			    ;

			inner_block
			    = +(skip_block | ~ch_p(T_RIGHTBRACE))
			    ;

			skip 
			 =  // extern "C" {...};
			    ( ch_p(T_EXTERN) 
			      >> ch_p(T_STRINGLIT) 
			      >> skip_block 
			      >> skip_semi)
			     // anything related to templates
			 |  (!ch_p(T_EXPORT) 
			      >> ch_p(T_TEMPLATE) 
			      >> !skip_block 
			      >> skip_semi)
			  // declarations (fixme- won't work for function defs)
//			 |  ( (~ch_p(T_IMPORT) & ~ch_p(T_EXPORT)) >> !skip_block >> skip_semi)
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

			translation_unit 
			    = *(export_stmt [ &print ]
			        | import_stmt 
			        | skip [ &print ] [emit(self.m_del)]
			        ) 
			      >> ch_p(T_EOF)
			    ; 
			
			
			BOOST_SPIRIT_DEBUG_RULE(export_stmt);
			BOOST_SPIRIT_DEBUG_RULE(import_stmt);
			BOOST_SPIRIT_DEBUG_RULE(skip_block);
		}		
		boost::spirit::rule<ScannerT> const& start () { return translation_unit; }
	};
};


#endif

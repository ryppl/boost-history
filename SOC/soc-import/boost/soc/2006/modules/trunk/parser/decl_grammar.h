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
		rule<ScannerT>  skip_semi, skip_block, skip, import_stmt,
		                export_stmt, translation_unit;
		
		definition (decl_grammar const& self) {
			
			skip_semi =
			 *(~ch_p(T_SEMICOLON)) >> ch_p(T_SEMICOLON);
			skip_block =
			 *(~ch_p(T_LEFTBRACE)) >> ch_p(T_LEFTBRACE) 
			 >> *(~ch_p(T_LEFTBRACE) | skip_block)
			 >> ch_p(T_RIGHTBRACE);
			
			skip 
			 =
			  // extern "C" {...};
			  (ch_p(T_EXTERN) >> ch_p(T_IDENTIFIER) >> skip_block >> skip_semi)
			  // anything related to templates
			 |(!ch_p(T_EXPORT) >> ch_p(T_TEMPLATE) >> !skip_block >> skip_semi)
			  // declarations
			 |( (/*~ch_p(T_IMPORT) &*/ ~ch_p(T_EXPORT)) >> !skip_block >> skip_semi)
			 ;


			// for right now, accept all of C++, we'll exclude what we want
			// to handle seperately.
			translation_unit = 
			   *skip ; 
			
			BOOST_SPIRIT_DEBUG_RULE(skip);
		}		
		rule<ScannerT> const& start () { return translation_unit; }
	};
};



#endif

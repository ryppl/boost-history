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

using namespace boost::spirit;
using namespace boost::wave;

struct match_token : public parser<match_token> {
	typedef match_token self_t;
	
	match_token (token_id id) : m_id(id) {}
	
	template<typename ScannerT>
	typename parser_result<self_t, ScannerT>::type
	parse (ScannerT const& scan) const {
		if (!scan.at_end ()) {
			typename ScannerT::iterator_t save = scan.first;
			++scan;
			if (*save == m_id)
				return scan.create_match (1, nil_t(), save, scan.first);
		} 
		return scan.no_match ();
	}
private:
	token_id   m_id;
};

match_token
t(token_id tk) {
	return match_token(tk);
}

struct nomatch_token : public parser<nomatch_token> {
	typedef nomatch_token self_t;
	
	nomatch_token (token_id id) : m_id(id) {}
	
	template<typename ScannerT>
	typename parser_result<self_t, ScannerT>::type
	parse (ScannerT const& scan) const {
		typename ScannerT::iterator_t save = scan.first;
		if (!scan.at_end ()) {
			++scan;
			if (*save == m_id)
				return scan.no_match ();
		} 
		return scan.create_match (1, nil_t(), save, scan.first);
	}
private:
	token_id   m_id;
};

nomatch_token
n(token_id tk) {
	return nomatch_token(tk);
}

// just a little debugging support for decl_grammar
void
break_here (context_iter_t ,context_iter_t ) {
	std::cout << "internal break" << std::endl;
}

/*
The grammar here's based on two sources:
1. Hartmut Kaiser's C grammar for Spirit.
2. GCC's C++ Parser: gcc/gcc/cp/parser.c

For #2, I'll  mention the function names as appropriate.
*/

struct decl_grammar : public grammar<decl_grammar> {
	// iterator splicing engine.

	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  skip_semi, skip_block, //skip, import_stmt,
		                export_stmt, translation_unit;
		
		definition (decl_grammar const& self) {
			
			skip_semi 
			    =     *(n(T_SEMICOLON)) >> t(T_SEMICOLON)
			    ;
			 
			skip_block 
			    = t(T_LEFTBRACE) 
			      >> t(T_RIGHTBRACE)
			    ;

/*			skip 
			 =
			  // extern "C" {...};
			  (ch_p(T_EXTERN) >> ch_p(T_IDENTIFIER) >> skip_block >> skip_semi)
			  // anything related to templates
			 |(!ch_p(T_EXPORT) >> ch_p(T_TEMPLATE) >> !skip_block >> skip_semi)
			  // declarations
			 |( (~ch_p(T_IMPORT) & ~ch_p(T_EXPORT)) >> !skip_block >> skip_semi)
			 ;*/

			export_stmt 
			    = t(T_EXPORT) 
			      >> t(T_NAMESPACE) 
			      >> t(T_IDENTIFIER)
			      >> skip_block 
			    ; 

			// for right now, accept all of C++, we'll exclude what we want
			// to handle seperately.
			translation_unit 
			    = *( export_stmt[ &print ] ) 
			      >> t(T_EOF)
			    ; 
			
			
// 			BOOST_SPIRIT_DEBUG_RULE(export_stmt);
// 			BOOST_SPIRIT_DEBUG_RULE(skip_block);
		}		
		rule<ScannerT> const& start () { return translation_unit; }
	};
};



#endif

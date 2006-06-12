#ifndef INCLUDE_DECL_GRAMMAR_H
#define INCLUDE_DECL_GRAMMAR_H

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_grammar_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/cpp_exceptions.hpp>

struct decl_grammar : public grammar<decl_grammar> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  var_decl, type_decl, decl, body;
		
		definition (decl_grammar const& self) {
			body =
			  ch_p(T_LEFTBRACE) 
			  >> (~ch_p(T_LEFTBRACE) | body) 
			  >> ch_p(T_RIGHTBRACE);
			// these are obviously insufficient for production use, but that'll
			// come later.
						
			var_decl = ch_p(T_IDENTIFIER) >> ch_p(T_SPACE)
			           >> ch_p(T_IDENTIFIER) >> ch_p(T_SEMICOLON)
			           >> ch_p(T_NEWLINE);
			
			// completely ignoring typedef and function defs right now.
			type_decl = ( ch_p(T_STRUCT)  | ch_p(T_CLASS) ) >> ch_p(T_IDENTIFIER)   
						>> !body >> ch_p(T_SEMICOLON);
			
			decl = *(var_decl | type_decl);
			BOOST_SPIRIT_DEBUG_RULE(var_decl);
			BOOST_SPIRIT_DEBUG_RULE(type_decl);
			BOOST_SPIRIT_DEBUG_RULE(decl);
			BOOST_SPIRIT_DEBUG_RULE(body);
		}
		rule<ScannerT> const& start () { return decl; }
	};
};



#endif

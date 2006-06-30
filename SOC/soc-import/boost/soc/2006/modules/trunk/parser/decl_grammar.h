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

/*
This grammar's a minimalistic brace-counter, with traps for
modular c++ constructs.
*/

struct decl_grammar : public boost::spirit::grammar<decl_grammar> {
	mutable std::vector<std::string> m_ids;
	// iterator splicing engine.
	mutable OutputDelegate& m_del;
	
	decl_grammar (OutputDelegate& del) : m_del(del) {}
	
	template<typename ScannerT>
	struct definition {
        typedef boost::spirit::rule<ScannerT> rule_type;

		rule_type translation_unit, declaration, inner_decl,
		          access_specifier, skip_block, export_stmt, import_stmt;
		
		definition (decl_grammar const& self) {
			using namespace boost::spirit;
            using namespace boost::wave;
            using boost::wave::util::pattern_p;
			using boost::phoenix::var;

            translation_unit
                =	 *declaration
                      >> ch_p(T_EOF)
                ;
                
            declaration
			    =   +(   ~ch_p(T_IMPORT) & ~ch_p(T_EXPORT) 
			           & ~ch_p(T_EOF)    & ~ch_p(T_RIGHTBRACE) 
			           & ~ch_p(T_LEFTBRACE) ) 
			         [ emit (var(self.m_del)) ]
			         
			    |   ( ch_p(T_IMPORT) 
			          >> import_stmt )
			          
			    |   ( ch_p(T_EXPORT) 
			          >> ch_p(T_NAMESPACE) 
			          >> export_stmt )
			          
			    |   ( ch_p(T_EXPORT) 
			          >> ch_p(T_TEMPLATE) ) 
			         [ emit (var(self.m_del)) ]
			         
			    |   ( ch_p(T_LEFTBRACE) 
			          >> *inner_decl 
			          >> ch_p(T_RIGHTBRACE) )
			          [ emit (var(self.m_del)) ]
			    ;
			    
			inner_decl
			   = +(declaration | ~ch_p(T_RIGHTBRACE))
			   ;

			access_specifier
			    = ( ch_p(T_PUBLIC)    [go_public (self.m_del)] 
					| ch_p(T_PRIVATE) [go_private (self.m_del)]
				  )
			        >> ch_p(T_COLON)
				;

			skip_block
			    = ch_p(T_LEFTBRACE) 
			      >> *(access_specifier | inner_decl)
			      >> ch_p(T_RIGHTBRACE)
			    ;

			export_stmt 
			    = ch_p(T_IDENTIFIER) [ save_as(self.m_ids) ]
			                         [ decl_module(self.m_del) ]
			      >> ch_p(T_LEFTBRACE)
			      >> *(access_specifier | inner_decl)
			      >> ch_p(T_RIGHTBRACE) [ finish_decl(var(self.m_del)) ]
			    ; 

			import_stmt
			    = ch_p(T_NAMESPACE)
			      >> ch_p(T_IDENTIFIER) [ import_module(self.m_del) ]
			      >> ch_p(T_SEMICOLON)
			    ;

			BOOST_SPIRIT_DEBUG_RULE(translation_unit);
			BOOST_SPIRIT_DEBUG_RULE(declaration);
			BOOST_SPIRIT_DEBUG_RULE(inner_decl);
			BOOST_SPIRIT_DEBUG_RULE(access_specifier);
			BOOST_SPIRIT_DEBUG_RULE(skip_block);
			BOOST_SPIRIT_DEBUG_RULE(export_stmt);
			BOOST_SPIRIT_DEBUG_RULE(import_stmt);
		}
		
		rule_type const& start () { return translation_unit; }
	};
};


#endif
